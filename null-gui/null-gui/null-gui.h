#pragma once
#include <stack>
#include "../helpers/math.h"
#include "../null-render/null-render.h"
#include "../null-input/null-input.h"
#include "../utils/utils.h"

namespace null_gui {
	enum class window_flags {
		standart = 0,
		group,
		no_title_bar,
		popup,
		no_move,
		set_size,
		set_pos,
		auto_size
	};

	class window {
	public:
		window(std::string wnd_name, vec2 wnd_pos, vec2 wnd_size, std::vector<window_flags> wnd_flags);
		bool have_flag(window_flags flag) { return std::count(flags.begin(), flags.end(), flag) > 0; }
		bool in_popup_region();
		window* get_hovered_group();
		window* get_main_window();
		float get_scroll() { return ignore_scroll ? 0.f : scroll_offset; }
		bool can_scroll() { return max_scroll != 0; }
		float get_scroll_offset();
		void focus_window();
		rect get_draw_pos(rect value);

		std::string name;
		vec2 pos;
		vec2 size = vec2(0, 0);
		vec2 arg_size;
		vec2 max_size;

		std::vector<window_flags> flags;

		vec2 drag_offset;
		bool dragging;

		vec2 draw_item_pos_prev;
		vec2 draw_item_pos;

		bool ignore_scroll;
		float scroll_offset;
		float scroll_target;
		float max_scroll;
		float scroll_mouse_offset;

		float column_offset;

		int idx;
		bool visible = true;
		bool close_call;

		window* parent_window = nullptr;
		window* child_popup_window = nullptr;
		std::vector<window*> child_group_window;

		window* hovered_group = nullptr;

		null_render::draw_list* draw_list;
	};

	enum class gui_var_t {
		_int = 0,
		_float = 1,
		_bool = 2,
		_color = 3,
		_vec2 = 4
	};

	class gui_var {
	public:
		gui_var(int* var, int new_value) {
			type = gui_var_t::_int;
			var_int = (int*)var;
			var_int_old = *var_int;
			*var_int = new_value;
		}

		gui_var(float* var, float new_value) {
			type = gui_var_t::_float;
			var_float = var;
			var_float_old = *var_float;
			*var_float = new_value;
		}

		gui_var(bool* var, bool new_value) {
			type = gui_var_t::_bool;
			var_bool = (bool*)var;
			var_bool_old = *var_bool;
			*var_bool = new_value;
		}

		gui_var(color* var, color new_value) {
			type = gui_var_t::_color;
			var_color = (color*)var;
			var_color_old = *var_color;
			*var_color = new_value;
		}

		gui_var(vec2* var, vec2 new_value) {
			type = gui_var_t::_vec2;
			var_vec2 = (vec2*)var;
			var_vec2_old = *var_vec2;
			*var_vec2 = new_value;
		}

		void pop() {
			switch (type) {
			case gui_var_t::_int:   *var_int = var_int_old;     break;
			case gui_var_t::_float: *var_float = var_float_old; break;
			case gui_var_t::_bool:  *var_bool = var_bool_old;   break;
			case gui_var_t::_color: *var_color = var_color_old; break;
			case gui_var_t::_vec2:  *var_vec2 = var_vec2_old;   break;
			}
		}

		gui_var_t type;

		int* var_int;
		int var_int_old;
		float* var_float;
		float var_float_old;
		bool* var_bool;
		bool var_bool_old;
		color* var_color;
		color var_color_old;
		vec2* var_vec2;
		vec2 var_vec2_old;
	};

	enum class select_type {
		none = 0,
		down,
		all
	};

	namespace deeps {
		class text_input_info {
		public:
			static text_input_info* add(text_input_info* input);
			static text_input_info* get_input(std::string name);
			static void win_poc(int id);
			static void control();

			void get_pos_on_cursor();
			void select_text();
			void clamp(int start = 0) {
				pos_in_text = math::clamp(pos_in_text, start, (int)value->size());
				select_max = math::clamp(select_max, start, (int)value->size());
				select_min = math::clamp(select_min, start, (int)value->size());
			}
			void reset_select() { selecting = select_type::none; select_max = 0; select_min = 0; };

			int get_id_under_cursor();
			float get_text_offset(int offset);
			float get_size_select();

			std::string name;
			std::string* value;
			std::string visible_text;
			rect work_rect;
			int pos_in_text;
			
			bool show_pos;
			float show_time;

			bool is_selecting() { return selecting != select_type::none && select_min != select_max; }
			select_type selecting;
			int select_min;
			int select_max;
		};

		std::vector<text_input_info*> text_inputs;
		HWND hwnd;
		vec2 display_size;
		float delta_time;
		INT64 ticks_per_second;
		INT64 time;
		float real_time;
		window* current_window = nullptr;
		window* hovered_window = nullptr;
		std::string active_name;
		std::string hovered_name;
		std::string active_window_name;
		std::string last_item_name;
		std::vector<window*> windows;
		std::vector<gui_var> pushed_vars;

