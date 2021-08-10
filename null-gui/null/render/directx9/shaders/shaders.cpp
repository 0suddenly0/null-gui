#include "shaders.h"

namespace null {
    namespace render {
        namespace shaders {
            shader_control::shader_control(const BYTE* shader_source) {
                init(shader_source);
            }

            void shader_control::init(const BYTE* shader_source) {
                switch(type) {
                case shader_type::pixel: if(!pixel_shader) directx9::device->CreatePixelShader(reinterpret_cast<const DWORD*>(shader_source), &pixel_shader); break;
                case shader_type::vertex: if(!vertex_shader) directx9::device->CreateVertexShader(reinterpret_cast<const DWORD*>(shader_source), &vertex_shader); break;
                }
            }

            void shader_control::clear() {
                if(pixel_shader) { pixel_shader->Release(); pixel_shader = nullptr; }
                if(vertex_shader) { vertex_shader->Release(); vertex_shader = nullptr; }
            }

            void shader_control::set_shader() {
                switch(type) {
                case shader_type::pixel: directx9::device->SetPixelShader(pixel_shader); break;
                case shader_type::vertex: directx9::device->SetVertexShader(vertex_shader); break;
                }
            }

            void shader_control::set_constant_f(const float* params, float location, int count) {
                switch(type) {
                case shader_type::pixel: directx9::device->SetPixelShaderConstantF(location, params, count); break;
                case shader_type::vertex: directx9::device->SetVertexShaderConstantF(location, params, count); break;
                }
            }

            void shader_control::set_constant_f(float* params, float location, int count) {
                switch(type) {
                case shader_type::pixel: directx9::device->SetPixelShaderConstantF(location, params, count); break;
                case shader_type::vertex: directx9::device->SetVertexShaderConstantF(location, params, count); break;
                }
            }

            void shader_control::set_constant_i(const int* params, float location, int count) {
                switch(type) {
                case shader_type::pixel: directx9::device->SetPixelShaderConstantI(location, params, count); break;
                case shader_type::vertex:directx9::device->SetVertexShaderConstantI(location, params, count); break;
                }
            }

            void shader_control::set_constant_i(int* params, float location, int count) {
                switch(type) {
                case shader_type::pixel: directx9::device->SetPixelShaderConstantI(location, params, count); break;
                case shader_type::vertex: directx9::device->SetVertexShaderConstantI(location, params, count); break;
                }
            }

            void shader_control::set_constant_b(const int* params, float location, int count) {
                switch(type) {
                case shader_type::pixel: directx9::device->SetPixelShaderConstantB(location, params, count); break;
                case shader_type::vertex: directx9::device->SetVertexShaderConstantB(location, params, count); break;
                }
            }

            void shader_control::set_constant_b(int* params, float location, int count) {
                switch(type) {
                case shader_type::pixel: directx9::device->SetPixelShaderConstantB(location, params, count); break;
                case shader_type::vertex: directx9::device->SetVertexShaderConstantB(location, params, count); break;
                }
            }

            IDirect3DTexture9* create_texture(vec2 tex_size) {
                IDirect3DTexture9* texture;
                directx9::device->CreateTexture(tex_size.x, tex_size.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &texture, nullptr);
                return texture;
            }

            void back_buffer_to_texture(IDirect3DTexture9* texture, rect backbuffer_region, D3DTEXTUREFILTERTYPE filtering) {
                if(!texture) return;
                if(backbuffer_region.size().x == 0.f || backbuffer_region.size().y == 0.f) backbuffer_region = rect(vec2(0.f, 0.f), display_size);

                IDirect3DSurface9* back_buffer = nullptr;
                IDirect3DSurface9* surface = nullptr;
                if(directx9::device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back_buffer) == D3D_OK) {
                    if(texture->GetSurfaceLevel(0, &surface) == D3D_OK) {
                        RECT backbuffer_rect = RECT{ (LONG)backbuffer_region.min.x, (LONG)backbuffer_region.min.y, (LONG)backbuffer_region.max.x, (LONG)backbuffer_region.max.y };
                        directx9::device->StretchRect(back_buffer, &backbuffer_rect, surface, nullptr, filtering);
                    }
                }

                if(back_buffer) { back_buffer->Release(); back_buffer = nullptr; }
                if(surface) { surface->Release(); surface = nullptr; }
            }

            void set_render_target(IDirect3DTexture9* texture) {
                IDirect3DSurface9* surface = nullptr;
                if(texture->GetSurfaceLevel(0, &surface) == D3D_OK)
                    directx9::device->SetRenderTarget(0, surface);

                if(surface) { surface->Release(); surface = nullptr; }
            }

            void begin_frame() {
                blur::clear_all_shaders();
            }

            void setup_render_state(detail::draw_data* _draw_data) {
                vertex::shader.set_shader();

                float L = _draw_data->display_pos.x + 0.5f;
                float R = _draw_data->display_pos.x + _draw_data->display_size.x + 0.5f;
                float T = _draw_data->display_pos.y + 0.5f;
                float B = _draw_data->display_pos.y + _draw_data->display_size.y + 0.5f;
                D3DMATRIX mat_projection = { { {
                    2.0f / (R - L),   0.0f,         0.0f,  0.0f,
                    0.0f,         2.0f / (T - B),   0.0f,  0.0f,
                    0.0f,         0.0f,         0.5f,  0.0f,
                    (L + R) / (L - R),  (T + B) / (B - T),  0.5f,  1.0f
                } } };
                vertex::shader.set_constant_f(&mat_projection.m[0][0], 0, 4);
            }

            void create_shaders() {
                vertex::create_shader_control();

                blur::create_shader_control();
                blur::clear_all_shaders();
            }

            void clear_shaders() {
                vertex::clear_shader_control();

                blur::clear_all_shaders();
                blur::clear_shader_control();
            }
        }
    }
}