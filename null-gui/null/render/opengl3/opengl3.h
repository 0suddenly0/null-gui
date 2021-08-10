#pragma once

#include "../render.h"
#include "gl3w/gl3w.h"

namespace null {
	namespace render {
		namespace opengl3 {
			struct vertex {
				vec2 pos;
				vec2 uv;
				unsigned int clr;
			};

			inline unsigned int font_texture = 0;

			void initialize();
			void shutdown();
			void begin_frame();

			void render_draw_data(detail::draw_data* _draw_data = &draw_data);
			void setup_render_state(detail::draw_data* _draw_data, int fb_width, int fb_height, GLuint vertex_array_object);
			bool create_fonts_texture();
			void destroy_fonts_texture();
			bool create_device_objects();
			void destroy_device_objects();
		}
	}
}