// Imgui version - 1.80 WIP (https://github.com/ocornut/imgui)
// To render utf-8 text correctly you need to convert it to plain text first - null_font::helpers::erase_utf8. Maybe later I will add more convenient draw_text/text_size interaction with utf-8
#pragma once
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
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

#include "../helpers//flags_list.h"
#include "../helpers/vectors.h"
#include "../helpers/color.h"
#include "stb/rectpack.h"
#include "stb/truetype.h"

namespace custom_stl {
    class bit_vector {
    public:
        std::vector<unsigned int> storage;
        void create(int sz) {
            storage.resize((sz + 31) >> 5);
            std::fill(storage.begin(), storage.end(), 0);
        }

        void clear() {
            storage.clear();
        }

        bool test_bit(int n) const {
            assert(n < (storage.size() << 5));
            unsigned int mask = (unsigned int)1 << (n & 31);
            return (storage[n >> 5] & mask) != 0;
        }

        void set_bit(int n) {
            assert(n < (storage.size() << 5));
            unsigned int mask = (unsigned int)1 << (n & 31);
            storage[n >> 5] |= mask;
        }

        void clear_bit(int n) {
            assert(n < (storage.size() << 5));
            unsigned int mask = (unsigned int)1 << (n & 31);
            storage[n >> 5] &= ~mask;
        }

        void unpack_bit_vector_to_flat_index_list(std::vector<int>* out) {
            assert(sizeof(storage.data()[0]) == sizeof(int));
            const unsigned int* begin = storage.data();
            const unsigned int* end = storage.data() + storage.size();
            for (const unsigned int* it = begin; it < end; it++)
                if (unsigned int entries_32 = *it)
                    for (unsigned int bit_n = 0; bit_n < 32; bit_n++)
                        if (entries_32 & ((unsigned int)1 << bit_n)) {
                            if (out->size() <= 0) *out = std::vector<int>{ (int)(((it - begin) << 5) + bit_n) };
                            else out->push_back((int)(((it - begin) << 5) + bit_n));
                        }
        }
    };
}

namespace null_font {
    class font;

    namespace helpers {
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
            custom_stl::bit_vector glyphs_set;
            std::vector<int> glyphs_list;
        };

        class dst_data {
        public:
            int src_count;
            int glyphs_highest;
            int glyphs_count;
            custom_stl::bit_vector glyphs_set;
        };

        class glyph {
        public:
            unsigned int codepoint : 31;
            unsigned int visible : 1;
            float advance_x;
            float x0, y0, x1, y1;
            float u0, v0, u1, v1;
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

        int upper_power_of_two(int v) { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }
        int get_char_from_utf8(unsigned int* out_char, const char* in_text, const char* in_text_end);

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
    }

    class font {
    public:
        std::vector<float> index_advance_x;
        float fallback_advance_x;
        float font_size;

        std::vector<unsigned short> index_lookup;
        std::vector<helpers::glyph> glyphs;
        helpers::glyph* fallback_glyph;

        helpers::atlas* container_atlas;
        helpers::font_config* config_data;
        short config_data_count;
        unsigned short fallback_char;
        unsigned short ellipsis_char;
        bool dirty_lookup_tables;
        float scale;
        float ascent, descent;
        unsigned char used_4k_pages_map[(0xFFFF + 1) / 4096 / 8];

        font();
        ~font();

        helpers::glyph* find_glyph(unsigned short c);
        helpers::glyph* find_glyph_no_fallback(unsigned short c);
        float get_char_advance(unsigned short c) const { return ((int)c < index_advance_x.size()) ? index_advance_x[(int)c] : fallback_advance_x; }
        bool is_loaded() const { return container_atlas != NULL; }
        vec2 calc_text_size_w(std::wstring text, float size = -1.f) const;
        vec2 calc_text_size(std::string text, float size = -1.f) const;
        const char* calc_word_wrap_position_a(float scale, const char* text, const char* text_end, float wrap_width) const;

