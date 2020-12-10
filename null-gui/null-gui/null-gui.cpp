#include "null-gui.h"

namespace null_gui {
	window::window(std::string wnd_name, vec2 wnd_pos, vec2 wnd_size, std::vector<window_flags> wnd_flags) {
		name = wnd_name;
		pos = wnd_pos;
		size = arg_size = wnd_size;
		flags = wnd_flags;

		if (null_gui::deeps::windows.size() != 0 && (have_flag(window_flags::group) || have_flag(window_flags::popup))) {
			parent_window = null_gui::deeps::windows.back();
		}

		draw_list = have_flag(window_flags::group) && parent_window ? get_main_window()->draw_list : new null_render::null_draw_list;
	}

	bool window::in_popup_region() {
		window* last_child = have_flag(window_flags::popup) ? this : child_popup_window;

		while (last_child != nullptr) {
			if (null_input::mouse_in_region(last_child->pos, last_child->pos + last_child->size)) return true;
			last_child = last_child->child_popup_window;
		}

		return false;
	}

	window* window::get_hovered_group() {
		if (child_group_window.size() <= 0) return nullptr;
		for (window* group : child_group_window) {
			if (null_input::mouse_in_region(group->pos, group->pos + group->size)) return group;
		}
		return nullptr;
	}

	window* window::get_main_window() {
		window* last = this;

		if (!last->parent_window) return nullptr;

		while (last->have_flag(window_flags::popup) || last->have_flag(window_flags::group)) {
			if (!last->parent_window) break;
			last = last->parent_window;
		}

		return last;
	}

	float window::get_scroll_offset() {
		if (!can_scroll()) return 0.f;

		return gui_settings::scrollbar_padding.x + gui_settings::scrollbar_thickness;
	}

	rect window::get_draw_pos(rect value) {
		vec2 clamped_draw_pos(null_math::clamp(draw_item_pos.x, pos.x, pos.x + size.x), null_math::clamp(draw_item_pos.y + get_scroll(), pos.y, pos.y + size.y));
		return rect(vec2(null_math::clamp(value.min.x, clamped_draw_pos.x, value.max.x), null_math::clamp(value.min.y, clamped_draw_pos.y, value.max.y)), value.max);
	}

	void window::focus_window() {
		if (this == deeps::windows.back()) return;
 		auto it = deeps::windows.begin() + idx;
		std::rotate(it, it + 1, deeps::windows.end());
	}

	namespace deeps {
		text_input_info* text_input_info::add(text_input_info* input) {
			text_input_info* finded = get_input(input->name);

			if (finded) {
				finded->name = input->name;
				finded->visible_text = input->visible_text;
				finded->work_rect = input->work_rect;
				return finded;
			}
			else text_inputs.push_back(input);

			return input;
		}

		text_input_info* text_input_info::get_input(std::string name) {
			for (text_input_info* a : text_inputs) if (a->name == name) return a;
			return nullptr;
		}

		void text_input_info::win_poc(int id) {
			text_input_info* active = get_input(active_name);
			if (!active) return;

			bool ctrl = null_input::get_key("ctrl")->down();
			
			if (null_input::key_names::for_input(id) && !ctrl) {
				if (!active->is_selecting()) {
					active->value->insert(active->value->begin() + active->pos_in_text, null_input::key_names::get_name(id, true).back());
				} else {
					active->value->erase(active->value->begin() + active->select_min, active->value->begin() + active->select_max);
					active->value->insert(active->value->begin() + active->select_min, null_input::key_names::get_name(id, true).back());
					active->pos_in_text = active->select_min;
					active->reset_select();
				}

				if (!active->is_selecting()) active->pos_in_text++;
				active->clamp();
			}

			if (null_input::get_key("backspace")->down()) {
				if (!active->is_selecting()) {
					if (active->pos_in_text > 0) {
						active->value->erase(active->value->begin() + active->pos_in_text - 1, active->value->begin() + active->pos_in_text);
						active->pos_in_text--;
						active->clamp();
					}
				} else {
					active->value->erase(active->value->begin() + active->select_min, active->value->begin() + active->select_max);
					active->pos_in_text = active->select_min;
					active->clamp();
				}
			}
			
			if (null_input::get_key("del")->down()) {
				if (!active->is_selecting()) {
					if (active->pos_in_text < active->value->size()) {
						active->value->erase(active->value->begin() + active->pos_in_text, active->value->begin() + active->pos_in_text + 1);
					}
				} else {
					active->value->erase(active->value->begin() + active->select_min, active->value->begin() + active->select_max);
					active->pos_in_text = active->select_min;
					active->clamp();
				}
			}

			if (ctrl) {
				if (null_input::get_key("left")->down() || null_input::get_key("right")->down()) {
					active->pos_in_text = null_input::get_key("left")->down() ? 0 : null_input::get_key("right")->down() ? active->value->size() : 0;
				}

				if (null_input::get_key("c")->down()) {
					null_input::write_clipboard(std::string(active->value->begin() + active->select_min, active->value->begin() + active->select_max));
					active->reset_select();
				}

				if (null_input::get_key("v")->down()) {
					if (active->is_selecting()) {
						active->value->erase(active->value->begin() + active->select_min, active->value->begin() + active->select_max);
						active->pos_in_text = active->select_min;
						active->reset_select();
					}

					active->value->insert(active->pos_in_text, null_input::read_clipboard());
					active->pos_in_text += null_input::read_clipboard().size();
				}
			} else {
				if (!active->is_selecting()) {
					active->pos_in_text += null_input::get_key("left")->down() ? -1 : null_input::get_key("right")->down() ? 1 : 0;
				} else {
					active->pos_in_text = null_input::get_key("left")->down() ? active->select_min : null_input::get_key("right")->down() ? active->select_max : active->pos_in_text;
					active->reset_select();
				}
			}

			active->clamp();
		}

