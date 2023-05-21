#include "external_texture.h"
#include <sys/syscall.h>
#include <unistd.h>

Error ExternalTexture::create(const RD::TextureFormat &p_format, const RD::TextureView &p_view, const Vector<Vector<uint8_t>> &p_data) {
	view = p_view;
	format = p_format;
	rid = RD::get_singleton()->external_texture_create(p_format, p_view, &fd, p_data);
	ERR_FAIL_COND_V_MSG(!rid.is_valid(), ERR_CANT_CREATE, "Unable to create external texture");

    return OK;
}

Error ExternalTexture::import(const RD::TextureFormat &p_format, const RD::TextureView &p_view, int p_pid, int p_fd) {
	int pid_fd = syscall(SYS_pidfd_open, p_pid, 0);
	fd = syscall(SYS_pidfd_getfd, pid_fd, p_fd, 0);
	ERR_FAIL_COND_V_MSG(fd == -1, ERR_CANT_ACQUIRE_RESOURCE, "Unable to get fd " + itos(p_fd) + " from process " + itos(p_pid));
	print_line("Fd " + itos(p_fd) + " gotten from process " + itos(p_pid));

	view = p_view;
	format = p_format;
	rid = RD::get_singleton()->external_texture_import(format, view, fd);
	ERR_FAIL_COND_V_MSG(!rid.is_valid(), ERR_CANT_CREATE, "Unable to import external texture from fd " + itos(fd));

    return OK;
}

Error ExternalTexture::_copy(RID p_texture, bool p_from) {
	ERR_FAIL_COND_V_MSG(!rid.is_valid(), ERR_UNAVAILABLE, "ExternalTexture is not valid. Create or import first");
	ERR_FAIL_COND_V_MSG(!p_texture.is_valid(), ERR_INVALID_PARAMETER, "Parameter texture is not valid.");

	Vector3 size = {
		static_cast<float>(format.width),
		static_cast<float>(format.height),
		static_cast<float>(format.depth)
	};
	Vector3 zero = { 0, 0, 0 };

	if (p_from) {
		return RD::get_singleton()->texture_copy(p_texture, rid, zero, zero, size, 0, 0, 0, 0);
	} else {
		return RD::get_singleton()->texture_copy(rid, p_texture, zero, zero, size, 0, 0, 0, 0);
	}
}

Error ExternalTexture::_create(const Ref<RDTextureFormat> &p_format, const Ref<RDTextureView> &p_view, const TypedArray<PackedByteArray> &p_data) {
	ERR_FAIL_COND_V(p_format.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(p_view.is_null(), ERR_INVALID_PARAMETER);

	Vector<Vector<uint8_t>> data;
	for (int i = 0; i < p_data.size(); i++) {
		Vector<uint8_t> byte_slice = p_data[i];
		ERR_FAIL_COND_V(byte_slice.is_empty(), ERR_INVALID_PARAMETER);
		data.push_back(byte_slice);
	}
	return create(RD::_get_base(p_format), RD::_get_base(p_view), data);
}

Error ExternalTexture::_import(const Ref<RDTextureFormat> &p_format, const Ref<RDTextureView> &p_view, int p_main_pid, int p_fd) {
	ERR_FAIL_COND_V(p_format.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(p_view.is_null(), ERR_INVALID_PARAMETER);

	return import(RD::_get_base(p_format), RD::_get_base(p_view), p_main_pid, p_fd);
}

void ExternalTexture::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create", "format", "view"), &ExternalTexture::_create, DEFVAL(Array()));
	ClassDB::bind_method(D_METHOD("import", "format", "view", "fid", "fd"), &ExternalTexture::_import);
	ClassDB::bind_method(D_METHOD("copy_to", "texture"), &ExternalTexture::copy_to);
	ClassDB::bind_method(D_METHOD("copy_from", "texture"), &ExternalTexture::copy_from);

	ClassDB::bind_method(D_METHOD("get_fd"), &ExternalTexture::get_fd);
	ClassDB::bind_method(D_METHOD("get_rid"), &ExternalTexture::get_rid);
}

ExternalTexture::ExternalTexture() {
}

ExternalTexture::~ExternalTexture() {
	if (rid.is_valid()) {
		print_line("Texture freed " + itos(rid.get_id()));
		RD::get_singleton()->free(rid);
	}
}
