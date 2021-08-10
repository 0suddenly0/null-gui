#include "shaders.h"

namespace null {
	namespace render {
		namespace shaders {
			void shader_control::compile(const char* fragment_source, const char* vertex_source) {
				if(!u_varialbes["handle"]["vertex"]) {
					u_varialbes["handle"]["vertex"] = glCreateShader(GL_VERTEX_SHADER);
					glShaderSource(u_varialbes["handle"]["vertex"], 1, &vertex_source, NULL);
					glCompileShader(u_varialbes["handle"]["vertex"]);
				}

				if(!u_varialbes["handle"]["fragment"]) {
					u_varialbes["handle"]["fragment"] = glCreateShader(GL_FRAGMENT_SHADER);
					glShaderSource(u_varialbes["handle"]["fragment"], 1, &fragment_source, NULL);
					glCompileShader(u_varialbes["handle"]["fragment"]);
				}

				if(!u_varialbes["handle"]["program"]) {
					u_varialbes["handle"]["program"] = glCreateProgram();
					glAttachShader(u_varialbes["handle"]["program"], u_varialbes["handle"]["vertex"]);
					glAttachShader(u_varialbes["handle"]["program"], u_varialbes["handle"]["fragment"]);
					glLinkProgram(u_varialbes["handle"]["program"]);
				}
			}

			void shader_control::clear() {
				if(u_varialbes["handle"]["program"] && u_varialbes["handle"]["vertex"]) { glDetachShader(u_varialbes["handle"]["program"], u_varialbes["handle"]["vertex"]); }
				if(u_varialbes["handle"]["program"] && u_varialbes["handle"]["fragment"]) { glDetachShader(u_varialbes["handle"]["program"], u_varialbes["handle"]["vertex"]); }
				if(u_varialbes["handle"]["program"]) { glDeleteProgram(u_varialbes["handle"]["program"]); u_varialbes["handle"]["program"] = 0; }

				if(u_varialbes["handle"]["vertex"]) { glDeleteShader(u_varialbes["handle"]["vertex"]); u_varialbes["handle"]["vertex"] = 0; }
				if(u_varialbes["handle"]["fragment"]) { glDeleteShader(u_varialbes["handle"]["fragment"]); u_varialbes["handle"]["fragment"] = 0; }
			}

			unsigned int create_texture(vec2 size) {
				int last_texture;
				glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

				glBindTexture(GL_TEXTURE_2D, last_texture);
				return texture;
			}

			void begin_frame() {
				blur::clear_all_shaders();
			}

			void setup_render_state(detail::draw_data* draw_data, unsigned int vertex_array_object) {
				general::setup_render_state(draw_data, vertex_array_object);
			}

			void create_shaders() {
				general::create_shader_control();

				blur::create_shader_control();
				blur::clear_all_shaders();
			}

			void clear_shaders() {
				general::clear_shader_control();

				blur::clear_shader_control();
				blur::clear_all_shaders();
			}
		}
	}
}