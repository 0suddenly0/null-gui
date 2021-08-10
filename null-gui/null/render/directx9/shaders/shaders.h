#pragma once
#include "../directx9.h"

namespace null {
	namespace render {
		namespace shaders {
			namespace sources {
			#include "vertex/compiled/vertex.h"
			#include "blur/compiled/blur_x.h"
			#include "blur/compiled/blur_y.h"
			}

			enum class shader_type {
				pixel,
				vertex
			};

			class shader_control {
			public:
				shader_control(shader_type _type) : type(_type) {}
				shader_control(const BYTE* shader_source);
				void init(const BYTE* shader_source);
				void clear();

				void set_shader();

				void set_constant_f(const float* params, float location, int count = 1);
				void set_constant_f(float* params, float location, int vec_count = 1);
				void set_constant_i(const int* params, float location, int count = 1);
				void set_constant_i(int* params, float location, int count = 1);
				void set_constant_b(const int* params, float location, int count = 1);
				void set_constant_b(int* params, float location, int count = 1);

				operator IDirect3DVertexShader9* () { return vertex_shader; }
				operator IDirect3DPixelShader9* () { return pixel_shader; }
				operator bool() { return type == shader_type::pixel ? pixel_shader != nullptr : vertex_shader != nullptr; }
				bool operator!() { return type == shader_type::pixel ? pixel_shader == nullptr : vertex_shader == nullptr; }

				shader_type type;

				IDirect3DVertexShader9* vertex_shader = nullptr;
				IDirect3DPixelShader9* pixel_shader = nullptr;
			};

			IDirect3DTexture9* create_texture(vec2 tex_size);
			void back_buffer_to_texture(IDirect3DTexture9* texture, rect backbuffer_region, D3DTEXTUREFILTERTYPE filtering = D3DTEXF_LINEAR);
			void set_render_target(IDirect3DTexture9* texture);

			void begin_frame();
			void setup_render_state(detail::draw_data* _draw_data);
			void create_shaders();
			void clear_shaders();

			namespace vertex {
				inline shader_control shader(shader_type::vertex);

				void create_shader_control();
				void clear_shader_control();
			}

			namespace blur {
				inline shader_control shader_x(shader_type::pixel);
				inline shader_control shader_y(shader_type::pixel);

				class shader {
				public:
					shader() = default;
					shader(draw_list* _shader_draw_list, rect _region, float _amount, float _alpha, float _rounding, flags_list<corner_flags> _rounding_corners) : shader_draw_list(_shader_draw_list), region(_region), amount(_amount), alpha(_alpha), rounding(_rounding), rounding_corners(_rounding_corners) {};
					~shader() { clear(); }
					void clear();

					void draw();

					void create_textures();
					void clear_textures();

					float alpha = 1.f;
					float amount = 1.f;
					float rounding = 0.f;
					rect region;
					flags_list<corner_flags> rounding_corners;

				private:
					void begin_draw();
					void end_draw();

					void use_x_shader();
					void use_y_shader();

					IDirect3DTexture9* first_texture = nullptr;
					IDirect3DTexture9* second_texture = nullptr;

					IDirect3DSurface9* becup_render_targer = nullptr;

					draw_list* shader_draw_list;
				};

				inline std::vector<std::shared_ptr<shader>> shader_list;
				std::shared_ptr<shader> create_shader(draw_list* shader_draw_list, rect region, float amount, float alpha, float rounding, flags_list<corner_flags> rounding_corners);
				void clear_all_shaders();

				void create_shader_control();
				void clear_shader_control();
			}
		}
	}
}