		window* find_window(std::string name);
		window* add_window(std::string name, vec2 pos, vec2 size, std::vector<window_flags> flags);

		bool text_input_behavior(rect size, bool* hovered, bool* pressed, std::string name);
		void scroll_behavior(rect size, bool* hovered, bool* pressed);
		bool key_bind_behavior(null_input::bind_key* bind, rect size, bool* hovered, std::string name);
		bool button_behavior(rect size, bool* hovered, bool* pressed, std::string name);
		void slider_behavior(rect size, bool* hovered, bool* pressed, std::string name);
		bool combo_behavior(rect size, int item_count, bool* hovered, bool* pressed, std::string name, std::vector<window_flags> flags);
		void colorpicker_behavior(color* clr, rect size, std::string name_item, std::string name, std::string tooltip, std::vector<window_flags> flags, bool alpha_bar);
		bool colorpicker_sliders_behavior(rect size, std::string name);
		void add_item(vec2 size, std::string name); 
		bool mouse_in_current_windows();
		std::string format_item(std::string text);
		void focus_current_window();
		void close_current_window();
		void push_var(gui_var var) { pushed_vars.push_back(var); }
		void pop_var() { pushed_vars.back().pop(); pushed_vars.pop_back(); }
		void window_control();
		void popups_control();
	}

	namespace gui_settings {
		color main_color = color(83, 100, 255, 255);
		color window_bg = color(26, 26, 26, 255);
		color window_title_bg = color(40, 40, 40, 255);
		color group_bg = color(31, 31, 31, 255);
		color button_bg = color(44, 44, 44, 255);
		color button_bg_hovered = color(46, 46, 46, 255);
		color button_bg_active = color(48, 48, 48, 255);
		color popup_bg = color(33, 33, 33, 255);
		color text = color(255, 255, 255, 255);
		color text_hovered = color(130, 130, 130, 255);
		color select_text = color(100, 100, 255, 100);

		float window_title_size = 20.f;
		float item_spacing = 5.f;
		float text_spacing = 4.f;
		float checkbox_size = 16.f;
		float combo_size = 16.f;
		float slider_size = 5.f;
		float selectable_offset = 4.f;
		float selectable_active_offset = 10.f;
		float colorpicker_size = 100.f;
		float colorpicker_thickness = 9.f;
		float scrollbar_thickness = 2.f;

		float double_click_time = 0.30f;
		float double_click_max_dist = 6.f;
		float show_pos_in_text_cooldown = 0.7f;

		vec2 button_padding = vec2(5, 1);
		vec2 window_padding = vec2(10, 10);
		vec2 scrollbar_padding = vec2(2, 2);

		bool items_size_full_window = true;
		bool checkbox_hovered_with_text = false;
		bool clamp_window_on_screen = true;
		bool spacing_checkbox_size = true;
		bool move_window_on_title_bar = true;
	}

	void pre_begin_gui(HWND hwnd);
	void begin_gui();

	bool begin_window(std::string name, vec2 pos, vec2 size, bool* open = nullptr);
	bool begin_window(std::string name, vec2 pos, vec2 size, std::vector<window_flags> flags = { window_flags::standart }, bool* open = nullptr);
	void end_window();

	void begin_scroll();
	void end_scroll();

	void text(std::string text);
	bool button(std::string text, vec2 size_arg = vec2(0, 0));
	bool clickable_text(std::string text);
	void checkbox(std::string text, bool* value);
	void slider_int(std::string text, int* value, int min, int max, std::string format = "%d", int step = 1, int ctrl_step = 5);
	void slider_float(std::string text, float* value, float min, float max, std::string format = "%.2f", float step = 0.1f, float ctrl_step = 0.5f);
	void combo(std::string text, int* value, std::vector<std::string> items);
	void multicombo(std::string text, std::vector<bool>* values, std::vector<std::string> items);
	void multicombo(std::string text, std::vector<bool*> values, std::vector<std::string> items);
	bool selectable(std::string text, bool selected);
	color colorpicker_sv(color clr, std::string name);
	float colorpicker_slider_h(color clr, std::string name);
	float colorpicker_slider_alpha(color clr, std::string name);
	void colorpicker(std::string text, color* clr, bool alpha_bar = true);
	bool text_input(std::string text, std::string* value, bool password = false);
	void key_bind(std::string text, null_input::bind_key* bind);
	
	bool begin_group(std::string name, vec2 arg_size = vec2(0, 0));
	void end_group();

	void begin_columns(int count);
	void next_column();
	void end_columns();

	void same_line();
	void tooltip(std::function<void()> func, std::string need_name = "");
}