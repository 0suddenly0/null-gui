#include "../directx11.h"

namespace null {
	namespace render {
		namespace shaders {
			namespace sources {
			#include "vertex/compiled/vertex.h"
			#include "pixel/compiled/pixel.h"
			#include "blur/compiled/blur_x.h"
			#include "blur/compiled/blur_y.h"
			}

			class resources {
			public:
				void init();
				void clear();

				void set_render_target();
				void get_render_target();
				void clear_render_target();

				ID3D11Texture2D* texture = nullptr;
				ID3D11RenderTargetView* render_target = nullptr;
				ID3D11ShaderResourceView* shader_resource = nullptr;
			};

			enum class shader_type {
				pixel,
				vertex
			};

			class shader_control {
			public:
				shader_control(shader_type _type) : type(_type) {}
				shader_control(const void* shader_source, SIZE_T source_size, size_t constant_size);
				void init(const void* shader_source, size_t source_size, size_t constant_size = 0);
				void clear();

				void set_shader();
				void set_constant();

				template <typename constnt_type>
				void edit_constant(constnt_type constant_class) {
					D3D11_MAPPED_SUBRESOURCE mapped_resource;
					directx11::context->Map(constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
					constnt_type* buffer = (constnt_type*)mapped_resource.pData;
					*buffer = constant_class;
					directx11::context->Unmap(constant_buffer, 0);
				}

				operator bool() { return (type == shader_type::pixel ? pixel_shader != nullptr : vertex_shader != nullptr) && (constant_buffer != nullptr && !without_constant); }
				bool operator!() { return (type == shader_type::pixel ? pixel_shader == nullptr : vertex_shader == nullptr) || (constant_buffer == nullptr && !without_constant); }

				shader_type type;
				bool without_constant;

				ID3D11VertexShader* vertex_shader = nullptr;
				ID3D11PixelShader* pixel_shader = nullptr;

				ID3D11Buffer* constant_buffer = nullptr;
			};

			void back_buffer_call(std::function<void(ID3D11Texture2D*)> call);

			void begin_frame();
			void setup_render_state(detail::draw_data* _draw_data);
			void create_shaders();
			void clear_shaders();
			void win_proc();

			namespace pixel {
				inline shader_control shader(shader_type::pixel);

				void create_shader_control();
				void clear_shader_control();
			}

			namespace vertex {
				inline shader_control shader(shader_type::vertex);

				struct constant { float matrix[4][4]; };

				void create_shader_control();
				void clear_shader_control();
			}

			namespace blur {
				inline shader_control shader_x(shader_type::pixel);
				inline shader_control shader_y(shader_type::pixel);

				inline resources becup_layer;
				inline resources first_layer;
				inline resources second_layer;

				struct constant { float clamp[2]; float size; };

				class shader {
				public:
					shader() = default;
					shader(draw_list* _shader_draw_list, rect _region, float _amount, float _alpha, float _rounding, flags_list<corner_flags> _rounding_corners) : shader_draw_list(_shader_draw_list), region(_region), amount(_amount), alpha(_alpha), rounding(_rounding), rounding_corners(_rounding_corners) {};
					~shader() { clear(); }
					void clear();

					void draw();

					void create_layers();

					float alpha = 1.f;
					float amount = 1.f;
					float rounding = 0.f;
					rect region;
					flags_list<corner_flags> rounding_corners;

					draw_list* shader_draw_list;
				private:
					void begin_draw();
					void end_draw();

					void set_constants(vec2 size);

					void use_x_shader();
					void use_y_shader();
				};

				inline std::vector<std::shared_ptr<shader>> shader_list;
				std::shared_ptr<shader> create_shader(draw_list* shader_draw_list, rect region, float amount, float alpha, float rounding, flags_list<corner_flags> rounding_corners);
				void clear_layers();
				void clear_all_shaders();

				void create_shader_control();
				void clear_shader_control();
			}
		}
	}
}