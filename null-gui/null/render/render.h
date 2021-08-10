// Imgui version - 1.80 WIP (https://github.com/ocornut/imgui)
// To render utf-8 text correctly you need to convert it to plain text first - font::detail::erase_utf8. Maybe later I will add more convenient draw_text/text_size interaction with utf-8
#pragma once
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#define STBTT_STATIC
#include <float.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <array>
#include <vector>
#include <string>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../../helpers/flags_list.h"
#include "../../helpers/vectors.h"
#include "../../helpers/color.h"
#include "stb/rectpack.h"
#include "stb/truetype.h"

namespace null {
    namespace font {
        class font;

        namespace detail {
            class src_data {
            public:
                stbtt_fontinfo font_info;
                stbtt_pack_range pack_range;
                stbrp_rect* rects;
                stbtt_packedchar* packed_chars;
                const unsigned short* src_ranges;
                int dst_index;
                int glyphs_highest;
                int glyphs_count;
                std::vector<unsigned int> glyphs_set;
                std::vector<int> glyphs_list;
            };

            class dst_data {
            public:
                int src_count;
                int glyphs_highest;
                int glyphs_count;
                std::vector<unsigned int> glyphs_set;
            };

            class glyph {
            public:
                unsigned int codepoint : 31;
                unsigned int visible : 1;
                float advance_x;
                rect corners;
                rect texture_coordinates;
            };

            class font_config {
            public:
                void* font_data;
                int font_data_size;
                bool font_data_owned_by_atlas;
                int font_no;
                float size_pixels;
                int oversample_h;
                int oversample_v;
                bool pixel_snap_h;
                vec2 glyph_extra_spacing;
                vec2 glyph_offset;
                const unsigned short* glyph_ranges;
                float glyph_min_advance_x;
                float glyph_max_advance_x;
                bool merge_mode;
                unsigned int rasterizer_flags;
                float rasterizer_multiply;
                unsigned short ellipsis_char;
                font* dst_font;

                font_config();
            };

            class glyph_ranges_builder {
            public:
                std::vector<unsigned int> used_chars;

                glyph_ranges_builder() { clear(); }
                void clear() { int size_in_bytes = (0xFFFF + 1) / 8; used_chars.resize(size_in_bytes / (int)sizeof(unsigned int)); memset(used_chars.data(), 0, (size_t)size_in_bytes); }
                bool get_bit(size_t n) const { int off = (int)(n >> 5); unsigned int mask = 1u << (n & 31); return (used_chars[off] & mask) != 0; }
                void set_bit(size_t n) { int off = (int)(n >> 5); unsigned int mask = 1u << (n & 31); used_chars[off] |= mask; }
                void add_char(unsigned short c) { set_bit(c); }
                void add_text(const char* text, const char* text_end = NULL);
                void add_ranges(const unsigned short* ranges);
                void build_ranges(std::vector<unsigned short>* out_ranges);
            };

            class atlas_custom_rect {
            public:
                unsigned short width, height;
                unsigned short x, y;
                unsigned int glyph_id;
                float glyph_advance_x;
                vec2 glyph_offset;
                font* font;
                atlas_custom_rect() { width = height = 0; x = y = 0xFFFF; glyph_id = 0; glyph_advance_x = 0.0f; glyph_offset = vec2(0, 0); font = NULL; }
                bool is_packed() const { return x != 0xFFFF; }
            };

            class atlas {
            public:
                atlas();
                ~atlas();
                font* add_font(const font_config* font_cfg);
                font* add_font_default(const font_config* font_cfg = NULL);
                font* add_font_from_file_ttf(const char* filename, float size_pixels, const font_config* font_cfg = NULL, const unsigned short* glyph_ranges = NULL);
                font* add_font_from_memory_ttf(void* font_data, int font_size, float size_pixels, const font_config* font_cfg = NULL, const unsigned short* glyph_ranges = NULL);
                font* add_font_from_memory_compressed_ttf(const void* compressed_font_data, int compressed_font_size, float size_pixels, const font_config* font_cfg = NULL, const unsigned short* glyph_ranges = NULL);
                font* add_font_from_memory_compressed_base_85_ttf(const char* compressed_font_data_base85, float size_pixels, const font_config* font_cfg = NULL, const unsigned short* glyph_ranges = NULL);
                void clear_input_data();
                void clear_tex_data();
                void clear_fonts();
                void clear();

