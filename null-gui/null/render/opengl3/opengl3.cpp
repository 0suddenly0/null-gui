#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>

#include "opengl3.h"
#include "shaders/shaders.h"

namespace null {
    namespace render {
        namespace opengl3 {
            void initialize() {
                renderer_has_vtx_offset = true;
            }

            void shutdown() {
                destroy_device_objects();
            }

            void begin_frame() {
                shaders::begin_frame();

                if(!shaders::general::shader)
                    create_device_objects();
            }

            void render_draw_data(detail::draw_data* draw_data) {
                int fb_width = (int)(draw_data->display_size.x);
                int fb_height = (int)(draw_data->display_size.y);
                if(fb_width <= 0 || fb_height <= 0)
                    return;

                GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
                glActiveTexture(GL_TEXTURE0);
                GLuint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&last_program);
                GLuint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&last_texture);
                GLuint last_sampler; glGetIntegerv(GL_SAMPLER_BINDING, (GLint*)&last_sampler);
                GLuint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&last_array_buffer);
                GLuint last_vertex_array_object; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&last_vertex_array_object);
            #ifdef GL_POLYGON_MODE
                GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
            #endif
                GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
                GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
                GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
                GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
                GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
                GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
                GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
                GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
                GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
                GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
                GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
                GLboolean last_enable_stencil_test = glIsEnabled(GL_STENCIL_TEST);
                GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
                GLboolean last_enable_primitive_restart = glIsEnabled(GL_PRIMITIVE_RESTART);

                GLuint vertex_array_object = 0;
                glGenVertexArrays(1, &vertex_array_object);

                setup_render_state(draw_data, fb_width, fb_height, vertex_array_object);

                for(int n = 0; n < draw_data->cmd_lists_count; n++) {
                    draw_list* cmd_list = draw_data->cmd_lists[n];

                    std::vector<vertex> vertex_buffer;
                    for(int i = 0; i < cmd_list->vtx_buffer.size(); i++) {
                        detail::vertex vtx = cmd_list->vtx_buffer[i];
                        vertex_buffer.push_back(vertex{ vtx.pos, vtx.uv, vtx.clr.to_unsigned() });
                    }

                    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vertex_buffer.size() * (int)sizeof(vertex), (const GLvoid*)vertex_buffer.data(), GL_STREAM_DRAW);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->idx_buffer.size() * (int)sizeof(unsigned short), (const GLvoid*)cmd_list->idx_buffer.data(), GL_STREAM_DRAW);

                    for(int cmd_i = 0; cmd_i < cmd_list->cmd_buffer.size(); cmd_i++) {
                        detail::cmd* pcmd = &cmd_list->cmd_buffer[cmd_i];

                        if(pcmd->setup_render_state_update_call) {
                            setup_render_state(draw_data, fb_width, fb_height, vertex_array_object);
                            continue;
                        }

                        if(pcmd->callback != nullptr) {
                            pcmd->callback(pcmd);
                            continue;
                        }

                        if(pcmd->clip_rect.min.x < fb_width && pcmd->clip_rect.min.y < fb_height && pcmd->clip_rect.max.x >= 0.0f && pcmd->clip_rect.max.y >= 0.0f) {
                            glScissor((int)pcmd->clip_rect.min.x, (int)(fb_height - pcmd->clip_rect.max.y), (int)(pcmd->clip_rect.size().x), (int)(pcmd->clip_rect.size().y));

                            glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->texture_id);
                            glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)pcmd->elem_count, sizeof(unsigned short) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(intptr_t)(pcmd->idx_offset * sizeof(unsigned short)), (GLint)pcmd->vtx_offset);
                            glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->elem_count, sizeof(unsigned short) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(intptr_t)(pcmd->idx_offset * sizeof(unsigned short)));
                        }
                    }
                }

                glDeleteVertexArrays(1, &vertex_array_object);

                glUseProgram(last_program);
                glBindTexture(GL_TEXTURE_2D, last_texture);
                glBindSampler(0, last_sampler);
                glActiveTexture(last_active_texture);
                glBindVertexArray(last_vertex_array_object);
                glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
                glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
                glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
                if(last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
                if(last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
                if(last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
                if(last_enable_stencil_test) glEnable(GL_STENCIL_TEST); else glDisable(GL_STENCIL_TEST);
                if(last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
                if(last_enable_primitive_restart) glEnable(GL_PRIMITIVE_RESTART); else glDisable(GL_PRIMITIVE_RESTART);

            #ifdef GL_POLYGON_MODE
                glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
            #endif
                glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
                glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
            }

            void setup_render_state(detail::draw_data* draw_data, int fb_width, int fb_height, GLuint vertex_array_object) {
                glEnable(GL_BLEND);
                glBlendEquation(GL_FUNC_ADD);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                glDisable(GL_CULL_FACE);
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_STENCIL_TEST);
                glEnable(GL_SCISSOR_TEST);
                glDisable(GL_PRIMITIVE_RESTART);
            #ifdef GL_POLYGON_MODE
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            #endif

                glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);

                shaders::setup_render_state(draw_data, vertex_array_object);
            }

            bool create_fonts_texture() {
                unsigned char* pixels;
                int width, height;
                font::font_atlas->get_tex_data_as_rgba32(&pixels, &width, &height);

                GLint last_texture;
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
                glGenTextures(1, &font_texture);
                glBindTexture(GL_TEXTURE_2D, font_texture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            #ifdef GL_UNPACK_ROW_LENGTH
                glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            #endif
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

                font::font_atlas->set_tex_id((void*)(intptr_t)font_texture);

                glBindTexture(GL_TEXTURE_2D, last_texture);

                return true;
            }

            void destroy_fonts_texture() {
                if(font_texture) {
                    glDeleteTextures(1, &font_texture);
                    font::font_atlas->set_tex_id(0);
                    font_texture = 0;
                }
            }

            bool create_device_objects() {
                GLint last_texture, last_array_buffer;
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
                glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
                GLint last_vertex_array;
                glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

                shaders::create_shaders();

                create_fonts_texture();

                glBindTexture(GL_TEXTURE_2D, last_texture);
                glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
                glBindVertexArray(last_vertex_array);

                return true;
            }

            void destroy_device_objects() {
                shaders::clear_shaders();

                destroy_fonts_texture();
            }
        }
    }
}