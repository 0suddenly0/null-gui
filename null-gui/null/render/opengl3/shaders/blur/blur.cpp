#include "../shaders.h"

namespace null {
	namespace render {
		namespace shaders {
			namespace blur {
				shader::~shader() {
					if(u_varialbes["texture"]["first"]) {
						glDeleteTextures(1, &u_varialbes["texture"]["first"]);
						u_varialbes["texture"]["first"] = 0;
					}
					if(u_varialbes["texture"]["second"]) {
						glDeleteTextures(1, &u_varialbes["texture"]["second"]);
						u_varialbes["texture"]["second"] = 0;
					}

					if(u_varialbes["buffer"]["frame"]) {
						glDeleteFramebuffers(1, &u_varialbes["buffer"]["frame"]);
						u_varialbes["buffer"]["frame"] = 0;
					}
				}

				void shader::begin_draw() {
					glGetIntegerv(GL_TEXTURE_BINDING_2D, &i_varialbes["backup"]["texture"]);
					glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &i_varialbes["backup"]["fbo"]);
					glGetIntegerv(GL_CURRENT_PROGRAM, &i_varialbes["backup"]["program"]);

					if(!u_varialbes["buffer"]["frame"])
						glGenFramebuffers(1, &u_varialbes["buffer"]["frame"]);

					glViewport(0, 0, display_size.x / amount, display_size.y / amount);
					glDisable(GL_SCISSOR_TEST);

					glBindFramebuffer(GL_DRAW_FRAMEBUFFER, u_varialbes["buffer"]["frame"]);
					glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, u_varialbes["texture"]["first"], 0);
					glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, u_varialbes["texture"]["second"], 0);
					glReadBuffer(GL_BACK);
					glDrawBuffer(GL_COLOR_ATTACHMENT0);
					glBlitFramebuffer(0, 0, display_size.x, display_size.y, 0, 0, display_size.x / amount, display_size.y / amount, GL_COLOR_BUFFER_BIT, GL_LINEAR);

					glEnableVertexAttribArray(0);
					glEnableVertexAttribArray(1);

					glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(opengl3::vertex), (GLvoid*)offsetof(opengl3::vertex, pos));
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(opengl3::vertex), (GLvoid*)offsetof(opengl3::vertex, uv));
				}

				void shader::end_draw() {
					glBindFramebuffer(GL_DRAW_FRAMEBUFFER, i_varialbes["backup"]["fbo"]);
					glUseProgram(i_varialbes["backup"]["program"]);
					glBindTexture(GL_TEXTURE_2D, i_varialbes["backup"]["texture"]);
				}

				void shader::use_x_shader() {
					glUseProgram(shader_x.u_varialbes["handle"]["program"]);
					glUniform1f(0, 1.f / (display_size.x / amount));

					vec2 clamp = vec2(region.min.x + 1.f, region.max.x - 1.f) / display_size.x;
					float _float[2] = { clamp.x, clamp.y };
					glUniform1fv(2, 2, _float);

					glDrawBuffer(GL_COLOR_ATTACHMENT1);
					glUniform1i(1, 0);
				}

				void shader::use_y_shader() {
					glUseProgram(shader_y.u_varialbes["handle"]["program"]);
					glUniform1f(0, 1.f / (display_size.y / amount));

					vec2 clamp = vec2(region.min.y + 1.f, region.max.y - 1.f) / display_size.y - vec2(1.f, 1.f);
					float _float[2] = { clamp.x * -1.f, clamp.y * -1.f };
					glUniform1fv(2, 2, _float);

					glDrawBuffer(GL_COLOR_ATTACHMENT0);
					glUniform1i(1, 0);
				}

				void shader::draw() {
					create_textures();
					create_shader_control();

					if(!u_varialbes["texture"]["first"] || !u_varialbes["texture"]["second"] || !shader_y || !shader_x)
						return;

					shader_draw_list->add_callback([=](detail::cmd* cmd) { begin_draw(); });
					for(int i = 0; i < 1; i++) {
						shader_draw_list->add_callback([=](detail::cmd* cmd) { use_x_shader(); });
						shader_draw_list->draw_image(reinterpret_cast<void*>(u_varialbes["texture"]["first"]), vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f));
						shader_draw_list->add_callback([=](detail::cmd* cmd) { use_y_shader(); });
						shader_draw_list->draw_image(reinterpret_cast<void*>(u_varialbes["texture"]["second"]), vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f));
					} shader_draw_list->add_callback([=](detail::cmd* cmd) { end_draw(); });
					shader_draw_list->add_callback(nullptr, true);

					rect uv = rect(vec2(0.f, 1.f), vec2(0.f, 1.f)) - (region / display_size) * vec2(-1.f, 1.f);
					shader_draw_list->draw_image_rounded(reinterpret_cast<void*>(u_varialbes["texture"]["first"]), region.min, region.max, uv.min, uv.max, color(1.f, 1.f, 1.f, 1.f), rounding, rounding_corners);
				}

				void shader::create_textures() {
					if(!u_varialbes["texture"]["first"]) { u_varialbes["texture"]["first"] = create_texture(display_size / amount); }
					if(!u_varialbes["texture"]["second"]) { u_varialbes["texture"]["second"] = create_texture(display_size / amount); }
				}

				std::shared_ptr<shader> create_shader(draw_list* shader_draw_list, rect region, float amount, float alpha, float rounding, flags_list<corner_flags> rounding_corners) {
					shaders_list.push_back(std::make_shared<shader>(shader_draw_list, region, amount, alpha, rounding, rounding_corners));
					return shaders_list.back();
				}

				void clear_all_shaders() {
					shaders_list.clear();
				}

				void create_shader_control() {
					if(!shader_y) shader_y.compile(sources::blur_y, sources::passthrough);
					if(!shader_x) shader_x.compile(sources::blur_x, sources::passthrough);
				}

				void clear_shader_control() {
					if(shader_y) shader_y.clear();
					if(shader_x) shader_x.clear();
				}
			}
		}

		void draw_list::draw_blur(vec2 start, vec2 end, float amount, float alpha, float rounding, flags_list<corner_flags> rounding_corners) {
			shaders::blur::create_shader(this, rect(start, end), amount, alpha, rounding, rounding_corners)->draw();
		}
	}
}