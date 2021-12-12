#pragma once
#pragma comment (lib,"d3d9.lib")
#include <d3d9.h>

#include "../render.h"

namespace null {
	namespace render {
		namespace directx9 {
			struct vertex {
				float    pos[3];
				D3DCOLOR clr;
				float    uv[2];
			};

			inline IDirect3DDevice9* device;
			inline IDirect3DVertexBuffer9* vertex_buffer;
			inline IDirect3DIndexBuffer9* index_buffer;
			inline IDirect3DTexture9* font_texture;
			inline IDirect3DVertexDeclaration9* vertex_declaration;

			bool initialize(IDirect3DDevice9* _device);
			void begin_frame();
			void shutdown();

			void render_draw_data(detail::draw_data* _draw_data = &draw_data);
			void setup_render_state(detail::draw_data* _draw_data);
			bool create_fonts_texture();
			bool create_device_objects();
			void invalidate_device_objects();

			static void create_texture(IDirect3DTexture9** source, vec2 size, int levels = 1, UINT usage = D3DUSAGE_RENDERTARGET, D3DFORMAT format = D3DFMT_X8R8G8B8, D3DPOOL pool = D3DPOOL_DEFAULT) { device->CreateTexture(size.x, size.y, levels, usage, format, pool, source, nullptr); }
			static void set_render_target(IDirect3DTexture9* texture) { IDirect3DSurface9* surface = nullptr; if (texture->GetSurfaceLevel(0, &surface) == D3D_OK) device->SetRenderTarget(0, surface); if (surface) { surface->Release(); surface = nullptr; } }
			static vec2 get_texture_size(IDirect3DTexture9* texture) { D3DSURFACE_DESC desc; texture->GetLevelDesc(0, &desc); return vec2(desc.Width, desc.Height); }
			void copy_texture(IDirect3DTexture9* source, IDirect3DTexture9* dest, rect source_region = rect(0.f, 0.f, 0.f, 0.f), rect dest_region = rect(0.f, 0.f, 0.f, 0.f));
			void backbuffer_to_texture(IDirect3DTexture9* texture, rect backbuffer_region, D3DTEXTUREFILTERTYPE filtering = D3DTEXF_LINEAR);
		}
	}
}