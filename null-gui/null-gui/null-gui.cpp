#include "null-gui.h"

namespace null_gui {
	rect window::get_draw_pos(rect value) {
		vec2 clamped_draw_pos(null_math::clamp(draw_item_pos.x, pos.x, pos.x + size.x), null_math::clamp(draw_item_pos.y + scroll_offset, pos.y, pos.y + size.y));
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
					hovered_name = name;

					if (!null_input::get_mouse_key_state(0).down()) _hovered = true;
					if (null_input::get_mouse_key_state(0).clicked()) active_name = name;
					if (active_name == name) {
						if (null_input::get_mouse_key_state(0).down()) _pressed = true;
						if (null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows() && null_input::get_mouse_key_state(0).pressed()) _active = true;
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
					hovered_name = name;

					if (!null_input::get_mouse_key_state(0).down() && null_input::mouse_in_region(wnd->get_draw_pos(size))) _hovered = true;
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
					hovered_name = name;

					if (!null_input::get_mouse_key_state(0).down())	_hovered = true;
					else _pressed = true;
					if (null_input::get_mouse_key_state(0).pressed()) deeps::add_window(name, vec2(size.min.x, size.max.y), vec2(size.max.x - size.min.x, 0.f), flags);
				}
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = _pressed;
			return _active;
		}

		void get_colorpicker_behavior(color* clr, rect size, std::string name_item, std::string name, std::string tooltip, std::vector<window_flags> flags, bool alpha_bar) {
			window* wnd = deeps::current_window;

			if (hovered_name == "" || hovered_name == name_item) {
				if (null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows()) {
					hovered_name = name_item;
					if (null_input::get_mouse_key_state(0).pressed()) deeps::add_window(name, vec2(size.max.x, size.min.y), vec2(0.f, 0.f), flags);
					if (null_input::get_mouse_key_state(1).pressed()) deeps::add_window(tooltip, vec2(size.max.x, size.min.y), vec2(0.f, 0.f), flags);
				}
			}
		}

		bool get_colorpicker_sliders_behavior(rect size, std::string name) {
			window* wnd = deeps::current_window;
			bool _pressed = false;

			if (hovered_name == "" || hovered_name == name) {
				if (null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows()) {
					hovered_name = name;

					if (null_input::get_mouse_key_state(0).down() && active_name == "") active_name = name;
				}
			}

			if (active_name == name) _pressed = true;

			if (!null_input::get_mouse_key_state(0).down() && active_name == name) {
				active_name = "";
			}

			return _pressed;
		}

		void add_item(vec2 size, std::string name) {
			window* wnd = current_window;
			size += vec2(gui_settings::item_spacing, 0.f);
			wnd->draw_item_pos_prev = wnd->draw_item_pos + vec2(size.x, 0.f);
			wnd->draw_item_pos.x = wnd->pos.x + (wnd->column_offset == 0.f ? gui_settings::window_padding.x : wnd->column_offset);
			wnd->draw_item_pos.y += size.y + gui_settings::item_spacing;

			if (wnd->max_size.x < wnd->draw_item_pos_prev.x - deeps::current_window->pos.x + gui_settings::item_spacing) wnd->max_size.x = wnd->draw_item_pos_prev.x - deeps::current_window->pos.x + gui_settings::item_spacing;
			if (wnd->max_size.y < wnd->draw_item_pos.y - deeps::current_window->pos.y) wnd->max_size.y = wnd->draw_item_pos.y - deeps::current_window->pos.y;

			last_item_name = name;
		}

		bool mouse_in_current_windows() {
			return deeps::hovered_window == deeps::current_window;
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

		void window_control() {
			for (int i = deeps::windows.size() - 1; i >= 0; i--) { //getting hovered window
				window* wnd = deeps::windows[i];
				if (!wnd->visible) continue;
				if (null_input::mouse_in_region(wnd->pos, wnd->pos + wnd->size)) {
					hovered_window = wnd;
					break;
				}
			}

			if (null_input::get_mouse_key_state(0).clicked() && (active_name.empty() && active_window_name.empty())) { //window moving
				for (int i = windows.size() - 1; i >= 0; --i) {
					window* wnd = windows[i];

					if (wnd->have_flag(window_flags::no_move)) continue;

					if (!wnd->dragging && null_input::mouse_in_region(wnd->pos, wnd->pos + vec2(wnd->size.x, gui_settings::move_window_on_title_bar ? gui_settings::window_title_size : wnd->size.y)) && wnd->visible) {
						wnd->drag_offset = null_input::mouse_pos() - wnd->pos;
						wnd->dragging = true;
						active_window_name = wnd->name;
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
		if(!IsIconic(deeps::hwnd)) deeps::display_size = vec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

		INT64 current_time;
		::QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
		deeps::delta_time = (float)(current_time - deeps::time) / deeps::ticks_per_second;
		deeps::time = current_time;

		deeps::hovered_name = "";
		deeps::last_item_name = "";
		null_input::update_keys_state();
		deeps::window_control();
		deeps::popups_control();
	}

	void same_line() {
		window* wnd = deeps::current_window;
		wnd->draw_item_pos = wnd->draw_item_pos_prev + vec2(gui_settings::item_spacing, 0.f);
	}

	void tooltip(std::function<void()> func, std::string need_name) {
		bool close = deeps::hovered_name == deeps::last_item_name;
		deeps::push_var({ &gui_settings::spacing_checkbox_size, false }); {
				if (begin_window("##tooltip", null_input::mouse_pos() + vec2(10, 10), vec2(0.f, 0.f), { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size }, &close)) {
					deeps::focus_current_window();
					func();
					end_window();
				}
		} deeps::pop_var();
	}
}