                bool build();
                void get_tex_data_as_alpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);
                void get_tex_data_as_rgba32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);
                bool is_built() const { return fonts.size() > 0 && (tex_pixels_alpha8 != NULL || tex_pixels_rgba32 != NULL); }
                void set_tex_id(void* id) { tex_id = id; }

                int add_custom_rect_regular(int width, int height);
                int add_custom_rect_font_glyph(font* font, unsigned short id, int width, int height, float advance_x, const vec2& offset = vec2(0, 0));
                atlas_custom_rect* get_custom_rect_by_index(int index) { assert(index >= 0); return &custom_rects[index]; }
                void calc_custom_rect_uv(const atlas_custom_rect* rect, vec2* out_uv_min, vec2* out_uv_max) const;

                bool locked;
                void* tex_id;
                int tex_desired_width;
                int tex_glyph_padding;

                unsigned char* tex_pixels_alpha8;
                unsigned int* tex_pixels_rgba32;
                int tex_width;
                int tex_height;
                vec2 tex_uv_scale;
                vec2 tex_uv_white_pixel;
                std::vector<font*> fonts;
                std::vector<atlas_custom_rect> custom_rects;
                std::vector<font_config> config_data;
                rect tex_uv_lines[64];

                int pack_id_mouse_cursors;
                int pack_id_lines;
            };

            static int upper_power_of_two(int v) { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }
            static int get_char_from_utf8(unsigned int* out_char, const char* in_text, const char* in_text_end);

            const unsigned short* glyph_ranges_default();
            const unsigned short* glyph_ranges_korean();
            const unsigned short* glyph_ranges_chinese_full();
            const unsigned short* glyph_ranges_cyrillic();
            const unsigned short* glyph_ranges_thai();
            const unsigned short* glyph_ranges_vietnamese();

