#pragma once
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
		vec2 get_draw_pos();
		std::string name;
		vec2 pos;
		vec2 size;
		vec2 drag_offset;
		bool dragging;
		vec2 draw_item_pos;
		int idx;
		bool visible;
		null_render::null_draw_list draw_list;
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
	}

	namespace gui_settings {
		color main_color = color(100, 100, 255, 255);
		color window_bg = color(30, 30, 30, 255);
		color window_title_bg = color(50, 50, 50, 255);
		color button_bg = color(50, 50, 50, 255);
		color button_bg_hovered = color(60, 60, 60, 255);

		float window_title_size = 20.f;
		float item_spacing = 5.f;
		float check_box_size = 10.f;

		vec2 button_padding = vec2(5, 1);
		vec2 window_padding = vec2(10, 10);

		bool clamp_window_on_screen = true;
		bool spacing_check_box_size = true;
		bool move_window_on_title_bar = true;
	}

	void pre_begin_gui(HWND hwnd);
	void begin_gui();

	bool begin_window(std::string name, vec2 pos, vec2 size, bool* open);
	void end_window();

	void text(std::string text);
	bool button(std::string text, vec2 size_arg = vec2(0, 0));
	//void check_box(std::string text, bool& var);
}