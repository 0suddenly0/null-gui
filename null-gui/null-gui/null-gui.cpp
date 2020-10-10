#include "null-gui.h"

namespace null_gui {
	rect window::get_draw_pos(rect value) {
		vec2 clamped_draw_pos(null_math::clamp(draw_item_pos.x, pos.x, pos.x + size.x), null_math::clamp(draw_item_pos.y, pos.y, pos.y + size.y));
		return rect(vec2(null_math::clamp(value.min.x, clamped_draw_pos.x, value.max.x), null_math::clamp(value.min.y, clamped_draw_pos.y, value.max.y)), value.max);
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

		window* add_window(std::string name, vec2 pos, vec2 size, std::vector<window_flags> flags) {
			window* wnd = new window(name, pos, size, flags);

			if (wnd->have_flag(window_flags::popup)) wnd->flags.push_back(window_flags::no_move);

			wnd->idx = windows.size();

			if (windows.size() == 0) windows.push_back(wnd);
			else {
				if (wnd->have_flag(window_flags::popup)) {
					wnd->parent_window = windows.back();
					windows.back()->child_window = wnd;
				}
				windows.insert(windows.end(), wnd);
			}

			return wnd;
		}

		bool get_button_behavior(rect size, bool* hovered, bool* pressed, std::string name) {
			window* wnd = deeps::current_window;
			bool _active = false;
			bool _hovered = false;
			bool _pressed = false;

			if (hovered_name == "" || hovered_name == name) {
				if ((null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows()) || active_name == name) {
					if (!null_input::get_mouse_key_state(0).down()) {
						_hovered = true;
						hovered_name = name;
					}
					if (null_input::get_mouse_key_state(0).clicked()) active_name = name;
					if (active_name == name) {
						if (null_input::get_mouse_key_state(0).down()) _pressed = true;
						if (null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows() && null_input::get_mouse_key_state(0).pressed()) _active = true;
						hovered_name = name;
					}
				}
			}

			if (!null_input::get_mouse_key_state(0).down() && active_name == name) {
				active_name = "";
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = _pressed;
			return _active;
		}

		void get_slider_behavior(rect size, bool* hovered, bool* pressed, std::string name) {
			window* wnd = deeps::current_window;
			bool _hovered = false;
			bool _pressed = false;

			if (hovered_name == "" || hovered_name == name) {
				if ((null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows()) || active_name == name) {
					if (!null_input::get_mouse_key_state(0).down() && null_input::mouse_in_region(wnd->get_draw_pos(size))) {
						_hovered = true;
						hovered_name = name;
					}
					if (null_input::get_mouse_key_state(0).clicked()) active_name = name;
					if (active_name == name) {
						if (null_input::get_mouse_key_state(0).down()) _pressed = true;
					}
				}
			}

			if (!null_input::get_mouse_key_state(0).down() && active_name == name) {
				active_name = "";
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = _pressed;
		}

		bool get_combo_behavior(rect size, int item_count, bool* hovered, bool* pressed, std::string name, std::vector<window_flags> flags) {
			window* wnd = deeps::current_window;
			bool _active = false;
			bool _hovered = false;
			bool _pressed = false;
			int max_size = item_count * null_font::text_size(name).y;

			if (hovered_name == "" || hovered_name == name) {
				if (null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows()) {
					if (!null_input::get_mouse_key_state(0).down()) {
						_hovered = true;
						hovered_name = name;
					}
					if (null_input::get_mouse_key_state(0).pressed()) {
						deeps::add_window(name, vec2(size.min.x, size.max.y), vec2(size.max.x - size.min.x, 0.f), flags);
						hovered_name = name;
					}
				}
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = _pressed;
			return _active;
		}

		void add_item(vec2 size) {
			window* wnd = current_window;
			wnd->draw_item_pos_prev = wnd->draw_item_pos + vec2(size.x, 0.f);
			wnd->draw_item_pos.x = wnd->pos.x + gui_settings::window_padding.x;
			wnd->draw_item_pos.y += size.y + gui_settings::item_spacing;
		}

		bool mouse_in_current_windows() {
			for (int i = null_gui::deeps::windows.size() - 1; i >= 0; i--) {
				null_gui::window* wnd = null_gui::deeps::windows[i];
				if (wnd == null_gui::deeps::current_window) return true;
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
			auto it = deeps::windows.begin() + current_window->idx;
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
			if (null_input::get_mouse_key_state(0).clicked() && (deeps::active_name.empty() && deeps::active_window_name.empty())) {
				for (int i = deeps::windows.size() - 1; i >= 0; --i) {
					window* wnd = deeps::windows[i];

					if (wnd->have_flag(window_flags::no_move)) continue;

					if (!wnd->dragging && null_input::mouse_in_region(wnd->pos, wnd->pos + vec2(wnd->size.x, gui_settings::move_window_on_title_bar ? gui_settings::window_title_size : wnd->size.y)) && wnd->visible) {
						wnd->drag_offset = null_input::mouse_pos() - wnd->pos;
						wnd->dragging = true;
						deeps::active_window_name = wnd->name;
						break;
					}
				}
			}
		}

		void popups_control() {
			for (int i = 0; i < deeps::windows.size(); i++) {
				window* wnd = deeps::windows[i];

				if (!wnd->have_flag(window_flags::popup)) continue;
				if (deeps::find_window(wnd->parent_window->name) == nullptr || !wnd->parent_window->visible) {
					deeps::windows.erase(deeps::windows.begin() + wnd->idx);
					if (deeps::active_window_name == wnd->name) deeps::active_window_name = "";
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

		deeps::hovered_name = "";
		null_input::update_keys_state();
		deeps::window_moving();
		deeps::popups_control();
	}

	void same_line() {
		window* wnd = deeps::current_window;
		wnd->draw_item_pos = wnd->draw_item_pos_prev + vec2(gui_settings::item_spacing, 0.f);
	}
}