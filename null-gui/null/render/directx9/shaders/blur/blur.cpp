#include "../shaders.h"

namespace null {
    namespace render {
        namespace shaders {
            namespace blur {
                void shader::overlay_texture() {
                    shader_draw_list->add_callback([=](detail::cmd* cmd) { begin_overlay(); set_shader(); });
                    for (int i = 0; i < 8; i++) {
                        shader_draw_list->add_callback([=](detail::cmd* cmd) { vertical_pass(); });
                        shader_draw_list->draw_image(texture, vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f));
                        shader_draw_list->add_callback([=](detail::cmd* cmd) { horizontal_pass(); });
                        shader_draw_list->draw_image(second_texture, vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f));
                    }
                    shader_draw_list->add_callback([=](detail::cmd* cmd) { end_overlay(); });
                    shader_draw_list->add_callback(nullptr, true);
                }

                void shader::set_shader() {
                    blur_shader.set_shader();

                    vec2 size = directx9::get_texture_size(texture);
                    blur_shader.set_constant_f(std::vector<float>{ 1.0f / size.x, 1.0f / size.y }.data(), 0);
                    blur_shader.set_constant_f(std::vector<float>{ amount }.data(), 2);
                }

                void shader::vertical_pass() {
                    blur_shader.set_constant_f(std::vector<float>{ 1.0f, 0.0f }.data(), 1);

                    directx9::set_render_target(second_texture);
                }

                void shader::horizontal_pass() {
                    blur_shader.set_constant_f(std::vector<float>{ 0.0f, 1.0f }.data(), 1);

                    directx9::set_render_target(texture);
                }

                std::shared_ptr<shader> create_shader(draw_list* shader_draw_list, float amount) {
                    shader_list.push_back(std::make_shared<shader>(shader_draw_list, amount));
                    return shader_list.back();
                }
            }
        }

        void draw_list::draw_blur(vec2 start, vec2 end, float amount, float alpha, float rounding, flags_list<corner_flags> rounding_corners) {
            std::shared_ptr<shaders::blur::shader> blur = shaders::blur::create_shader(this, amount);
            blur->add_callback(shaders::shader_callbacks::create_textures, [=](shaders::base_shader* shader) { add_callback([=](detail::cmd*) { directx9::backbuffer_to_texture(shader->texture, rect(start, end)); }); });

            blur->create_textures(end - start);
            blur->overlay_texture();

            draw_image_rounded(blur->texture, start, end, vec2(0.f, 0.f), vec2(1.f, 1.f), color(1.f, 1.f, 1.f, alpha), rounding, rounding_corners);
        }

        void* draw_list::overlay_blur(void* texture, float amount) {
            std::shared_ptr<shaders::blur::shader> blur = shaders::blur::create_shader(this, amount);
            blur->add_callback(shaders::shader_callbacks::create_textures, [=](shaders::base_shader* shader) { directx9::copy_texture((IDirect3DTexture9*)texture, shader->texture); });

            blur->create_textures(directx9::get_texture_size((IDirect3DTexture9*)texture));
            blur->overlay_texture();
            return blur->texture;
        }
    }
}