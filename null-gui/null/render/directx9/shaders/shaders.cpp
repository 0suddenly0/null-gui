#include "shaders.h"

namespace null {
    namespace render {
        namespace shaders {
            void begin_frame() {
                blur::clear_all_shaders();
            }

            void setup_render_state(detail::draw_data* _draw_data) {
                vertex::vertex_shader.set_shader();

                float left = _draw_data->display_pos.x + 0.5f;
                float right = _draw_data->display_pos.x + _draw_data->display_size.x + 0.5f;
                float top = _draw_data->display_pos.y + 0.5f;
                float bottom = _draw_data->display_pos.y + _draw_data->display_size.y + 0.5f;
                D3DMATRIX matrix = {{{
                    2.0f / (right - left),           0.0f,                            0.0f, 0.0f,
                    0.0f,                            2.0f / (top - bottom),           0.0f, 0.0f,
                    0.0f,                            0.0f,                            0.5f, 0.0f,
                    (left + right) / (left - right), (top + bottom) / (bottom - top), 0.5f, 1.0f
                }}}; vertex::set_matrix(matrix);
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

            void base_shader::begin_overlay() {
                call_callbacks(shader_callbacks::begin_overlay);

                directx9::device->GetRenderTarget(0, &render_target);

                directx9::device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
                directx9::device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
                directx9::device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

                vertex::set_matrix(get_matrix(directx9::get_texture_size(texture)));
            }

            void base_shader::end_overlay() {
                directx9::device->SetRenderTarget(0, render_target);

                render_target->Release();
                render_target = nullptr;

                directx9::device->SetPixelShader(nullptr);
                directx9::device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);

                call_callbacks(shader_callbacks::end_overlay);
            }
        }
    }
}