		void text_input_info::control() {
			text_input_info* active = get_input(active_name);

			if (!active) {
				for (text_input_info* a : text_inputs) {
					a->get_pos_on_cursor();
					a->show_time = 0.f;
				}
			} else {
				if (deeps::real_time == 0.f) active->show_time = deeps::real_time;
				if (deeps::real_time - active->show_time >= gui_settings::show_pos_in_text_cooldown) {
					active->show_time = deeps::real_time;
					active->show_pos = !active->show_pos;
				}

				active->get_pos_on_cursor();
				active->select_text();
				active->clamp();
			}
		}

		void text_input_info::get_pos_on_cursor() {
			if (null_input::get_key("mouse left")->clicked()) {
				vec2 size = null_font::text_size(visible_text);
				if (!null_input::mouse_in_region(work_rect.min, work_rect.min + size)) {
					if (null_input::mouse_in_region(work_rect)) pos_in_text = visible_text.size();
					return;
				}

				std::string a = ""; a += visible_text[get_id_under_cursor()];
				float last_size = null_font::text_size(a).x;
				float centre_pos = work_rect.min.x + get_text_offset(get_id_under_cursor()) + (last_size / 2.f);

				pos_in_text = null_input::mouse_pos().x <= centre_pos ? get_id_under_cursor() : get_id_under_cursor() + 1;
			}
		}

		void text_input_info::select_text() {
			vec2 text_size = null_font::text_size(visible_text);
			if (null_input::get_key("mouse left")->down() && !null_input::mouse_in_region(work_rect.min, work_rect.max)) {
				reset_select();
				return;
			}

			if (null_input::get_key("mouse left")->clicked() && null_input::mouse_in_region(work_rect.min, work_rect.max)) {
				reset_select();
			}

			if (null_input::get_key("mouse left")->double_clicked() || (null_input::get_key("ctrl")->down() && null_input::get_key("a")->down())) {
				select_min = 0;
				select_max = visible_text.size();
				selecting = select_type::all;
			}

			if (null_input::get_key("mouse left")->down() && null_input::mouse_in_region(work_rect.min, work_rect.min + text_size) && selecting != select_type::all) {
				std::string a_t = ""; a_t += visible_text[get_id_under_cursor()];
				float last_size = null_font::text_size(a_t).x;
				float centre_pos = work_rect.min.x + get_text_offset(get_id_under_cursor()) + (last_size / 2.f);
				int b = null_input::mouse_pos().x <= centre_pos ? get_id_under_cursor() : get_id_under_cursor() + 1;
				int a = pos_in_text;

				if (a != b) {
					select_max = max(a, b);
					select_min = min(a, b);
					selecting = select_type::down;
				} else {
					reset_select();
				}
			}
		}

		int text_input_info::get_id_under_cursor() {
			int id = 0;
			vec2 size = null_font::text_size(visible_text);
			std::string temp = visible_text;
			for (int i = visible_text.size() - 1; i >= 0; i--) {
				if (null_input::mouse_in_region(work_rect.min, work_rect.min + size))
					id = i;

				temp.pop_back();
				size = null_font::text_size(temp);
			}
			return id;
		}

		float text_input_info::get_text_offset(int offset) {
			std::string text = std::string(visible_text.begin(), visible_text.begin() + null_math::clamp(offset, 0, (int)visible_text.size())) + ".";
			return null_font::text_size(text).x  - null_font::text_size(".").x;
		}

		float text_input_info::get_size_select() {
			clamp();
			std::string text = std::string(visible_text.begin() + select_min, visible_text.begin() + select_max);
			return null_font::text_size(text).x;
		}

