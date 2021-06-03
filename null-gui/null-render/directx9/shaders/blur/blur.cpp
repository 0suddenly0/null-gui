#include "../shaders.h"

namespace null_render {
    namespace shaders {
        namespace blur {
            void shader::clear() {
                if (becup_render_targer) { becup_render_targer->Release(); becup_render_targer = nullptr; }
                if (first_texture) { first_texture->Release();  first_texture = nullptr; }
                if (second_texture) { second_texture->Release(); second_texture = nullptr; }
            }

            void shader::draw() {
                create_textures();

                if (!first_texture || !second_texture || !shader_x || !shader_y)
                    return;

                shader_draw_list->add_callback([=](helpers::cmd* cmd) { begin_draw(); });
                for (int i = 0; i < 8; i++) {
                    shader_draw_list->add_callback([=](helpers::cmd* cmd) { use_x_shader(); });
                    shader_draw_list->draw_image(reinterpret_cast<void*>(first_texture), { -1.0f, -1.0f }, { 1.0f, 1.0f });
                    shader_draw_list->add_callback([=](helpers::cmd* cmd) { use_y_shader(); });
                    shader_draw_list->draw_image(reinterpret_cast<void*>(second_texture), { -1.0f, -1.0f }, { 1.0f, 1.0f });
                }
                shader_draw_list->add_callback([=](helpers::cmd* cmd) { end_draw(); });

                shader_draw_list->add_callback(nullptr, true);

                shader_draw_list->draw_image_rounded(first_texture, region.min, region.max, { 0.0f, 0.0f }, { 1.0f, 1.0f }, color(1.f, 1.f, 1.f, alpha), rounding);
            }

            void shader::create_textures() {
                if (!first_texture) directx9::device->CreateTexture(region.size().x / amount, region.size().y / amount, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &first_texture, nullptr);//first_texture = (region.size() / amount);
                if (!second_texture) directx9::device->CreateTexture(region.size().x / amount, region.size().y / amount, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &second_texture, nullptr);//second_texture = create_texture(region.size() / amount);
            }

            void shader::clear_textures() {
                if (first_texture) { first_texture->Release(); first_texture = nullptr; }
                if (second_texture) { second_texture->Release(); second_texture = nullptr; }
            }

            void shader::begin_draw() {
                directx9::device->GetRenderTarget(0, &becup_render_targer);

                back_buffer_to_texture(first_texture, region);

                directx9::device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
                directx9::device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
                directx9::device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

                const D3DMATRIX matrix{ {{
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    -1.0f / (region.size().x / amount), 1.0f / (region.size().x / amount), 0.0f, 1.0f
                }} };
                shaders::vertex::shader.set_constant_f(&matrix.m[0][0], 0, 4);
            }

            void shader::end_draw() {
                directx9::device->SetRenderTarget(0, becup_render_targer);

                becup_render_targer->Release();
                becup_render_targer = nullptr;

                directx9::device->SetPixelShader(nullptr);
                directx9::device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
            }

            void shader::use_x_shader() {
                shader_x.set_shader();

                const float param[4] = { 1.0f / (region.size().x / amount) };
                shader_x.set_constant_f(param, 0);

                set_render_target(second_texture);
            }

            void shader::use_y_shader() {
                shader_y.set_shader();

                const float param[4] = { 1.0f / (region.size().y / amount) };
                shader_y.set_constant_f(param, 0);

                set_render_target(first_texture);
            }

            shader* create_shader(draw_list* shader_draw_list, rect region, float amount, float alpha, float rounding) {
                shader_list.push_back(new shader(shader_draw_list, region, amount, alpha, rounding));
                return shader_list.back();
            }

            void clear_all_shaders() {
                for (shader* cur_shader : shader_list) {
                    cur_shader->clear();
                    delete cur_shader;
                }
                shader_list.clear();
            }

            void create_shader_control() {
                if (!shader_x) shader_x.init(shader_sources::blur_x);
                if (!shader_y) shader_y.init(shader_sources::blur_y);
            }

            void clear_shader_control() {
                if (shader_x) shader_x.clear();
                if (shader_y) shader_y.clear();
            }
        }
    }

    void draw_list::draw_blur(vec2 start, vec2 end, float amount, float alpha, float rounding) {
        shaders::blur::create_shader(this, rect(start, end), amount, alpha, rounding)->draw();
    }
}