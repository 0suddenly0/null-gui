#pragma once
#include <stack>
#include "null-render/null-render.h"
#include "null-input/null-input.h"

namespace null_gui {
	enum window_flags {
		window_flag_standart = 0,
		window_flag_tooltip = 1 << 0,
		windpo_flag_big_title_bar = 1 << 1,
	};

	class window {
	public:
		rect get_draw_pos(rect value);
		std::string name;
		vec2 pos;
		vec2 size;
		vec2 drag_offset;
		bool dragging;
		vec2 draw_item_pos_prev;
		vec2 draw_item_pos;
		int idx;
		bool visible = true;
		null_render::null_draw_list draw_list;
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
			case gui_var_t::_int: {
				*var_int = var_int_old;
			} break;
			case gui_var_t::_float: {
				*var_float = var_float_old;
			} break;
			case gui_var_t::_bool: {
				*var_bool = var_bool_old;
			} break;
			case gui_var_t::_color: {
				*var_color = var_color_old;
			} break;
			case gui_var_t::_vec2: {
				*var_vec2 = var_vec2_old;
			} break;
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

	namespace deeps {
		HWND hwnd;
		vec2 display_size;
		float delta_time;
		INT64 ticks_per_second;
		INT64 time;
		window* current_window = nullptr;
		std::string active_name;
		std::vector<window*> windows;
		std::vector<gui_var> pushed_vars;

		window* find_window(std::string name);
		window* add_window(std::string name, vec2 pos, vec2 size);
		window* get_current_window();

		bool get_button_behavior(rect size, bool* hovered, bool* pressed, std::string name);
		void add_item(vec2 size); 
		bool mouse_in_current_windows();
		void set_display_size(vec2 size);
		vec2 get_display_size();
		std::string format_item(std::string text);
		void focus_current_window();
		void push_var(gui_var var);
		void pop_var();
		void window_moving();
	}

	namespace gui_settings {
		color main_color = color(100, 100, 255, 255);
		color window_bg = color(30, 30, 30, 255);
		color window_title_bg = color(50, 50, 50, 255);
		color button_bg = color(50, 50, 50, 255);
		color button_bg_hovered = color(60, 60, 60, 255);

		float window_title_size = 20.f;
		float item_spacing = 5.f;
		float text_checkbox_spacing = 4.f;
		float checkbox_size = 16.f;

		vec2 button_padding = vec2(5, 1);
		vec2 window_padding = vec2(10, 10);

		bool checkbox_hovered_with_text = false;
		bool clamp_window_on_screen = true;
		bool spacing_checkbox_size = true;
		bool move_window_on_title_bar = true;
	}

	void pre_begin_gui(HWND hwnd);
	void begin_gui();

	bool begin_window(std::string name, vec2 pos, vec2 size, bool* open = nullptr);
	void end_window();

	void text(std::string text);
	bool button(std::string text, vec2 size_arg = vec2(0, 0));
	void checkbox(std::string text, bool* var);

	void same_line();
}