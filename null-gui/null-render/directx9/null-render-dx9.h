#pragma once
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#include <d3d9.h>

#include "../null-render.h"

struct IDirect3DDevice9;

namespace null_render {
	namespace shaders {
		namespace shader_sources {
#include "shaders/blur/compiled/test.h"
#include "shaders/blur/compiled/blur_x.h"
#include "shaders/blur/compiled/blur_y.h"
#include "shaders/vertex/compiled/vertex.h"
		}

		class shader_control {
		public:
			shader_control() = default;
			shader_control(const BYTE* shader_source);
			void init(const BYTE* shader_source);
			void use(float uniform, float location);
			void clear();

			operator bool() { return pixel_shader != nullptr; }
			bool operator!() { return pixel_shader == nullptr; }

			IDirect3DPixelShader9* pixel_shader = nullptr;
		};

		IDirect3DTexture9* create_texture(vec2 tex_size);
		void backbuffer_to_texture(IDirect3DTexture9* texture, rect backbuffer_region, D3DTEXTUREFILTERTYPE filtering = D3DTEXF_LINEAR);
		void set_render_target(IDirect3DTexture9* texture);

		namespace blur {
			shader_control shader_x;
			shader_control shader_y;

			class shader {
			public:
				shader() = default;
				shader(draw_list* _shader_draw_list, rect _region, float _amount, float _alpha, float _rounding) : shader_draw_list(_shader_draw_list), region(_region), amount(_amount), alpha(_alpha), rounding(_rounding) { };
				void clear();

				void draw();

				void create_textures();
				void clear_textures();

				float alpha = 1.f;
				float amount = 1.f;
				float rounding = 0.f;
				rect region;

			private:
				void begin_draw();
				void end_draw();

				void use_x_shader();
				void use_y_shader();

				IDirect3DTexture9* first_texture;
				IDirect3DTexture9* second_texture;

				IDirect3DSurface9* becup_render_targer;

				draw_list* shader_draw_list;
			};

			std::vector<shader*> shader_list;
			shader* create_shader(draw_list* shader_draw_list, rect region, float amount, float alpha, float rounding);
			void clear_all_shaders();

			void create_shader_control();
			void clear_shader_control();
		}
	}

	namespace directx9 {
		IDirect3DDevice9* device;
		IDirect3DVertexBuffer9* vertex_buffer;
		IDirect3DIndexBuffer9* index_buffer;
		IDirect3DTexture9* font_texture;
		IDirect3DVertexDeclaration9* vertex_declaration;
		IDirect3DVertexShader9* vertex_shader;

		bool init(IDirect3DDevice9* _device);
		void begin();
		void shutdown();
		
		void render_draw_data(helpers::draw_data* _draw_data = &draw_data);
		void setup_render_state(helpers::draw_data* _draw_data);
		bool create_fonts_texture();
		bool create_device_objects();
		void invalidate_device_objects();
	}
}