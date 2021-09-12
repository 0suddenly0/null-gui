#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include <map>

#include "../../math/math.h"
#include "../../utils/utils.h"
#include "../render/render.h"
#include "../input/input.h"

namespace null {
	namespace gui {
		namespace style {
			inline color main_color = color(83, 100, 255, 255);
			inline color window_bg = color(26, 26, 26, 255);
			inline color window_title_bg = color(40, 40, 40, 255);
			inline color group_bg = color(31, 31, 31, 255);
			inline color button_bg = color(44, 44, 44, 255);
			inline color button_bg_hovered = color(46, 46, 46, 255);
			inline color button_bg_active = color(48, 48, 48, 255);
			inline color popup_bg = color(33, 33, 33, 255);
			inline color text = color(255, 255, 255, 255);
			inline color text_hovered = color(130, 130, 130, 255);
			inline color select_text = color(100, 100, 255, 100);

			inline float window_title_size = 20.f;
			inline float window_title_line_size = 2.f;
			inline float item_spacing = 5.f;
			inline float text_spacing = 4.f;
			inline float checkbox_size = 16.f;
			inline float check_mark_size = 4.f;
			inline float checkbox_body_offset = 2.f;
			inline float combo_size = 16.f;
			inline float combo_arrow_size = 1.f;
			inline float combo_window_padding = 0.f;
			inline int max_auto_size_combo = 5;
			inline float slider_size = 5.f;
			inline float selectable_offset = 4.f;
			inline float selectable_active_offset = 10.f;
			inline float color_picker_size = 100.f;
			inline float color_picker_thickness = 9.f;
			inline float scrollbar_thickness = 2.f;
			inline float text_input_line_size = 2.f;
			inline float new_line_size = 16.f;
			inline float window_rounding = 0.f;
			inline float window_title_rounding = 0.f;
			inline float slider_rounding = 0.f;
			inline float checkbox_rounding = 0.f;
			inline float button_rounding = 0.f;
			inline float combo_rounding = 0.f;
			inline float scrollbar_rounding = 0.f;
			inline float color_picker_rounding = 0.f;
			inline float key_bind_rounding = 0.f;
			inline float text_input_rounding = 0.f;

			inline float show_pos_in_text_cooldown = 0.7f;

			inline vec2 text_input_padding = vec2(3, 0);
			inline vec2 button_padding = vec2(5, 1);
			inline vec2 window_padding = vec2(10, 10);
			inline vec2 scrollbar_padding = vec2(2, 2);

			inline bool items_size_full_window = true;
			inline bool checkbox_hovered_with_text = false;
			inline bool clamp_window_on_screen = true;
			inline bool move_window_on_title_bar = true;
			inline bool show_scrollbar_background = true;
		}

		enum class var_type {
			type_int = 0,
			type_string,
			type_float
		};

		enum class window_flags {
			standart = 0,
			group,
			no_title_bar,
			no_title_line,
			popup,
			no_move,
			set_size,
			set_pos,
			auto_size,
			no_padding
		};

		class window {
		public:
			window(std::string wnd_name, vec2 wnd_pos, vec2 wnd_size, flags_list<window_flags> wnd_flags);
			window* in_popup_region();
			window* get_hovered_group();
			window* get_main_window();
			void focus_window();
			void clamp_on_screen();
			void change_current_item() { draw_item_pos = last_draw_item_pos; }

			bool can_scroll() { return max_scroll != 0; }
			float get_scroll_offset() { return ignore_scroll ? 0.f : scroll_offset; }
			float get_scroll_thickness() { return can_scroll() ? style::scrollbar_padding.x + style::scrollbar_thickness : 0.f; }
			vec2 get_padding() { return flags.contains(window_flags::no_padding) ? vec2(0.f, 0.f) : style::window_padding; }
			vec2 get_window_size_with_padding();
			float get_title_size();
			bool can_open_tooltip();
			bool can_draw_item(rect item_rect);

			std::string name;
			vec2 pos;
			vec2 size = vec2(0, 0);
			vec2 arg_size;
			vec2 max_size;

			flags_list<window_flags> flags;

			vec2 drag_offset;
			bool dragging;

			vec2 draw_item_pos_prev;
			vec2 last_draw_item_pos;
			vec2 draw_item_pos;

			bool in_same_line = false;
			bool ignore_scroll = false;
			float scroll_offset = 0.f;
			float scroll_target = 0.f;
			float max_scroll = 0.f;
			float scroll_mouse_offset = 0.f;

			bool in_column = false;
			float column_offset = 0.f;

			int idx;
			bool visible = true;
			bool close_call = false;
			bool dont_add_item = false;

			window* parent_window = nullptr;
			std::vector<window*> child_popup_window;
			std::vector<window*> child_group_window;

			window* hovered_group = nullptr;

			std::shared_ptr<render::draw_list> draw_list;
		};

		enum class select_type {
			none = 0,
			down,
			all
		};

		namespace detail {
			class text_input_info {
			public:
				text_input_info(std::string _name, void* _value, rect _working_rect, var_type _type = var_type::type_string, std::string _format = "%.3f");

				static text_input_info* add(text_input_info input);
				static text_input_info* get_input(std::string name);
				static void win_poc(input::key_id key);
				static void add_char();
				static void control();

				void set_value(std::string new_value);
				std::string convert_value_to_string();

				void update_string_value();
				void update_value();

