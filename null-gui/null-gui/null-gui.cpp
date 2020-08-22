#include "null-gui.h"

namespace null_gui {
	vec2 window::get_draw_pos() {
		return vec2(std::clamp(draw_item_pos.x, pos.x, pos.x + size.x), std::clamp(draw_item_pos.y, pos.y + settings::window_title_size, pos.y + size.y));
	}

	namespace deeps {
		window* find_window(std::string name, int* back_idx) {
			for (int i = 0; i < windows.size(); i++) {
				window* wnd = windows[i];
				if (wnd->name == name) {
					if (back_idx != nullptr) *back_idx = i;
					return wnd;
				}
			}
			return nullptr;
		}

		window* add_window(std::string name, vec2 pos, vec2 size) {
			window* wnd = new window{ name, pos, size };
			if (windows.size() == 0) windows.push_back(wnd);
			else windows.insert(windows.begin(), wnd);
			return wnd;
		}

		window* get_current_window() {
			return current_window;
		}

		bool get_button_behavior(rect size, bool* hovered, bool* pressed, std::string name) {
			window* wnd = deeps::get_current_window();
			bool _active = false;
			bool _hovered = false;
			bool _pressed = false;

			if ((null_input::mouse_in_region(wnd->get_draw_pos(), size.max) && deeps::mouse_in_current_windows()) || active_name == name) {
				if(!null_input::get_mouse_key_state(0).down()) _hovered = true;
				else if (active_name == name) _pressed = true;
				if (null_input::get_mouse_key_state(0).clicked()) active_name = name;
				if (null_input::get_mouse_key_state(0).pressed()) _active = true;
			}

			if (!null_input::get_mouse_key_state(0).down() && active_name == name) {
				active_name = "";
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = _pressed;
			return _active;
		}

		void add_item(vec2 size) {
			window* wnd = get_current_window();
			wnd->draw_item_pos.y += size.y + settings::item_spacing;
		}

		bool mouse_in_current_windows() {
			for (int i = null_gui::deeps::windows.size() - 1; i >= 0; i--) {
				null_gui::window* wnd = null_gui::deeps::windows[i];
				if (wnd == null_gui::deeps::get_current_window()) return true;
				if (null_input::mouse_in_region(wnd->pos, wnd->pos + wnd->size)) return false;
			}
		}

		void set_display_size(vec2 size) {
			if (size != 0.f) display_size = size;
		}

		vec2 get_display_size() {
			return display_size;
		}
	}

	void pre_begin_gui(HWND hwnd) {
		deeps::hwnd = hwnd;
		::QueryPerformanceFrequency((LARGE_INTEGER*)&deeps::ticks_per_second);
		::QueryPerformanceCounter((LARGE_INTEGER*)&deeps::time);
	}

	void begin_gui() {
		RECT rect;
		::GetClientRect(deeps::hwnd, &rect);
		deeps::set_display_size(vec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top)));

		INT64 current_time;
		::QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
		deeps::delta_time = (float)(current_time - deeps::time) / deeps::ticks_per_second;
		deeps::time = current_time;

		null_input::update_keys_state();
	}
}