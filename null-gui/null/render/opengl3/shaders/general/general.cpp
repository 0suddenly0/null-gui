#include "../shaders.h"

namespace null {
	namespace render {
		namespace shaders {
			namespace general {
				void setup_render_state(detail::draw_data* draw_data, unsigned int vertex_array_object) {
					float L = draw_data->display_pos.x;
					float R = draw_data->display_pos.x + draw_data->display_size.x;
					float T = draw_data->display_pos.y;
					float B = draw_data->display_pos.y + draw_data->display_size.y;
					const float ortho_projection[4][4] =
					{
						{ 2.0f / (R - L),   0.0f,         0.0f,   0.0f },
						{ 0.0f,         2.0f / (T - B),   0.0f,   0.0f },
						{ 0.0f,         0.0f,        -1.0f,   0.0f },
						{ (R + L) / (L - R),  (T + B) / (B - T),  0.0f,   1.0f },
					};
					glUseProgram(shader.u_varialbes["handle"]["program"]);
					glUniform1i(shader.i_varialbes["location"]["sampler"], 0);
					glUniformMatrix4fv(shader.i_varialbes["location"]["proj_mtx"], 1, GL_FALSE, &ortho_projection[0][0]);

					glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.

					//(void)vertex_array_object;
					glBindVertexArray(vertex_array_object);

					// Bind vertex/index buffers and setup attributes for ImDrawVert
					glBindBuffer(GL_ARRAY_BUFFER, shader.u_varialbes["handle"]["vbo"]);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader.u_varialbes["handle"]["elements"]);
					glEnableVertexAttribArray(shader.u_varialbes["location"]["position"]);
					glEnableVertexAttribArray(shader.u_varialbes["location"]["uv"]);
					glEnableVertexAttribArray(shader.u_varialbes["location"]["color"]);
					glVertexAttribPointer(shader.u_varialbes["location"]["position"], 2, GL_FLOAT, GL_FALSE, sizeof(opengl3::vertex), (void*)offsetof(opengl3::vertex, pos));
					glVertexAttribPointer(shader.u_varialbes["location"]["uv"], 2, GL_FLOAT, GL_FALSE, sizeof(opengl3::vertex), (void*)offsetof(opengl3::vertex, uv));
					glVertexAttribPointer(shader.u_varialbes["location"]["color"], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(opengl3::vertex), (void*)offsetof(opengl3::vertex, clr));
				}

				void create_shader_control() {
					if(!shader) shader.compile();

					shader.i_varialbes["location"]["sampler"] = glGetUniformLocation(shader.u_varialbes["handle"]["program"], "sampler");
					shader.i_varialbes["location"]["proj_mtx"] = glGetUniformLocation(shader.u_varialbes["handle"]["program"], "proj_mtx");
					shader.u_varialbes["location"]["position"] = (unsigned int)glGetAttribLocation(shader.u_varialbes["handle"]["program"], "position");
					shader.u_varialbes["location"]["uv"] = (unsigned int)glGetAttribLocation(shader.u_varialbes["handle"]["program"], "uv");
					shader.u_varialbes["location"]["color"] = (unsigned int)glGetAttribLocation(shader.u_varialbes["handle"]["program"], "color");

					glGenBuffers(1, &shader.u_varialbes["handle"]["vbo"]);
					glGenBuffers(1, &shader.u_varialbes["handle"]["elements"]);
				}

				void clear_shader_control() {
					if(shader) shader.clear();

					if(shader.u_varialbes["handle"]["vbo"]) { glDeleteBuffers(1, &shader.u_varialbes["handle"]["vbo"]); shader.u_varialbes["handle"]["vbo"] = 0; }
					if(shader.u_varialbes["handle"]["elements"]) { glDeleteBuffers(1, &shader.u_varialbes["handle"]["elements"]); shader.u_varialbes["handle"]["elements"] = 0; }
				}
			}
		}
	}
}