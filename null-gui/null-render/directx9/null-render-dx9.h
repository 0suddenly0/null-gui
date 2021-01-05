#pragma once
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#include <d3d9.h>
#include "../null-render.h"

struct IDirect3DDevice9;

namespace null_render {
	namespace directx9 {
		bool init(IDirect3DDevice9* device);
		void begin();
		void shutdown();
		
		void render_draw_data(helpers::draw_data* _draw_data = &draw_data);
		void setup_render_state(helpers::draw_data* _draw_data);
		bool create_fonts_texture();
		bool create_device_objects();
		void invalidate_device_objects();
	}
}