        void build_lookup_table();
        void clear_output_data();
        void grow_index(int new_size);
        void add_glyph(helpers::font_config* src_cfg, unsigned short c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x);
        void add_remap_char(unsigned short dst, unsigned short src, bool overwrite_dst = true);
        void set_glyph_visible(unsigned short c, bool visible);
        void set_fallback_char(unsigned short c);
        bool is_glyph_range_unused(unsigned int c_begin, unsigned int c_last);
    };

    namespace vars {
        bool atlas_owned_by_initialize;
        font* main_font;
        float font_size;
        float font_base_size;
        std::vector<font*> font_list;
        helpers::atlas* font_atlas;
        float font_global_scale;
    }

    void set_current_font(font* font);
    vec2 text_size_w(std::wstring text);
    vec2 text_size(std::string text);
    font* get_default_font() { return vars::main_font ? vars::main_font : vars::font_atlas->fonts[0]; }

    void push_font(font* font);
    void pop_font();

    font* load_font(std::string filename, float size_pixels, const unsigned short* glyph_ranges = helpers::glyph_ranges_cyrillic(), helpers::font_config* font_cfg = NULL) { return vars::font_atlas->add_font_from_file_ttf(filename.c_str(), size_pixels, font_cfg, glyph_ranges); }
    font* load_font_memory(void* font_data, int font_size, float size_pixels, const unsigned short* glyph_ranges = helpers::glyph_ranges_cyrillic(), helpers::font_config* font_cfg = NULL) { return vars::font_atlas->add_font_from_memory_ttf(font_data, font_size, size_pixels, font_cfg, glyph_ranges); }
    font* load_font_compressed(const void* compressed_font_data, int compressed_font_size, float size_pixels, const unsigned short* glyph_ranges = helpers::glyph_ranges_cyrillic(), helpers::font_config* font_cfg = NULL) { return vars::font_atlas->add_font_from_memory_compressed_ttf(compressed_font_data, compressed_font_size, size_pixels, font_cfg, glyph_ranges); }
    font* load_font_base_85(const char* compressed_font_data_base85, float size_pixels, const unsigned short* glyph_ranges = helpers::glyph_ranges_cyrillic(), helpers::font_config* font_cfg = NULL) { return vars::font_atlas->add_font_from_memory_compressed_base_85_ttf(compressed_font_data_base85, size_pixels, font_cfg, glyph_ranges); }
}

namespace null_render {
    namespace shaders {
        namespace blur {
            class shader;
        }
    }

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

    namespace helpers {
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
            null_font::font* font;
            float font_size;
            float curve_tessellation_tol;
            float circle_segment_max_error;
            rect clip_rect_fullscreen;
            flags_list<draw_list_flags> initial_flags;
            vec2 arc_fast_vtx[12];
            unsigned char circle_segment_counts[64];
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
        void setup_draw_data(std::vector<draw_list*>* draw_lists, helpers::draw_data* draw_data);
    }

    class draw_list {
    public:
        std::vector<helpers::cmd> cmd_buffer;
        std::vector<unsigned short> idx_buffer;
        std::vector<helpers::vertex> vtx_buffer;
        flags_list<draw_list_flags> flags;

        unsigned int _vtx_current_idx;
        helpers::shared_data* _data;
        helpers::vertex* _vtx_write_ptr;
        unsigned short* _idx_write_ptr;
        std::vector<rect> _clip_rect_stack;
        std::vector<void*> _texture_id_stack;
        std::vector<vec2> _path;
        helpers::cmd_header _cmd_header;

        void initialize(helpers::shared_data* shared_data) { _data = shared_data; }
        draw_list(helpers::shared_data* shared_data = NULL) { _data = shared_data; }
        ~draw_list() { _clear_free_memory(); }

        void push_clip_rect(vec2 clip_rect_min, vec2 clip_rect_max, bool intersect_with_current_clip_rect = false);
        void push_clip_rect_full_screen();
        void pop_clip_rect();
        void push_texture_id(void* texture_id);
        void pop_texture_id();
        rect get_clip_rect() { return _clip_rect_stack.back(); }
        vec2 get_clip_rect_min() { return get_clip_rect().min; }
        vec2 get_clip_rect_max() { return get_clip_rect().max; }