				void update_utf_text();
				void get_pos_on_cursor();
				void select_text();
				void clamp(int start = 0) {
					pos_in_text = math::clamp(pos_in_text, start, (int)string_value.size());
					select_max = math::clamp(select_max, start, (int)string_value.size());
					select_min = math::clamp(select_min, start, (int)string_value.size());
				}
				void reset_select() { selecting = select_type::none; select_max = 0; select_min = 0; };

				int get_id_under_cursor();
				float get_text_offset(int offset);

				std::string name;
				void* value;
				std::string last_value;
				std::string string_value;
				std::string value_for_render;
				std::wstring converted_value;
				rect working_rect;
				int pos_in_text;

				var_type type;
				std::string format; //format for convert float to string

				bool show_pos;
				float show_time;

				bool is_selecting() { return selecting != select_type::none && select_min != select_max; }
				select_type selecting;
				int select_min;
				int select_max;
			};

			class abstract_pushed_var {
			public:
				virtual void reset() = 0;
			};

			template <typename t>
			class pushed_var : public abstract_pushed_var {
			public:
				pushed_var(t* _var, t _new_value) : var(_var), old_value(*_var) { *var = _new_value; }
				void reset() { *var = old_value; }

				t* var;
				t old_value;
			};

			inline HWND hwnd;
			inline vec2 display_size;
			inline float delta_time;
			inline INT64 ticks_per_second;
			inline INT64 time;
			inline float real_time;
			inline window* current_window = nullptr;
			inline window* hovered_window = nullptr;
			inline std::string active_name;
			inline std::string hovered_name;
			inline std::string active_window_name;
			inline std::string last_item_name;
			inline std::vector<std::shared_ptr<window>> windows;
			inline std::vector<std::shared_ptr<abstract_pushed_var>> pushed_vars;
			inline std::vector<text_input_info> text_inputs;

			template <typename t>
			static void push_var(t* var, t new_value) { pushed_vars.push_back((std::shared_ptr<abstract_pushed_var>)(std::make_shared<pushed_var<t>>(var, new_value))); }
			static void pop_var() { pushed_vars.back()->reset(); pushed_vars.pop_back(); }

			namespace controls_behavior {
				bool button(rect size, bool* hovered, bool* pressed, std::string name);
				void color_picker(rect size, std::string name_item, std::string name, std::string tooltip, flags_list<window_flags> flags, bool alpha_bar);
				bool color_picker_sliders(rect size, std::string name);
				bool combo(rect size, bool* hovered, bool* pressed, std::string name, flags_list<window_flags>& flags);
				bool key_bind(input::bind_key* bind, rect size, bool* hovered, std::string name);
				void scroll(rect size, bool* hovered, bool* pressed);
				void slider(rect size, bool* hovered, bool* pressed, std::string name);
				bool text_input(rect size, bool* hovered, bool* pressed, std::string name);
			}

			std::string parse_format(std::string format);

			bool window_exist(std::string name);
			window* find_window(std::string name);
			window* add_window(std::string name, vec2 pos, vec2 size, flags_list<window_flags> flags);

			bool can_use_item(rect item_size, std::string item_name);
			void add_item(vec2 size, std::string name);
			std::string format_item(std::string text);

			static void set_active_item(std::string item_name) { active_name = item_name; }

			static bool mouse_in_current_window() { return hovered_window == (current_window->flags.contains(window_flags::group) ? current_window->get_main_window() : current_window); }
			static void close_current_window() { current_window->close_call = true; }
			void focus_current_window();
			void window_control();
			void popups_control();
		}

		void initialize(HWND hwnd);
		void shutdown();
		void begin_frame();

		bool begin_window(std::string name, vec2 pos, vec2 size, bool* open = nullptr);
		bool begin_window(std::string name, vec2 pos, vec2 size, flags_list<window_flags> flags = { }, bool* open = nullptr);
		void end_window();

		void begin_scroll();
		void end_scroll();

		void text(std::string text);
		bool button(std::string text, vec2 size_arg = vec2(0, 0));
		bool clickable_text(std::string text);
		void checkbox(std::string text, bool* value);
		void slider_int(std::string text, int* value, int min_value, int max_value, std::string format = "%d");
		void slider_float(std::string text, float* value, float min_value, float max_value, std::string format = "%.2f", int round = 0);
		void slider(std::string text, void* value, void* min_value, void* max_value, std::string format, int round, var_type type);
		void combo(std::string text, int* value, std::vector<std::string> items);
		void multi_combo(std::string text, std::vector<bool>* values, std::vector<std::string> items);
		void multi_combo(std::string text, std::vector<bool*> values, std::vector<std::string> items);
		bool selectable(std::string text, bool selected);
		color color_picker_sv(color clr, std::string name);
		float color_picker_slider_h(color clr, std::string name);
		float color_picker_slider_alpha(color clr, std::string name);
		void color_picker(std::string text, color* clr, bool alpha_bar = true);
		bool text_input(std::string text, void* value, bool password, var_type type, std::string format);
		bool text_input(std::string text, std::string* value, bool password = false);
		bool text_input(std::string text, int* value);
		bool text_input(std::string text, float* value, std::string format = "%.3f");
		void key_bind(std::string text, input::bind_key* bind);

		bool begin_group(std::string name, vec2 arg_size = vec2(0, 0));
		void end_group();

		void begin_columns(int count);
		void next_column();
		void end_columns();

		void new_line();
		void same_line();
		void tooltip(std::string tooltip_text);
		void functional_tooltip(std::function<void()> func);
	}
}