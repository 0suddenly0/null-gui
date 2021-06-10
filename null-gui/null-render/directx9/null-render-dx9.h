#pragma once
#pragma comment (lib,"d3d9.lib")
#include <d3d9.h>

#include "../null-render.h"

namespace null_render {
	namespace directx9 {
		inline IDirect3DDevice9* device;
		inline IDirect3DVertexBuffer9* vertex_buffer;
		inline IDirect3DIndexBuffer9* index_buffer;
		inline IDirect3DTexture9* font_texture;
		inline IDirect3DVertexDeclaration9* vertex_declaration;

		bool initialize(IDirect3DDevice9* _device);
		void begin_frame();
		void shutdown();
		
		void render_draw_data(helpers::draw_data* _draw_data = &draw_data);
		void setup_render_state(helpers::draw_data* _draw_data);
		bool create_fonts_texture();
		bool create_device_objects();
		void invalidate_device_objects();
	}
}