#include "../shaders.h"

namespace null_render {
    namespace shaders {
        namespace blur {
            void shader::clear() {
                becup_layer.clear();
                first_layer.clear();
                second_layer.clear();
            }

            void shader::draw() {
                create_layers();

                if (!shader_x || !shader_y)
                    return;

                rect calc_uv = region / settings::display_size;

                shader_draw_list->add_callback([=](helpers::cmd* cmd) { begin_draw(); });
                for (int i = 0; i < 8; i++) {
                    shader_draw_list->add_callback([=](helpers::cmd* cmd) { use_x_shader(); });
                    shader_draw_list->draw_image(first_layer.shader_resource, region.min, region.max, calc_uv.min, calc_uv.max);

                    shader_draw_list->add_callback([=](helpers::cmd* cmd) { use_y_shader(); });
                    shader_draw_list->draw_image(second_layer.shader_resource, region.min, region.max, calc_uv.min, calc_uv.max);
                }
                shader_draw_list->add_callback([=](helpers::cmd* cmd) { end_draw(); });
                shader_draw_list->add_callback(nullptr, true);

                shader_draw_list->draw_image_rounded(first_layer.shader_resource, region.min, region.max, calc_uv.min, calc_uv.max, color(1.f, 1.f, 1.f, alpha), rounding);
            }

            void shader::create_layers() {
                first_layer.init();
                second_layer.init();
            }

            void shader::begin_draw() {
                becup_layer.get_render_target();

                first_layer.clear_render_target();
                second_layer.clear_render_target();

                back_buffer_call([=](ID3D11Texture2D* back_buffer) {
                    directx11::context->CopyResource(first_layer.texture, back_buffer);
                    directx11::context->CopyResource(second_layer.texture, back_buffer);
                    });

                //copy_backbuffer(&first_layer);
                //copy_backbuffer(&second_layer);

                set_constants(settings::display_size / amount);
            }

            void shader::end_draw() {
                becup_layer.set_render_target();
                becup_layer.clear();
            }

            void shader::set_constants(vec2 size) {
                vec2 clamp_x = vec2(region.min.x + 1.f, region.max.x - 1.f) / settings::display_size.x;
                vec2 clamp_y = vec2(region.min.y + 1.f, region.max.y - 1.f) / settings::display_size.y;
                shader_x.edit_constant(constant{ { clamp_x.x, clamp_x.y }, 1.f / size.x });
                shader_y.edit_constant(constant{ { clamp_y.x, clamp_y.y }, 1.f / size.y });
            }

            void shader::use_x_shader() {
                second_layer.set_render_target();
                shader_x.set_shader();
                shader_x.set_constant();
            }

            void shader::use_y_shader() {
                first_layer.set_render_target();
                shader_y.set_shader();
                shader_y.set_constant();
            }

            shader* create_shader(draw_list* shader_draw_list, rect region, float amount, float alpha, float rounding) {
                shader_list.push_back(new shader(shader_draw_list, region, amount, alpha, rounding));
                return shader_list.back();
            }

            void clear_layers() {
                becup_layer.clear();
                first_layer.clear();
                second_layer.clear();
            }

            void clear_all_shaders() {
                shader_list.clear();
            }

            void create_shader_control() {
                if (!shader_x) shader_x.init(shader_sources::blur_x, sizeof(shader_sources::blur_x), sizeof(constant));
                if (!shader_y) shader_y.init(shader_sources::blur_y, sizeof(shader_sources::blur_y), sizeof(constant));
            }

            void clear_shader_control() {
                if (shader_x) { shader_x.clear(); }
                if (shader_y) { shader_y.clear(); }
            }
        }
    }

    void draw_list::draw_blur(vec2 start, vec2 end, float amount, float alpha, float rounding) {
        shaders::blur::create_shader(this, rect(start, end), amount, alpha, rounding)->draw();
    }
}