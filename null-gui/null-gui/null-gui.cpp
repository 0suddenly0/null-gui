#include "null-gui.h"

namespace null_gui {
	rect window::get_draw_pos(rect value) {
		vec2 clamped_draw_pos(std::clamp(draw_item_pos.x, pos.x, pos.x + size.x), std::clamp(draw_item_pos.y, pos.y, pos.y + size.y));
		return rect(vec2(std::clamp(value.min.x, clamped_draw_pos.x, value.max.x), std::clamp(value.min.y, clamped_draw_pos.y, value.max.y)), value.max);
	}

	namespace deeps {
		window* find_window(std::string name) {
			for (int i = 0; i < windows.size(); i++) {
				window* wnd = windows[i];
				wnd->idx = i;
				if (wnd->name == name) {
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

			if ((null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows()) || active_name == name) {
				if (!null_input::get_mouse_key_state(0).down()) _hovered = true;
				if (null_input::get_mouse_key_state(0).clicked()) active_name = name;
				if(active_name == name) {
					if(null_input::get_mouse_key_state(0).down()) _pressed = true;
					if (null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows() && null_input::get_mouse_key_state(0).pressed()) _active = true;
				}
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
			wnd->draw_item_pos_prev = wnd->draw_item_pos + vec2(size.x, 0.f);
			wnd->draw_item_pos.x = wnd->pos.x + gui_settings::window_padding.x /*+ (gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_checkbox_spacing : 0.f)*/;
			wnd->draw_item_pos.y += size.y + gui_settings::item_spacing;
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

		std::string format_item(std::string text) {
			std::string ret;

			for (int i = 0; i < text.length(); i++) {
				if (text[i] == '#' && i + 1 < text.length() && text[i + 1] == '#') break;
				else ret.push_back(text[i]);
			}

			return ret;
		}

		void focus_current_window() {
			auto it = deeps::windows.begin() + get_current_window()->idx;
			std::rotate(it, it + 1, deeps::windows.end());
		}

		void push_var(gui_var var) {
			pushed_vars.push_back(var);
		}

		void pop_var() {
			pushed_vars.back().pop();
			pushed_vars.pop_back();
		}

		void window_moving() {
			if (null_input::get_mouse_key_state(0).clicked() && deeps::active_name.empty()) {
				for (int i = deeps::windows.size() - 1; i >= 0; --i) {
					window* wnd = deeps::windows[i];

					if (!wnd->dragging && null_input::mouse_in_region(wnd->pos, wnd->pos + vec2(wnd->size.x, gui_settings::move_window_on_title_bar ? gui_settings::window_title_size : wnd->size.y)) && wnd->visible) {
						wnd->drag_offset = null_input::mouse_pos() - wnd->pos;
						wnd->dragging = true;
						deeps::active_name = wnd->name;
						break;
					}
				}
			}
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
		deeps::window_moving();
	}
	
	void same_line() {
		window* wnd = deeps::get_current_window();
		wnd->draw_item_pos = wnd->draw_item_pos_prev + vec2(gui_settings::item_spacing, 0.f);
	}
}