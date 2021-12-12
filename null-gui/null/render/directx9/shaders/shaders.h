#pragma once
#include "../directx9.h"

namespace null {
	namespace render {
		namespace shaders {
			namespace sources {
			#include "vertex/compiled/vertex.h"
			#include "blur/compiled/blur.h"
			}

			template <typename shader_type>
			class base_shader_control {
			public:
				base_shader_control() = default;
				base_shader_control(const BYTE* shader_source) { initialize(shader_source); }
				virtual void initialize(const BYTE* shader_source) = 0;
				virtual void clear() { if (shader) { shader->Release(); shader = nullptr; } };

				virtual void set_shader() = 0;

				virtual void set_constant_f(float* params, float location, int count = 1) = 0;
				virtual void set_constant_i(int* params, float location, int count = 1) = 0;
				virtual void set_constant_b(int* params, float location, int count = 1) = 0;

				virtual operator shader_type* () { return shader; }
				virtual operator bool() { return shader != nullptr; }
				virtual bool operator!() { return shader == nullptr; }

				shader_type* shader = nullptr;
			};

			class vertex_shader_control : public base_shader_control<IDirect3DVertexShader9> {
			public:
				vertex_shader_control() = default;
				vertex_shader_control(const BYTE* shader_source) : base_shader_control(shader_source) {};
				void initialize(const BYTE* shader_source) override { if (!shader) directx9::device->CreateVertexShader(reinterpret_cast<const DWORD*>(shader_source), &shader); }

				void set_shader() override { directx9::device->SetVertexShader(shader); }

				void set_constant_f(float* params, float location, int count = 1) override { directx9::device->SetVertexShaderConstantF(location, params, count); }
				void set_constant_i(int* params, float location, int count = 1) override { directx9::device->SetVertexShaderConstantI(location, params, count); }
				void set_constant_b(int* params, float location, int count = 1) override { directx9::device->SetVertexShaderConstantB(location, params, count); }
			};

			class pixel_shader_control : public base_shader_control<IDirect3DPixelShader9> {
			public:
				pixel_shader_control() = default;
				pixel_shader_control(const BYTE* shader_source) : base_shader_control(shader_source) {};
				void initialize(const BYTE* shader_source) override { if (!shader) directx9::device->CreatePixelShader(reinterpret_cast<const DWORD*>(shader_source), &shader); }

				void set_shader() override { directx9::device->SetPixelShader(shader); }

				void set_constant_f(float* params, float location, int count = 1) override { directx9::device->SetPixelShaderConstantF(location, params, count); }
				void set_constant_i(int* params, float location, int count = 1) override { directx9::device->SetPixelShaderConstantI(location, params, count); }
				void set_constant_b(int* params, float location, int count = 1) override { directx9::device->SetPixelShaderConstantB(location, params, count); }
			};

			void begin_frame();
			void setup_render_state(detail::draw_data* _draw_data);
			void create_shaders();
			void clear_shaders();

			enum class shader_callbacks {
				create_textures,
				clear_textures,
				begin_overlay,
				end_overlay
			};

			class base_shader {
			public:
				base_shader() = default;
				base_shader(draw_list* _shader_draw_list) :
					shader_draw_list(_shader_draw_list) {
				};
				~base_shader() { clear(); }

				//standart fucntions
				virtual void clear() { if (render_target) { render_target->Release(); render_target = nullptr; } clear_textures(); }

				virtual void overlay_texture() {}

				virtual void create_textures(vec2 size) { if (!texture) directx9::create_texture(&texture, size); call_callbacks(shader_callbacks::create_textures); }
				virtual void clear_textures() { if (texture) { texture->Release(); texture = nullptr; } call_callbacks(shader_callbacks::clear_textures); }

				//callbacks for overlay shader on texture
				virtual void begin_overlay();
				virtual void end_overlay();

				void add_callback(shader_callbacks callback, std::function<void(base_shader*)> func) { callbacks[callback].push_back(func); }

				virtual D3DMATRIX get_matrix(vec2 size) {
					return D3DMATRIX{ {{
							1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							-1.0f / size.x, 1.0f / size.y, 0.0f, 1.0f
						}} };
				}

				draw_list* shader_draw_list = nullptr;

				IDirect3DTexture9* texture = nullptr;
				IDirect3DSurface9* render_target = nullptr;
			protected:
				void call_callbacks(shader_callbacks callback) { for (std::function<void(base_shader*)> func : callbacks[callback]) func(this); }
				std::map<shader_callbacks, std::vector<std::function<void(base_shader*)>>> callbacks;
			};

			namespace vertex {
				inline vertex_shader_control vertex_shader;

				static void set_matrix(D3DMATRIX matrix) { vertex_shader.set_constant_f(&matrix.m[0][0], 0, 4); }

				static void create_shader_control() { if (!vertex_shader) vertex_shader.initialize(sources::vertex); }
				static void clear_shader_control() { if (vertex_shader) vertex_shader.clear(); }
			}

			namespace blur {
				inline pixel_shader_control blur_shader;

				class shader : public base_shader {
				public:
					shader() : base_shader() {};
					shader(draw_list* _shader_draw_list, float _amount) :
						base_shader(_shader_draw_list), amount(_amount) {
						add_callback(shader_callbacks::create_textures, std::bind(&shader::create_second_texture, this, this));
						add_callback(shader_callbacks::clear_textures, std::bind(&shader::clear_second_textures, this, this));
					};

					//callbacks
					void create_second_texture(base_shader*) { if (!second_texture) directx9::create_texture(&second_texture, directx9::get_texture_size(texture)); }
					void clear_second_textures(base_shader*) { if (second_texture) { second_texture->Release(); second_texture = nullptr; } }

					void overlay_texture() override;

					void set_shader();

					void vertical_pass();
					void horizontal_pass();

					float amount = 1.f;

					IDirect3DTexture9* second_texture = nullptr;
				};

				inline std::vector<std::shared_ptr<shader>> shader_list;
				std::shared_ptr<shader> create_shader(draw_list* shader_draw_list, float amount);
				static void clear_all_shaders() { shader_list.clear(); }

				static void create_shader_control() { if (!blur_shader) blur_shader.initialize(sources::blur); }
				static void clear_shader_control() { if (blur_shader) blur_shader.clear(); }
			}
		}
	}
}