        void draw_blur(vec2 start, vec2 end, float amount = 1.f, float alpha = 1.f, float rounding = 0.f);
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
        void draw_char(null_font::font* font, float size, vec2 pos, color clr, unsigned short c);
        void draw_text_multicolor(std::vector<std::pair<std::string, color>> text_list, vec2 pos, bool outline = true, std::array<bool, 2> centered = { false, false }, null_font::font* font = NULL, float size = 0.f);
        void draw_text_multicolor(std::vector<std::pair<std::string, color>> text_list, vec2 pos, null_font::font* font, float size, rect* clip_rect = NULL, bool cpu_fine_clip = false);
        void draw_text(std::string text, vec2 pos, color clr, bool outline = true, std::array<bool, 2> centered = { false, false }, null_font::font* font = NULL, float size = 0.f);
        void draw_text(std::string text, vec2 pos, color clr, null_font::font* font, float size, rect* clip_rect = NULL, bool cpu_fine_clip = false);
        void draw_poly_line(std::vector<vec2> points, color clr, bool closed, float thickness);
        void draw_convex_poly_filled(std::vector<vec2> points, color clr);
        void draw_convex_poly_filled_multicolor(std::vector<vec2> points, vec2 min, vec2 max, std::array<color, 2> upper_colors, std::array<color, 2> bottom_colors);
        void draw_convex_poly_filled_multicolor_liner(std::vector<vec2> points, vec2 min, vec2 max, color first_color, color second_color);
        void draw_bezier_curve(std::array<vec2, 4> points, color clr, float thickness, int num_segments = 0);
        void draw_image(void* user_texture_id, vec2 min, vec2 max, vec2 uv_min = vec2(0, 0), vec2 uv_max = vec2(1, 1), color clr = color(1.f, 1.f, 1.f, 1.f));
        void draw_image_quad(void* user_texture_id, std::array<vec2, 4> points, std::array<vec2, 4> uvs = { vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1)}, color clr = color(1.f, 1.f, 1.f, 1.f));
        void draw_image_rounded(void* user_texture_id, vec2 min, vec2 max, vec2 uv_min, vec2 uv_max, color clr, float rounding, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all));

        void path_clear() { _path.resize(0); }
        void path_line_to(vec2 pos) { _path.push_back(pos); }
        void path_line_to_merge_duplicate(vec2 pos) { if (_path.size() == 0 || memcmp(&_path.data()[_path.size() - 1], &pos, 8) != 0) _path.push_back(pos); }
        void path_fill_convex(color clr) { draw_convex_poly_filled(_path, clr); _path.resize(0); }
        void path_fill_convex_multicolor(vec2 min, vec2 max, std::array<color, 2> upper_colors, std::array<color, 2> bottom_colors) { draw_convex_poly_filled_multicolor(_path, min, max, upper_colors, bottom_colors); _path.resize(0); }
        void path_fill_convex_multicolor_liner(vec2 min, vec2 max, color first_color, color second_color) { draw_convex_poly_filled_multicolor_liner(_path, min, max, first_color, second_color); _path.resize(0); }
        void path_stroke(color clr, bool closed, float thickness = 1.0f) { draw_poly_line(_path, clr, closed, thickness); _path.resize(0); }
        void path_arc_to(vec2 center, float radius, float a_min, float a_max, int num_segments = 10);
        void path_arc_to_fast(vec2 center, float radius, int a_min_of_12, int a_max_of_12);
        void path_bezier_curve_to(std::array<vec2, 3> points, int num_segments = 0);
        void path_rect(vec2 rect_min, vec2 rect_max, float rounding = 0.0f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all));
        void path_bezier_to_casteljau(std::vector<vec2>* path, std::array<vec2, 4> points, float tess_tol, int level);

        void add_callback(std::function<void(helpers::cmd*)> callback, bool update_render_state = false);
        void add_draw_cmd();
        draw_list* clone_output() const;

        void prim_reserve(int idx_count, int vtx_count);
        void prim_unreserve(int idx_count, int vtx_count);
        void prim_rect(vec2 a, vec2 b, color clr);
        void prim_rect_uv(vec2 a, vec2 b, vec2 uv_a, vec2 uv_b, color clr);
        void prim_quad_uv(std::array<vec2, 4> points, std::array<vec2, 4> uvs, color clr);
        void prim_write_vtx(vec2 pos, vec2 uv, color clr) { _vtx_write_ptr->pos = pos; _vtx_write_ptr->uv = uv; _vtx_write_ptr->clr = clr; _vtx_write_ptr++; _vtx_current_idx++; }
        void prim_write_idx(unsigned short idx) { *_idx_write_ptr = idx; _idx_write_ptr++; }
        void prim_vtx(vec2 pos, vec2 uv, color clr) { prim_write_idx((unsigned short)_vtx_current_idx); prim_write_vtx(pos, uv, clr); }

        void _reset_for_begin_render();
        void _clear_free_memory();
        void _pop_unused_draw_cmd();
        void _on_changed_clip_rect();
        void _on_changed_texture_id();
        void _on_changed_vtx_offset();
    };

    helpers::draw_data draw_data;
    helpers::data_builder data_builder;
    helpers::shared_data shared_data;

    draw_list background_draw_list;
    draw_list foreground_draw_list;

    namespace settings {
        bool initialized;
        bool renderer_has_vtx_offset = true;
        vec2 display_size;
    }

    void initialize(null_font::helpers::atlas* shared_font_atlas = NULL) {
        background_draw_list.initialize(&shared_data);
        foreground_draw_list.initialize(&shared_data);
        settings::initialized = false;
        null_font::vars::atlas_owned_by_initialize = shared_font_atlas ? false : true;
        null_font::vars::main_font = NULL;
        null_font::vars::font_size = null_font::vars::font_base_size = 0.0f;
        null_font::vars::font_atlas = shared_font_atlas ? shared_font_atlas : new null_font::helpers::atlas();
        //settings::renderer_has_vtx_offset = false;
        settings::display_size = vec2(0.f, 0.f);
    }

    void begin_render(HWND hwnd);
    void render();
    void end_render() { null_font::vars::font_atlas->locked = false; }
    void shutdown();


    void push_clip_rect(rect clip_rect, bool intersect_with_current_clip_rect = false, draw_list* list = &background_draw_list) { list->push_clip_rect(clip_rect.min, clip_rect.max, intersect_with_current_clip_rect); }
    void push_clip_rect(vec2 min, vec2 max, bool intersect_with_current_clip_rect = false, draw_list* list = &background_draw_list) { list->push_clip_rect(min, max, intersect_with_current_clip_rect); }
    void pop_clip_rect(draw_list* list = &background_draw_list) { list->pop_clip_rect(); }

    void draw_blur(vec2 start, vec2 end, float amount, float alpha, float rounding, draw_list* list = &background_draw_list) { list->draw_blur(start, end, amount, alpha, rounding); }
    void draw_line(vec2 start_point, vec2 end_point, color clr, float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_line(start_point, end_point, clr, thickness); }
    void draw_rect(vec2 min, vec2 max, color clr, float rounding = 0.0f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_rect(min, max, clr, rounding, rounding_corners, thickness); }
    void draw_rect_filled(vec2 min, vec2 max, color clr, float rounding = 0.0f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), draw_list* list = &background_draw_list) { list->draw_rect_filled(min, max, clr, rounding, rounding_corners); }
    void draw_rect_filled_multicolor(vec2 min, vec2 max, std::array<color, 2> upper_colors, std::array<color, 2> bottom_colors, float rounding = 0.f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), draw_list* list = &background_draw_list) { list->draw_rect_filled_multicolor(min, max, upper_colors, bottom_colors, rounding, rounding_corners); }
    void draw_rect_filled_multicolor_vertical(vec2 min, vec2 max, color upper_color, color bottom_color, float rounding = 0.f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), draw_list* list = &background_draw_list) { list->draw_rect_filled_multicolor_vertical(min, max, upper_color, bottom_color, rounding, rounding_corners); }
    void draw_rect_filled_multicolor_horizontal(vec2 min, vec2 max, color left_color, color right_color, float rounding = 0.f, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), draw_list* list = &background_draw_list) { list->draw_rect_filled_multicolor_horizontal(min, max, left_color, right_color, rounding, rounding_corners); }
    void draw_quad(std::array<vec2, 4> points, color clr, float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_quad(points, clr, thickness); }
    void draw_quad_filled(std::array<vec2, 4> points, color clr, draw_list* list = &background_draw_list) { list->draw_quad_filled(points, clr); }
    void draw_triangle(std::array<vec2, 3> points, color clr, float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_triangle(points, clr, thickness); }
    void draw_triangle_filled(std::array<vec2, 3> points, color clr, draw_list* list = &background_draw_list) { list->draw_triangle_filled(points, clr); }
    void draw_circle(vec2 center, color clr, float radius, int num_segments = 0, float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_circle(center, clr, radius, num_segments, thickness); }
    void draw_circle_filled(vec2 center, color clr, float radius, int num_segments = 0, draw_list* list = &background_draw_list) { list->draw_circle_filled(center, clr, radius, num_segments); }
    void draw_ngon(vec2 center, float radius, color clr, int num_segments, float thickness = 1.0f, draw_list* list = &background_draw_list) { list->draw_ngon(center, radius, clr, num_segments, thickness); }
    void draw_ngon_filled(vec2 center, float radius, color clr, int num_segments, draw_list* list = &background_draw_list) { list->draw_ngon_filled(center, radius, clr, num_segments); }
    void draw_char(null_font::font* font, float size, vec2 pos, color clr, unsigned short c, draw_list* list = &background_draw_list) { list->draw_char(font, size, pos, clr, c); }
    void draw_text_multicolor(std::vector<std::pair<std::string, color>> text_list, vec2 pos, bool outline = true, std::array<bool, 2> centered = { false, false }, draw_list* list = &background_draw_list) { list->draw_text_multicolor(text_list, pos, outline, centered); }
    void draw_text_multicolor(std::vector<std::pair<std::string, color>> text_list, vec2 pos, null_font::font* font, float size, rect* clip_rect = NULL, bool cpu_fine_clip = false, draw_list* list = &background_draw_list) { list->draw_text_multicolor(text_list, pos, font, size, clip_rect, cpu_fine_clip); }
    void draw_text(std::string text, vec2 pos, color clr, bool outline = true, std::array<bool, 2> centered = { false, false }, draw_list* list = &background_draw_list) { list->draw_text(text, pos, clr, outline, centered); }
    void draw_text(std::string text, vec2 pos, color clr, null_font::font* font, float size, rect* clip_rect = NULL, bool cpu_fine_clip = false, draw_list* list = &background_draw_list) { list->draw_text(text, pos, clr, font, size, clip_rect, cpu_fine_clip); }
    void draw_polyline(std::vector<vec2> points, color clr, bool closed, float thickness, draw_list* list = &background_draw_list) { list->draw_poly_line(points, clr, closed, thickness); }
    void draw_convex_poly_filled(std::vector<vec2> points, color clr, draw_list* list = &background_draw_list) { list->draw_convex_poly_filled(points, clr); }
    void draw_image(void* user_texture_id, vec2 min, vec2 max, vec2 uv_min = vec2(0, 0), vec2 uv_max = vec2(1, 1), color clr = color(1.f, 1.f, 1.f, 1.f), draw_list* list = &background_draw_list) { list->draw_image(user_texture_id, min, max, uv_min, uv_max, clr); }
    void draw_image_quad(void* user_texture_id, std::array<vec2, 4> points, std::array<vec2, 4> uvs = { vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(0, 1) }, color clr = color(1.f, 1.f, 1.f, 1.f), draw_list* list = &background_draw_list) { list->draw_image_quad(user_texture_id, points, uvs, clr); }
    void draw_image_rounded(void* user_texture_id, vec2 min, vec2 max, vec2 uv_min, vec2 uv_max, color clr, float rounding, flags_list<corner_flags> rounding_corners = flags_list<corner_flags>(corner_flags::all), draw_list* list = &background_draw_list) { list->draw_image_rounded(user_texture_id, min, max, uv_min, uv_max, clr, rounding, rounding_corners); }
}