            void atlas_build_render_lines_tex_data(atlas* atlas);
            void atlas_build_render_default_tex_data(atlas* atlas);
            bool atlas_build_with_stb_truetype(atlas* atlas);
            void atlas_build_init(atlas* atlas);
            void atlas_build_setup_font(atlas* atlas, font* font, font_config* font_config, float ascent, float descent);
            void atlas_build_pack_custom_rects(atlas* atlas, void* stbrp_context_opaque);
            void atlas_build_finish(atlas* atlas);
            void atlas_build_render_1bpp_rect_from_string(atlas* atlas, int atlas_x, int atlas_y, int w, int h, const char* in_str, char in_marker_char, unsigned char in_marker_pixel_value);
            void atlas_build_multiply_calc_lookup_table(unsigned char out_table[256], float in_multiply_factor);
            void atlas_build_multiply_rect_alpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride);

            char utf8_to_ascii(wchar_t utf8_char);
            std::wstring convert_wstring(std::string text);
            std::string convert_string(std::wstring text);
            std::string convert_utf8(std::string text);
            std::string erase_utf8(std::string text);

            void unpack_bit_vector_to_flat_index_list(std::vector<unsigned int> source, std::vector<int>* out);
        }

        class font {
        public:
            std::vector<float> index_advance_x;
            float fallback_advance_x;
            float font_size;

            std::vector<unsigned short> index_lookup;
            std::vector<detail::glyph> glyphs;
            detail::glyph* fallback_glyph;

            detail::atlas* container_atlas;
            detail::font_config* config_data;
            short config_data_count;
            unsigned short fallback_char;
            unsigned short ellipsis_char;
            bool dirty_lookup_tables;
            float scale;
            float ascent, descent;
            unsigned char used_4k_pages_map[(0xFFFF + 1) / 4096 / 8];

            font();
            ~font();

            detail::glyph* find_glyph(unsigned short c);
            detail::glyph* find_glyph_no_fallback(unsigned short c);
            float get_char_advance(unsigned short c) const { return ((int)c < index_advance_x.size()) ? index_advance_x[(int)c] : fallback_advance_x; }
            bool is_loaded() const { return container_atlas != NULL; }
            vec2 calc_text_size_w(std::wstring text, float size = -1.f) const;
            vec2 calc_text_size(std::string text, float size = -1.f) const;
            const char* calc_word_wrap_position_a(float scale, const char* text, const char* text_end, float wrap_width) const;

            void build_lookup_table();
            void clear_output_data();
            void grow_index(int new_size);
            void add_glyph(detail::font_config* src_cfg, unsigned short c, rect corners, rect texture_coordinates, float advance_x);
            void add_remap_char(unsigned short dst, unsigned short src, bool overwrite_dst = true);
            void set_glyph_visible(unsigned short c, bool visible);
            void set_fallback_char(unsigned short c);
            bool is_glyph_range_unused(unsigned int c_begin, unsigned int c_last);
        };

        inline bool atlas_owned_by_initialize;
        inline font* main_font;
        inline float font_size;
        inline float font_base_size;
        inline std::vector<font*> font_list;
        inline detail::atlas* font_atlas;
        inline float font_global_scale;

        void set_current_font(font* font);
        vec2 text_size_w(std::wstring text);
        vec2 text_size(std::string text);
        static font* get_default_font() { return main_font ? main_font : font_atlas->fonts[0]; }

        void push_font(font* font);
        void pop_font();

        static font* load_font(std::string filename, float size_pixels, const unsigned short* glyph_ranges = detail::glyph_ranges_cyrillic(), detail::font_config* font_cfg = NULL) { return font_atlas->add_font_from_file_ttf(filename.c_str(), size_pixels, font_cfg, glyph_ranges); }
        static font* load_font_memory(void* font_data, int font_size, float size_pixels, const unsigned short* glyph_ranges = detail::glyph_ranges_cyrillic(), detail::font_config* font_cfg = NULL) { return font_atlas->add_font_from_memory_ttf(font_data, font_size, size_pixels, font_cfg, glyph_ranges); }
        static font* load_font_compressed(const void* compressed_font_data, int compressed_font_size, float size_pixels, const unsigned short* glyph_ranges = detail::glyph_ranges_cyrillic(), detail::font_config* font_cfg = NULL) { return font_atlas->add_font_from_memory_compressed_ttf(compressed_font_data, compressed_font_size, size_pixels, font_cfg, glyph_ranges); }
        static font* load_font_base_85(const char* compressed_font_data_base85, float size_pixels, const unsigned short* glyph_ranges = detail::glyph_ranges_cyrillic(), detail::font_config* font_cfg = NULL) { return font_atlas->add_font_from_memory_compressed_base_85_ttf(compressed_font_data_base85, size_pixels, font_cfg, glyph_ranges); }
    }

    namespace render {
        class draw_list;

        enum class draw_list_flags {
            anti_aliased_lines = 0,
            anti_aliased_lines_use_tex,
            anti_aliased_fill,
            allow_vtx_offset
        };

        enum class corner_flags {
            top_left,
            top_right,
            bot_left,
            bot_right,
            top,
            bot,
            left,
            right,
            all
        };

        namespace detail {
            class cmd_header {
            public:
                rect clip_rect;
                void* texture_id;
                unsigned int vtx_offset;
            };

            class vertex {
            public:
                vec2 pos;
                vec2 uv;
                color clr;
            };

            class data_builder {
            public:
                std::vector<draw_list*> layer;

                void clear() { layer.resize(0); }
                void clear_free_memory() { layer.clear(); }
            };

            class draw_data {
            public:
                bool valid;
                draw_list** cmd_lists;
                int cmd_lists_count;
                int total_idx_count;
                int total_vtx_count;
                vec2 display_pos;
                vec2 display_size;

                draw_data() { valid = false; clear(); }
                ~draw_data() { clear(); }
                void clear() { valid = false; cmd_lists = NULL; cmd_lists_count = total_vtx_count = total_idx_count = 0; display_pos = display_size = vec2(0.f, 0.f); }
                void de_index_all_buffers();
                void scale_clip_rects(vec2 fb_scale);
            };

            class shared_data {
            public:
                vec2 tex_uv_white_pixel;
                font::font* font;
                float font_size;
                float curve_tessellation_tol;
                float circle_segment_max_error;
                rect clip_rect_fullscreen;
                flags_list<draw_list_flags> initial_flags;
                std::array<vec2, 12> arc_fast_vtx;
                std::array<unsigned char, 64> circle_segment_counts;
                const rect* tex_uv_lines;

                shared_data();
                void set_circle_segment_max_error(float max_error);
            };

            class cmd {
            public:
                rect clip_rect;
                void* texture_id;
                unsigned int vtx_offset;
                unsigned int idx_offset;
                unsigned int elem_count;
                bool setup_render_state_update_call;
                std::function<void(cmd*)> callback;

                cmd() { memset(this, 0, sizeof(*this)); }
            };

            void shade_verts_linear_uv(draw_list* draw_list, int vert_start_idx, int vert_end_idx, vec2 a, vec2 b, vec2 uv_a, vec2 uv_b, bool clamp);
            void add_draw_list_to_draw_data(std::vector<draw_list*>* out_list, draw_list* draw_list);
            void setup_draw_data(std::vector<draw_list*>* draw_lists, draw_data* draw_data);
        }

        class draw_list {
        public:
            std::vector<detail::cmd> cmd_buffer;
            std::vector<unsigned short> idx_buffer;
            std::vector<detail::vertex> vtx_buffer;
            flags_list<draw_list_flags> flags;

            unsigned int vtx_current_idx;
            detail::shared_data* shared_data;
            detail::vertex* vtx_write_ptr;
            unsigned short* idx_write_ptr;
            std::vector<rect> clip_rect_stack;
            std::vector<void*> texture_id_stack;
            std::vector<vec2> path;
            detail::cmd_header cmd_header;

            void initialize(detail::shared_data* _shared_data) { shared_data = _shared_data; }
            draw_list(detail::shared_data* _shared_data = NULL) { shared_data = _shared_data; }
            ~draw_list() { clear_free_memory(); }

            void push_clip_rect(vec2 clip_rect_min, vec2 clip_rect_max, bool intersect_with_current_clip_rect = false);
            void push_clip_rect_full_screen() { push_clip_rect(shared_data->clip_rect_fullscreen.min, shared_data->clip_rect_fullscreen.max); }
            void pop_clip_rect();
            void push_texture_id(void* texture_id);
            void pop_texture_id();
            rect get_clip_rect() { return clip_rect_stack.back(); }
            vec2 get_clip_rect_min() { return get_clip_rect().min; }
            vec2 get_clip_rect_max() { return get_clip_rect().max; }

            void draw_blur(vec2 start, vec2 end, float amount = 1.f, float alpha = 1.f, float rounding = 0.f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all));
            void draw_line(vec2 start_point, vec2 end_point, color clr, float thickness = 1.0f);
            void draw_rect(vec2 min, vec2 max, color clr, float rounding = 0.0f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), float thickness = 1.0f);
            void draw_rect_filled(vec2 min, vec2 max, color clr, float rounding = 0.0f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all));
            void draw_rect_filled_multicolor(vec2 min, vec2 max, std::array<color, 2> upper_colors, std::array<color, 2> bottom_colors, float rounding = 0.f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all));
            void draw_rect_filled_multicolor_vertical(vec2 min, vec2 max, color upper_color, color bottom_color, float rounding = 0.f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all));
            void draw_rect_filled_multicolor_horizontal(vec2 min, vec2 max, color left_color, color right_color, float rounding = 0.f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all));
            void draw_quad(std::array<vec2, 4> points, color clr, float thickness = 1.0f);
            void draw_quad_filled(std::array<vec2, 4> points, color clr);
            void draw_triangle(std::array<vec2, 3> points, color clr, float thickness = 1.0f);
            void draw_triangle_filled(std::array<vec2, 3> points, color clr);
            void draw_circle(vec2 center, color clr, float radius, int num_segments = 0, float thickness = 1.0f);
            void draw_circle_filled(vec2 center, color clr, float radius, int num_segments = 0);
            void draw_ngon(vec2 center, float radius, color clr, int num_segments, float thickness = 1.0f);
            void draw_ngon_filled(vec2 center, float radius, color clr, int num_segments);
            void draw_char(font::font* font, float size, vec2 pos, color clr, unsigned short c);
            void draw_text_multicolor(std::vector<std::pair<std::string, color>> text_list, vec2 pos, bool outline = true, std::array<bool, 2> centered = { false, false }, font::font* font = NULL, float size = 0.f);
            void draw_text_multicolor(std::vector<std::pair<std::string, color>> text_list, vec2 pos, font::font* font, float size);
            void draw_text(std::string text, vec2 pos, color clr, bool outline = true, std::array<bool, 2> centered = { false, false }, font::font* font = NULL, float size = 0.f);
            void draw_text(std::string text, vec2 pos, color clr, font::font* font, float size);
            void draw_poly_line(std::vector<vec2> points, color clr, bool closed, float thickness);
            void draw_convex_poly_filled(std::vector<vec2> points, color clr);
            void draw_convex_poly_filled_multicolor(std::vector<vec2> points, vec2 min, vec2 max, std::array<color, 2> upper_colors, std::array<color, 2> bottom_colors);
            void draw_convex_poly_filled_multicolor_liner(std::vector<vec2> points, vec2 min, vec2 max, color first_color, color second_color);
            void draw_bezier_curve(std::array<vec2, 4> points, color clr, float thickness, int num_segments = 0);
            void draw_image(void* user_texture_id, vec2 min, vec2 max, vec2 uv_min = vec2(0, 0), vec2 uv_max = vec2(1, 1), color clr = color(1.f, 1.f, 1.f, 1.f));
            void draw_image_quad(void* user_texture_id, std::array<vec2, 4> points, std::array<vec2, 4> uvs = { vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1) }, color clr = color(1.f, 1.f, 1.f, 1.f));
            void draw_image_rounded(void* user_texture_id, vec2 min, vec2 max, vec2 uv_min, vec2 uv_max, color clr, float rounding, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all));

            void path_clear() { path.resize(0); }
            void path_line_to(vec2 pos) { path.push_back(pos); }
            void path_line_to_merge_duplicate(vec2 pos) { if(path.size() == 0 || memcmp(&path.data()[path.size() - 1], &pos, 8) != 0) path.push_back(pos); }
            void path_fill_convex(color clr) { draw_convex_poly_filled(path, clr); path.resize(0); }
            void path_fill_convex_multicolor(vec2 min, vec2 max, std::array<color, 2> upper_colors, std::array<color, 2> bottom_colors) { draw_convex_poly_filled_multicolor(path, min, max, upper_colors, bottom_colors); path.resize(0); }
            void path_fill_convex_multicolor_liner(vec2 min, vec2 max, color first_color, color second_color) { draw_convex_poly_filled_multicolor_liner(path, min, max, first_color, second_color); path.resize(0); }
            void path_stroke(color clr, bool closed, float thickness = 1.0f) { draw_poly_line(path, clr, closed, thickness); path.resize(0); }
            void path_arc_to(vec2 center, float radius, float a_min, float a_max, int num_segments = 10);
            void path_arc_to_fast(vec2 center, float radius, int a_min_of_12, int a_max_of_12);
            void path_bezier_curve_to(std::array<vec2, 3> points, int num_segments = 0);
            void path_rect(vec2 rect_min, vec2 rect_max, float rounding = 0.0f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all));
            void path_bezier_to_casteljau(std::vector<vec2>* path, std::array<vec2, 4> points, float tess_tol, int level);

            void add_callback(std::function<void(detail::cmd*)> callback, bool update_render_state = false);
            void add_draw_cmd();
            draw_list* clone_output() const;

            void prim_reserve(int idx_count, int vtx_count);
            void prim_unreserve(int idx_count, int vtx_count);
            void prim_rect(vec2 a, vec2 b, color clr);
            void prim_rect_uv(vec2 a, vec2 b, vec2 uv_a, vec2 uv_b, color clr);
            void prim_quad_uv(std::array<vec2, 4> points, std::array<vec2, 4> uvs, color clr);
            void prim_write_vtx(vec2 pos, vec2 uv, color clr) { vtx_write_ptr->pos = pos; vtx_write_ptr->uv = uv; vtx_write_ptr->clr = clr; vtx_write_ptr++; vtx_current_idx++; }
            void prim_write_idx(unsigned short idx) { *idx_write_ptr = idx; idx_write_ptr++; }
            void prim_vtx(vec2 pos, vec2 uv, color clr) { prim_write_idx((unsigned short)vtx_current_idx); prim_write_vtx(pos, uv, clr); }

            void reset_for_begin_render();
            void clear_free_memory();
            void pop_unused_draw_cmd();
            void on_changed_clip_rect();
            void on_changed_texture_id();
            void on_changed_vtx_offset();
        };

        inline detail::draw_data draw_data;
        inline detail::data_builder data_builder;
        inline detail::shared_data shared_data;

        inline draw_list background_draw_list;
        inline draw_list foreground_draw_list;

        inline bool initialized;
        inline bool renderer_has_vtx_offset = true;
        inline vec2 display_size;

        void initialize(font::detail::atlas* shared_font_atlas = NULL);
        void begin_frame(HWND hwnd);
        void setup_draw_data();
        static void end_frame() { font::font_atlas->locked = false; }
        void shutdown();


        static void push_clip_rect(rect clip_rect, bool intersect_with_current_clip_rect = false, draw_list* list = &background_draw_list) { list->push_clip_rect(clip_rect.min, clip_rect.max, intersect_with_current_clip_rect); }
        static void push_clip_rect(vec2 min, vec2 max, bool intersect_with_current_clip_rect = false, draw_list* list = &background_draw_list) { list->push_clip_rect(min, max, intersect_with_current_clip_rect); }
        static void pop_clip_rect(draw_list* list = &background_draw_list) { list->pop_clip_rect(); }

        static void draw_blur(vec2 start, vec2 end, float amount = 1.f, float alpha = 1.f, float rounding = 0.f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), draw_list* list = &background_draw_list) { list->draw_blur(start, end, amount, alpha, rounding, rounding_corners); }
        static void draw_line(vec2 start_point, vec2 end_point, color clr, float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_line(start_point, end_point, clr, thickness); }
        static void draw_rect(vec2 min, vec2 max, color clr, float rounding = 0.0f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_rect(min, max, clr, rounding, rounding_corners, thickness); }
        static void draw_rect_filled(vec2 min, vec2 max, color clr, float rounding = 0.0f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), draw_list* list = &background_draw_list) { list->draw_rect_filled(min, max, clr, rounding, rounding_corners); }
        static void draw_rect_filled_multicolor(vec2 min, vec2 max, std::array<color, 2> upper_colors, std::array<color, 2> bottom_colors, float rounding = 0.f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), draw_list* list = &background_draw_list) { list->draw_rect_filled_multicolor(min, max, upper_colors, bottom_colors, rounding, rounding_corners); }
        static void draw_rect_filled_multicolor_vertical(vec2 min, vec2 max, color upper_color, color bottom_color, float rounding = 0.f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), draw_list* list = &background_draw_list) { list->draw_rect_filled_multicolor_vertical(min, max, upper_color, bottom_color, rounding, rounding_corners); }
        static void draw_rect_filled_multicolor_horizontal(vec2 min, vec2 max, color left_color, color right_color, float rounding = 0.f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), draw_list* list = &background_draw_list) { list->draw_rect_filled_multicolor_horizontal(min, max, left_color, right_color, rounding, rounding_corners); }
        static void draw_quad(std::array<vec2, 4> points, color clr, float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_quad(points, clr, thickness); }
        static void draw_quad_filled(std::array<vec2, 4> points, color clr, draw_list* list = &background_draw_list) { list->draw_quad_filled(points, clr); }
        static void draw_triangle(std::array<vec2, 3> points, color clr, float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_triangle(points, clr, thickness); }
        static void draw_triangle_filled(std::array<vec2, 3> points, color clr, draw_list* list = &background_draw_list) { list->draw_triangle_filled(points, clr); }
        static void draw_circle(vec2 center, color clr, float radius, int num_segments = 0, float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_circle(center, clr, radius, num_segments, thickness); }
        static void draw_circle_filled(vec2 center, color clr, float radius, int num_segments = 0, draw_list* list = &background_draw_list) { list->draw_circle_filled(center, clr, radius, num_segments); }
        static void draw_ngon(vec2 center, float radius, color clr, int num_segments, float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_ngon(center, radius, clr, num_segments, thickness); }
        static void draw_ngon_filled(vec2 center, float radius, color clr, int num_segments, draw_list* list = &background_draw_list) { list->draw_ngon_filled(center, radius, clr, num_segments); }
        static void draw_char(font::font* font, float size, vec2 pos, color clr, unsigned short c, draw_list* list = &background_draw_list) { list->draw_char(font, size, pos, clr, c); }
        static void draw_text_multicolor(std::vector<std::pair<std::string, color>> text_list, vec2 pos, bool outline = true, std::array<bool, 2> centered = { false, false }, draw_list* list = &background_draw_list) { list->draw_text_multicolor(text_list, pos, outline, centered); }
        static void draw_text_multicolor(std::vector<std::pair<std::string, color>> text_list, vec2 pos, font::font* font, float size, draw_list* list = &background_draw_list) { list->draw_text_multicolor(text_list, pos, font, size); }
        static void draw_text(std::string text, vec2 pos, color clr, bool outline = true, std::array<bool, 2> centered = { false, false }, draw_list* list = &background_draw_list) { list->draw_text(text, pos, clr, outline, centered); }
        static void draw_text(std::string text, vec2 pos, color clr, font::font* font, float size, draw_list* list = &background_draw_list) { list->draw_text(text, pos, clr, font, size); }
        static void draw_polyline(std::vector<vec2> points, color clr, bool closed, float thickness, draw_list* list = &background_draw_list) { list->draw_poly_line(points, clr, closed, thickness); }
        static void draw_convex_poly_filled(std::vector<vec2> points, color clr, draw_list* list = &background_draw_list) { list->draw_convex_poly_filled(points, clr); }
        static void draw_image(void* user_texture_id, vec2 min, vec2 max, vec2 uv_min = vec2(0, 0), vec2 uv_max = vec2(1, 1), color clr = color(1.f, 1.f, 1.f, 1.f), draw_list* list = &background_draw_list) { list->draw_image(user_texture_id, min, max, uv_min, uv_max, clr); }
        static void draw_image_quad(void* user_texture_id, std::array<vec2, 4> points, std::array<vec2, 4> uvs = { vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(0, 1) }, color clr = color(1.f, 1.f, 1.f, 1.f), draw_list* list = &background_draw_list) { list->draw_image_quad(user_texture_id, points, uvs, clr); }
        static void draw_image_rounded(void* user_texture_id, vec2 min, vec2 max, vec2 uv_min, vec2 uv_max, color clr, float rounding, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), draw_list* list = &background_draw_list) { list->draw_image_rounded(user_texture_id, min, max, uv_min, uv_max, clr, rounding, rounding_corners); }
    }
}