		window* find_window(std::string name) {
			for (int i = 0; i < windows.size(); i++) {
				window* wnd = windows[i];
				wnd->idx = i;
				if (wnd->name == name)
					return wnd;
			}

			return nullptr;
		}

		window* add_window(std::string name, vec2 pos, vec2 size, std::vector<window_flags> flags) {
			window* wnd = new window(name, pos, size, flags);

			if (wnd->have_flag(window_flags::popup)) wnd->flags.push_back(window_flags::no_move);

			wnd->idx = wnd->have_flag(window_flags::group) ? 0 : windows.size();

			if (windows.size() == 0) windows.push_back(wnd);
			else {
				if (wnd->have_flag(window_flags::popup) || wnd->have_flag(window_flags::group)) {
					if(wnd->have_flag(window_flags::popup)) windows.back()->child_popup_window = wnd;
					else windows.back()->child_group_window.push_back(wnd);
				}
				windows.insert(wnd->have_flag(window_flags::group) ? windows.begin() : windows.end(), wnd);
			}

			return wnd;
		}

		bool text_input_behavior(rect size, bool* hovered, bool* pressed, std::string name) {
			window* wnd = deeps::current_window;
			bool _hovered = false;
			bool _pressed = false;

			if (hovered_name == "" || hovered_name == name) {
				if ((null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows()) || active_name == name) {
					hovered_name = name;

					if (null_input::mouse_in_region(wnd->draw_list->get_clip())) {
						if (!null_input::get_key("mouse left")->down() && null_input::mouse_in_region(wnd->get_draw_pos(size))) _hovered = true;
						if (null_input::get_key("mouse left")->clicked()) active_name = name;
					}
					if (active_name == name) {
						if (null_input::get_key("mouse left")->down()) _pressed = true;
					}
				}
			}

			if ((null_input::get_key("mouse left")->down() && (!null_input::mouse_in_region(wnd->get_draw_pos(size)) || !deeps::mouse_in_current_windows()) && active_name == name) || (null_input::get_key("enter")->down() && active_name == name)) {
				active_name = "";
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = _pressed;
			return active_name == name;
		}

		void get_scroll_behavior(rect size, bool* hovered, bool* pressed) {
			window* wnd = deeps::current_window;
			bool _hovered = false;
			bool _pressed = false;
			std::string name = utils::format("scrollbar##%s", wnd->name.c_str());

			if ((hovered_name == "" || hovered_name == name)) {
				if ((null_input::mouse_in_region(size) && deeps::mouse_in_current_windows()) || active_name == name) {
					hovered_name = name;

					if (null_input::mouse_in_region(wnd->draw_list->get_clip())) {
						if (!null_input::get_key("mouse left")->down()) _hovered = true;
						if (null_input::get_key("mouse left")->clicked()) active_name = name;
					}

					if (active_name == name && null_input::get_key("mouse left")->down()) _pressed = true;
				}
			}

			if (!null_input::get_key("mouse left")->down() && active_name == name) {
				active_name = "";
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = _pressed;
		}

		bool get_button_behavior(rect size, bool* hovered, bool* pressed, std::string name) {
			window* wnd = deeps::current_window;
			bool _active = false;
			bool _hovered = false;
			bool _pressed = false;

			if ((hovered_name == "" || hovered_name == name)) {
				if ((null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows()) || active_name == name) {
					hovered_name = name;

					if (null_input::mouse_in_region(wnd->draw_list->get_clip())) {
						if (!null_input::get_key("mouse left")->down()) _hovered = true;
						if (null_input::get_key("mouse left")->clicked()) active_name = name;
					}
					if (active_name == name) {
						if (null_input::get_key("mouse left")->down()) _pressed = true;
						if (null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows() && null_input::get_key("mouse left")->pressed()) _active = true;
					}
				}
			}

			if (!null_input::get_key("mouse left")->down() && active_name == name) {
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

					if (null_input::mouse_in_region(wnd->draw_list->get_clip())) {
						if (!null_input::get_key("mouse left")->down() && null_input::mouse_in_region(wnd->get_draw_pos(size))) _hovered = true;
						if (null_input::get_key("mouse left")->clicked()) active_name = name;
					}
					if (active_name == name) {
						if (null_input::get_key("mouse left")->down()) _pressed = true;
					}
				}
			}

			if (!null_input::get_key("mouse left")->down() && active_name == name) {
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
				if (null_input::mouse_in_region(wnd->draw_list->get_clip()) && deeps::mouse_in_current_windows() && null_input::mouse_in_region(wnd->get_draw_pos(size))) {
					hovered_name = name;

					if (!null_input::get_key("mouse left")->down()) _hovered = true;
					else _pressed = true;
					if (null_input::get_key("mouse left")->pressed()) deeps::add_window(name, vec2(size.min.x, size.max.y), vec2(size.max.x - size.min.x, 0.f), flags);
				}
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = _pressed;
			return _active;
		}

		void get_colorpicker_behavior(color* clr, rect size, std::string name_item, std::string name, std::string tooltip, std::vector<window_flags> flags, bool alpha_bar) {
			window* wnd = deeps::current_window;

			if (hovered_name == "" || hovered_name == name_item) {
				if (null_input::mouse_in_region(wnd->draw_list->get_clip()) && deeps::mouse_in_current_windows() && null_input::mouse_in_region(wnd->get_draw_pos(size))) {
					hovered_name = name_item;
					if (null_input::get_key("mouse left")->pressed()) deeps::add_window(name, vec2(size.max.x, size.min.y), vec2(0.f, 0.f), flags);
					if (null_input::get_key("mouse right")->pressed()) deeps::add_window(tooltip, vec2(size.max.x, size.min.y), vec2(0.f, 0.f), flags);
				}
			}
		}

		bool get_colorpicker_sliders_behavior(rect size, std::string name) {
			window* wnd = deeps::current_window;
			bool _pressed = false;

			if (hovered_name == "" || hovered_name == name) {
				if (null_input::mouse_in_region(wnd->get_draw_pos(size)) && deeps::mouse_in_current_windows() && null_input::mouse_in_region(wnd->get_draw_pos(size))) {
					hovered_name = name;

					if (null_input::get_key("mouse left")->down() && active_name == "") active_name = name;
				}
			}

			if (active_name == name) _pressed = true;

			if (!null_input::get_key("mouse left")->down() && active_name == name) {
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

			if (wnd->max_size.x < wnd->draw_item_pos_prev.x - deeps::current_window->pos.x) wnd->max_size.x = wnd->draw_item_pos_prev.x - deeps::current_window->pos.x;
			if (wnd->max_size.y < wnd->draw_item_pos.y - deeps::current_window->pos.y) wnd->max_size.y = wnd->draw_item_pos.y - deeps::current_window->pos.y;

			last_item_name = name;
		}

		bool mouse_in_current_windows() {
			return deeps::hovered_window == (deeps::current_window->have_flag(window_flags::group) ? deeps::current_window->get_main_window() : deeps::current_window);
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
			if(current_window->have_flag(window_flags::group)) current_window->get_main_window()->focus_window();
			else current_window->focus_window();
		}

		void window_control() {
			for (int i = deeps::windows.size() - 1; i >= 0; i--) { //getting hovered window and group
				window* wnd = deeps::windows[i];
				if (!wnd->visible) continue;

				if (!null_input::mouse_in_region(wnd->pos, wnd->pos + wnd->size)) {
					wnd->hovered_group = nullptr;
				} else {
					hovered_window = wnd->have_flag(window_flags::group) ? wnd->get_main_window() : wnd;
					hovered_window->hovered_group = hovered_window->get_hovered_group();
					break;
				}
			}

			if (null_input::get_key("mouse left")->clicked() && (active_name.empty() && active_window_name.empty())) { //window moving
				for (int i = windows.size() - 1; i >= 0; --i) {
					window* wnd = windows[i];

					if (wnd->have_flag(window_flags::no_move)) continue;

					if (!wnd->dragging && hovered_window == wnd && null_input::mouse_in_region(wnd->pos, wnd->pos + vec2(wnd->size.x, gui_settings::move_window_on_title_bar ? gui_settings::window_title_size : wnd->size.y)) && wnd->visible) {
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

				if (wnd->have_flag(window_flags::popup)) {
					if (deeps::find_window(wnd->parent_window->name) == nullptr || !wnd->parent_window->visible) {
						deeps::windows.erase(deeps::windows.begin() + wnd->idx);
						if (deeps::active_window_name == wnd->name) deeps::active_window_name = "";
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
		if(!IsIconic(deeps::hwnd)) deeps::display_size = vec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

		INT64 current_time;
		::QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
		deeps::delta_time = (float)(current_time - deeps::time) / deeps::ticks_per_second;
		deeps::time = current_time;
		deeps::real_time += deeps::delta_time;

		if (deeps::hovered_window && !null_input::mouse_in_region(deeps::hovered_window->pos, deeps::hovered_window->pos + deeps::hovered_window->size))
			deeps::hovered_window = nullptr;
		deeps::hovered_name = "";
		deeps::last_item_name = "";
		null_input::bind_control();
		null_input::update_keys_state();
		deeps::window_control();
		deeps::popups_control();
		deeps::text_input_info::control();
	}

	void same_line() {
		window* wnd = deeps::current_window;
		wnd->draw_item_pos = wnd->draw_item_pos_prev + vec2(gui_settings::item_spacing, 0.f);
	}

	void tooltip(std::function<void()> func, std::string need_name) {
		if (deeps::active_window_name != "") return;
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