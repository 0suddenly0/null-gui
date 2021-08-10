#pragma once
#include <map>
#include <string>
#include <functional>

#include "../opengl3.h"

namespace null {
	namespace render {
		namespace shaders {
			typedef std::map<std::string, std::map<std::string, unsigned int>> uint_map;
			typedef std::map<std::string, std::map<std::string, int>> int_map;

			namespace sources {
				static const char* blur_y = R"(
				#version 130
				#extension GL_ARB_explicit_attrib_location : require
				#extension GL_ARB_explicit_uniform_location : require

				in vec2 frag_uv;
				out vec4 color;

				layout(location = 0) uniform float texelHeight;
				layout(location = 1) uniform sampler2D texSampler;
				layout(location = 2) uniform float clamp_coordinats[2];

				float offsets[3] = float[](0.0f, 1.3846153846f, 3.2307692308f);
				float weights[3] = float[](0.2270270270f, 0.3162162162f, 0.0702702703f);

				void main() {
					color = texture(texSampler, frag_uv);
					color.rgb *= weights[0];
					for (int i = 1; i < 3; ++i) {        
						float upper = clamp(frag_uv.y - texelHeight * offsets[i], clamp_coordinats[1], clamp_coordinats[0]);
				        float bottom = clamp(frag_uv.y + texelHeight * offsets[i], clamp_coordinats[1], clamp_coordinats[0]);
				        color.rgb += texture(texSampler, vec2(frag_uv.x, upper)).rgb * weights[i];
						color.rgb += texture(texSampler, vec2(frag_uv.x, bottom)).rgb * weights[i];
					}
				}
				)";

				static const char* blur_x = R"(
				#version 130
				#extension GL_ARB_explicit_attrib_location : require
				#extension GL_ARB_explicit_uniform_location : require

				in vec2 frag_uv;
				out vec4 color;

				layout(location = 0) uniform float texelWidth;
				layout(location = 1) uniform sampler2D texSampler;
				layout(location = 2) uniform float clamp_coordinats[2];

				float offsets[3] = float[](0.0f, 1.3846153846f, 3.2307692308f);
				float weights[3] = float[](0.2270270270f, 0.3162162162f, 0.0702702703f);

				void main() {
				    color = texture(texSampler, frag_uv);
				    color.rgb *= weights[0];
				    for (int i = 1; i < 3; ++i) {
				        float left = clamp(frag_uv.x - texelWidth * offsets[i], clamp_coordinats[0], clamp_coordinats[1]);
				        float right = clamp(frag_uv.x + texelWidth * offsets[i], clamp_coordinats[0], clamp_coordinats[1]);
				        color.rgb += texture(texSampler, vec2(left, frag_uv.y)).rgb * weights[i];
				        color.rgb += texture(texSampler, vec2(right, frag_uv.y)).rgb * weights[i];
				    }
				}
				)";

				static const char* passthrough = R"(
				#version 130
				#extension GL_ARB_explicit_attrib_location : require

				layout(location = 0) in vec2 pos;
				layout(location = 1) in vec2 uv;

				out vec2 frag_uv;

				void main() {
				    frag_uv = uv;
				    gl_Position = vec4(pos.xy, 0, 1);
				}
				)";

				static const char* fragment = R"(
				#version 130
				
				uniform sampler2D sampler;
				in vec2 frag_uv;
				in vec4 frag_color;
				out vec4 out_color;
				
				void main() { 
					out_color = frag_color * texture(sampler, frag_uv.st);
				}
				)";

				static const char* vertex = R"(
				#version 130
				
				uniform mat4 proj_mtx;
				in vec2 position;
				in vec2 uv;
				in vec4 color;
				out vec2 frag_uv;
				out vec4 frag_color;
				
				void main() {
				    frag_uv = uv;
				    frag_color = color;
				    gl_Position = proj_mtx * vec4(position.xy,0,1);
				}
				)";
			}

			class shader_control {
			public:
				void compile(const char* fragment_source = sources::fragment, const char* vertex_source = sources::vertex);
				void clear();

				operator bool() { return u_varialbes["handle"]["program"] && u_varialbes["handle"]["vertex"] && u_varialbes["handle"]["fragment"]; }
				bool operator!() { return !u_varialbes["handle"]["program"] || !u_varialbes["handle"]["vertex"] || !u_varialbes["handle"]["fragment"]; }

				uint_map u_varialbes;
				int_map i_varialbes;
			};

			unsigned int create_texture(vec2 size);

			void begin_frame();
			void setup_render_state(detail::draw_data* draw_data, unsigned int vertex_array_object);
			void create_shaders();
			void clear_shaders();

			namespace blur {
				inline rect mins_maxes = rect(vec2(0.f, 1.f), vec2(1.f, 0.f));
				inline shader_control shader_y;
				inline shader_control shader_x;

				class shader {
				public:
					shader();
					shader(draw_list* _shader_draw_list, rect _region, float _amount, float _alpha, float _rounding, flags_list<corner_flags> _rounding_corners) :
						shader_draw_list(_shader_draw_list), region(_region), amount(_amount), alpha(_alpha), rounding(_rounding), rounding_corners(_rounding_corners) {}
					~shader();
					void begin_draw();
					void end_draw();

					void use_x_shader();
					void use_y_shader();

					void draw();

					void create_textures();

					draw_list* shader_draw_list;

					rect region;

					float amount;
					float alpha;
					float rounding;

					flags_list<corner_flags> rounding_corners;

					uint_map u_varialbes;
					int_map i_varialbes;
				};

				inline std::vector<std::shared_ptr<shader>> shaders_list;

				std::shared_ptr<shader> create_shader(draw_list* shader_draw_list, rect region, float amount, float alpha, float rounding, flags_list<corner_flags> rounding_corners);
				void clear_all_shaders();

				void create_shader_control();
				void clear_shader_control();
			}

			namespace general {
				inline shader_control shader;

				void setup_render_state(detail::draw_data* draw_data, unsigned int vertex_array_object);
				void create_shader_control();
				void clear_shader_control();
			}
		}
	}
}