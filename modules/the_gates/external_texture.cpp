#include "external_texture.h"

#ifdef _WIN32
#include "Windows.h"
#include "zmq_context.h"
#include "socket.hpp"
#include "message.hpp"
#else
#include "flingfd.h"
#endif

Error ExternalTexture::create(const RD::TextureFormat &p_format, const RD::TextureView &p_view, const Vector<Vector<uint8_t>> &p_data) {
	view = p_view;
	format = p_format;
	rid = RD::get_singleton()->external_texture_create(p_format, p_view, &filehandle, p_data);
	ERR_FAIL_COND_V_MSG(!rid.is_valid(), ERR_CANT_CREATE, "Unable to create external texture");

    return OK;
}

Error ExternalTexture::import(const RD::TextureFormat &p_format, const RD::TextureView &p_view) {
	ERR_FAIL_COND_V_MSG(filehandle == FileHandleInvalid, ERR_UNAVAILABLE, "filehandle is no valid. Receive filehandle first");

	view = p_view;
	format = p_format;
	rid = RD::get_singleton()->external_texture_import(format, view, filehandle);
	ERR_FAIL_COND_V_MSG(!rid.is_valid(), ERR_CANT_CREATE, "Unable to import external texture from filehandle");

    return OK;
}

bool ExternalTexture::send_filehandle(const String &p_path) {
	ERR_FAIL_COND_V_MSG(filehandle == FileHandleInvalid, false, "Sending invalid filehandle. First create external texture");

#ifdef _WIN32
	// Duplicate handle
	PackedStringArray split = p_path.split("|");
	ERR_FAIL_COND_V_MSG(split.size() != 2, false, "Invalid path. Should be 'ipc://path|targetProcessId'");
	int targetProcessId = split[1].to_int();

	HANDLE hTargetProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, targetProcessId);
	ERR_FAIL_COND_V_MSG(hTargetProcess == INVALID_HANDLE_VALUE, false, "Unable to OpenProcess from " + itos(targetProcessId));

	HANDLE hDuplicateHandle;
	bool success = DuplicateHandle(GetCurrentProcess(), (HANDLE)filehandle, hTargetProcess, &hDuplicateHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
	ERR_FAIL_COND_V_MSG(!success, false, "Unable to DuplicateHandle. Error code: " + itos(GetLastError()));

	printf("DuplicateHandle: %p\n", hDuplicateHandle);

	// Send handle
	zmqpp::socket sock(zmqpp::socket(ctx, zmqpp::socket_type::pair));
	sock.connect(split[0].utf8().get_data());

	filehandle = (FileHandle)hDuplicateHandle;
	printf("filehandle: %p\n", filehandle);
	zmqpp::message msg;
	int64_t data = reinterpret_cast<int64_t>(filehandle);
	msg << data;
	success = sock.send(msg, true);
	sock.close();

	// Clean up
	CloseHandle(hTargetProcess);
	if (!success) {
		CloseHandle(hDuplicateHandle);
	}

	return success;
#else
	return flingfd_simple_send(p_path.utf8().get_data(), filehandle);
#endif
}

bool ExternalTexture::recv_filehandle(const String &p_path) {
#ifdef _WIN32
	zmqpp::socket sock(zmqpp::socket(ctx, zmqpp::socket_type::pair));
	sock.bind(p_path.utf8().get_data());

	size_t size = sizeof(FileHandle);
	zmqpp::message msg;
	sock.receive(msg, false); // WARNING: BLOCKING COMMAND
	int64_t data;
	msg >> data;
	filehandle = reinterpret_cast<void*>(data);
	sock.close();
#else
	filehandle = flingfd_simple_recv(p_path.utf8().get_data());
#endif
	ERR_FAIL_COND_V_MSG(filehandle == FileHandleInvalid, false, "Recieve filehandle failed");
	printf("Received FileHandle: %p\n", filehandle);

	return true;
}

Error ExternalTexture::_copy(RID p_texture, bool p_from) {
	ERR_FAIL_COND_V_MSG(!rid.is_valid(), ERR_UNAVAILABLE, "ExternalTexture is not valid. Create or import first");
	ERR_FAIL_COND_V_MSG(!p_texture.is_valid(), ERR_INVALID_PARAMETER, "Parameter texture is not valid");

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

Error ExternalTexture::_import(const Ref<RDTextureFormat> &p_format, const Ref<RDTextureView> &p_view) {
	ERR_FAIL_COND_V(p_format.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(p_view.is_null(), ERR_INVALID_PARAMETER);

	return import(RD::_get_base(p_format), RD::_get_base(p_view));
}

void ExternalTexture::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create", "format", "view"), &ExternalTexture::_create, DEFVAL(Array()));
	ClassDB::bind_method(D_METHOD("send_filehandle", "path"), &ExternalTexture::send_filehandle);
	ClassDB::bind_method(D_METHOD("copy_to", "texture"), &ExternalTexture::copy_to);
	ClassDB::bind_method(D_METHOD("copy_from", "texture"), &ExternalTexture::copy_from);

	// ClassDB::bind_method(D_METHOD("get_filehandle"), &ExternalTexture::get_filehandle);
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
