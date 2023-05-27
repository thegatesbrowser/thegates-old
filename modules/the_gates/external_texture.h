#ifndef EXTERNAL_TEXTURE_H
#define EXTERNAL_TEXTURE_H

#include "core/object/ref_counted.h"
#include "servers/rendering/rendering_device.h"
#include "servers/rendering/rendering_device_binds.h"

class ExternalTexture : public RefCounted {
	GDCLASS(ExternalTexture, RefCounted);

    FileHandle filehandle = FileHandleInvalid;
    RID rid;
    RD::TextureView view;
    RD::TextureFormat format;

    Error _copy(RID p_texture, bool p_from);

protected:
	static void _bind_methods();
	Error _create(const Ref<RDTextureFormat> &p_format, const Ref<RDTextureView> &p_view, const TypedArray<PackedByteArray> &p_data);
	Error _import(const Ref<RDTextureFormat> &p_format, const Ref<RDTextureView> &p_view);

public:
    Error create(const RD::TextureFormat &p_format, const RD::TextureView &p_view, const Vector<Vector<uint8_t>> &p_data = Vector<Vector<uint8_t>>());
	Error import(const RD::TextureFormat &p_format, const RD::TextureView &p_view);
	bool send_filehandle(const String &p_path);
	bool recv_filehandle(const String &p_path);

    Error copy_to(RID p_texture) { return _copy(p_texture, false); };
    Error copy_from(RID p_texture) { return _copy(p_texture, true); };

	// FileHandle get_filehandle() const { return filehandle; };
	RID get_rid() const { return rid; };

	ExternalTexture();
	~ExternalTexture();
};

#endif // EXTERNAL_TEXTURE_H
