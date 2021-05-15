#include "null-gui.h"

namespace null_gui {
	window::window(std::string wnd_name, vec2 wnd_pos, vec2 wnd_size, std::vector<window_flags> wnd_flags) {
		name = wnd_name;
		pos = wnd_pos;
		size = arg_size = wnd_size;
		flags = wnd_flags;

		if (deeps::current_window && (flags.contains(window_flags::group) || flags.contains(window_flags::popup))) {
			parent_window = deeps::current_window;
		}

		draw_list = flags.contains(window_flags::group) && parent_window ? get_main_window()->draw_list : new null_render::draw_list(&null_render::shared_data);
		draw_list->_reset_for_begin_render();
		draw_list->push_texture_id(null_font::vars::font_atlas->tex_id);
		draw_list->push_clip_rect_full_screen();
	}

	window* window::in_popup_region() {
		if (!parent_window) return nullptr;
		if (parent_window->child_popup_window.size() <= 0) return nullptr;
		for (window* group : parent_window->child_popup_window) {
			if (null_input::mouse_in_region(group->pos, group->pos + group->size)) return group;
		}
		return nullptr;
	}

	window* window::get_hovered_group() {
		if (child_group_window.size() <= 0) return nullptr;
		for (window* group : child_group_window) {
			if (group->get_hovered_group() != nullptr) return group->get_hovered_group();
			if (null_input::mouse_in_region(group->pos, group->pos + group->size)) return group;
		}
		return nullptr;
	}

	window* window::get_main_window() {
		window* last = this;

		if (!last->parent_window) return last;

		while (last->flags.contains(window_flags::popup) || last->flags.contains(window_flags::group)) {
			if (!last->parent_window) break;
			last = last->parent_window;
		}

		return last;
	}

	void window::focus_window() {
		if (this == deeps::windows.back()) return;
		auto it = deeps::windows.begin() + idx;
		std::rotate(it, it + 1, deeps::windows.end());
	}

	void window::clamp_on_screen() {
		if (gui_settings::clamp_window_on_screen && !flags.contains(window_flags::group)) {
			pos.x = math::clamp(pos.x, 0.f, deeps::display_size.x - size.x);
			pos.y = math::clamp(pos.y, 0.f, deeps::display_size.y - size.y);
		}
	}

	float window::get_scroll_thickness() {
		return can_scroll() ? gui_settings::scrollbar_padding.x + gui_settings::scrollbar_thickness : 0.f;
	}

	float window::get_window_size_with_padding() {
		return size.x - (gui_settings::window_padding.x * 2) - get_scroll_thickness();
	}

	float window::get_title_size() {
		float result = 0.f;
		result += (flags.contains(window_flags::no_title_bar) || flags.contains(window_flags::popup) ? 0.f : gui_settings::window_title_size);
		result += (flags.contains(window_flags::no_title_line) ? 0.f : gui_settings::window_title_line_size);
		return result;
	}

	bool window::can_open_tooltip() {
		if (child_popup_window.size() > 1) return false; 
		else if (child_popup_window.size() == 1 && child_popup_window.back()->name == "##tooltip") return true;
		else if (child_popup_window.size() == 0) return true;
		return false;
	}

	bool window::can_draw_item(rect item_rect) {
		bool y_top = item_rect.min.y > pos.y && item_rect.min.y < pos.y + size.y;
		bool y_bottom = item_rect.max.y > pos.y && item_rect.max.y < pos.y + size.y;

		return y_top || y_bottom;
	}

	rect window::get_draw_pos(rect value) {
		vec2 clamped_draw_pos(math::clamp(draw_item_pos.x, pos.x, pos.x + size.x), math::clamp(draw_item_pos.y + get_scroll_offset(), pos.y, pos.y + size.y));
		return rect(vec2(math::clamp(value.min.x, clamped_draw_pos.x, value.max.x), math::clamp(value.min.y, clamped_draw_pos.y, value.max.y)), value.max);
	}

	namespace deeps {
		text_input_info* text_input_info::add(text_input_info input) {
			text_input_info* finded = get_input(input.name);

			if (finded) {
				finded->name = input.name;
				finded->working_rect = input.working_rect;
				return finded;
			} else
				text_inputs.push_back(input);

			return &text_inputs.back();
		}

		text_input_info* text_input_info::get_input(std::string name) {
			for (text_input_info& a : text_inputs) if (a.name == name) return &a;
			return nullptr;
		}

		void text_input_info::win_poc(int id) {
			text_input_info* active = get_input(active_name);
			if (!active) return;

			if (null_input::get_key(null_input::key_id::backspace)->down()) {
				if (!active->is_selecting()) {
					if (active->pos_in_text > 0) {
						active->string_value.erase(active->string_value.begin() + active->pos_in_text - 1, active->string_value.begin() + active->pos_in_text);
						active->pos_in_text--;
						active->clamp();
					}
				} else {
					active->string_value.erase(active->string_value.begin() + active->select_min, active->string_value.begin() + active->select_max);
					active->pos_in_text = active->select_min;
					active->clamp();
				}
			}
			
			if (null_input::get_key(null_input::key_id::del)->down()) {
				if (!active->is_selecting()) {
					if (active->pos_in_text < active->string_value.size()) {
						active->string_value.erase(active->string_value.begin() + active->pos_in_text, active->string_value.begin() + active->pos_in_text + 1);
					}
				} else {
					active->string_value.erase(active->string_value.begin() + active->select_min, active->string_value.begin() + active->select_max);
					active->pos_in_text = active->select_min;
					active->clamp();
				}
			}

			if (null_input::get_key(null_input::key_id::ctrl)->down()) {
				if (null_input::get_key(null_input::key_id::left)->down() || null_input::get_key(null_input::key_id::right)->down()) {
					active->pos_in_text = null_input::get_key(null_input::key_id::left)->down() ? 0 : null_input::get_key(null_input::key_id::right)->down() ? active->string_value.size() : 0;
				}

				if (null_input::get_key(null_input::key_id::c)->down()) {
					null_input::write_clipboard(std::string(active->string_value.begin() + active->select_min, active->string_value.begin() + active->select_max));
					active->reset_select();
				}

				if (null_input::get_key(null_input::key_id::v)->down()) {
					if (active->is_selecting()) {
						active->string_value.erase(active->string_value.begin() + active->select_min, active->string_value.begin() + active->select_max);
						active->pos_in_text = active->select_min;
						active->reset_select();
					}

					active->string_value.insert(active->pos_in_text, null_input::read_clipboard());
					active->pos_in_text += null_input::read_clipboard().size();
				}
			} else {
				if (!active->is_selecting()) {
					active->pos_in_text += null_input::get_key(null_input::key_id::left)->down() ? -1 : null_input::get_key(null_input::key_id::right)->down() ? 1 : 0;
				} else {
					active->pos_in_text = null_input::get_key(null_input::key_id::left)->down() ? active->select_min : null_input::get_key(null_input::key_id::right)->down() ? active->select_max : active->pos_in_text;
					active->reset_select();
				}
			}

			active->update_value();
			active->update_utf_text();
			active->clamp();
		}

		void text_input_info::add_char() {
			text_input_info* active = get_input(active_name);
			if (!active) return;
			
			if (null_input::key_data::char_allowed(null_input::last_symbol) && !null_input::get_key(null_input::key_id::ctrl)->down()) {
				std::string value_for_check = active->string_value;
				char ascii_char = null_font::helpers::utf8_to_ascii(null_input::last_symbol);
				value_for_check.insert(value_for_check.begin() + active->pos_in_text, ascii_char);
				bool can_write = null_font::text_size(null_font::helpers::convert_utf8(value_for_check).c_str()).x < active->working_rect.size().x;

				if (!active->is_selecting()) {
					if (can_write) active->string_value = value_for_check;
				} else {
					active->string_value.erase(active->string_value.begin() + active->select_min, active->string_value.begin() + active->select_max);
					active->string_value.insert(value_for_check.begin() + active->select_min, ascii_char);
					active->pos_in_text = active->select_min;
					active->reset_select();
				}

				if (!active->is_selecting() && can_write) active->pos_in_text++;
				active->clamp();
			}

			active->update_value();
			active->update_utf_text();
			active->clamp();
		}

		void text_input_info::control() {
			text_input_info* active_info = get_input(active_name);

			if (!active_info) {
				for (text_input_info& info : text_inputs) {
					info.update_string_value();
					info.update_utf_text();
					info.get_pos_on_cursor();
					info.show_time = 0.f;
				}
			} else {
				if (deeps::real_time == 0.f) active_info->show_time = deeps::real_time;
				if (deeps::real_time - active_info->show_time >= gui_settings::show_pos_in_text_cooldown) {
					active_info->show_time = deeps::real_time;
					active_info->show_pos = !active_info->show_pos;
				}

				active_info->update_utf_text();
				active_info->get_pos_on_cursor();
				active_info->select_text();
				active_info->clamp();
			}
		}

		void text_input_info::set_value(std::string new_value) {
			switch (type) {
				case var_type::type_int:
					*(int*)value = atoi(new_value.c_str());
					break;
				case var_type::type_string:
					*(std::string*)value = new_value;
					break;
				case var_type::type_float:
					*(float*)value = atof(new_value.c_str());
					break;
			}
		}

		std::string text_input_info::convert_value_to_string() {
			switch (type) {
				case var_type::type_int: return std::to_string(*(int*)value);
				case var_type::type_string: return *(std::string*)value;
				case var_type::type_float: return utils::format(format.c_str(), *(float*)value);
			}
			return "";
		}

		void text_input_info::update_string_value() {
			string_value = convert_value_to_string();
		}

		void text_input_info::update_value() {
			set_value(string_value);
		}

		void text_input_info::update_utf_text() {
			if (string_value != last_value) {
				last_value = string_value;
				converted_value = null_font::helpers::convert_wstring(string_value);
				value_for_render = null_font::helpers::convert_string(converted_value);
			}
		}

		void text_input_info::get_pos_on_cursor() {
			if (null_input::get_key(null_input::key_id::mouse_left)->clicked() && converted_value.size() > 0) {
				vec2 size = null_font::text_size_w(converted_value);
				if (!null_input::click_mouse_in_region(working_rect.min, working_rect.min + size)) {
					if (null_input::mouse_in_region(working_rect)) pos_in_text = converted_value.size();
					return;
				}

				std::wstring a = L""; a += converted_value[get_id_under_cursor()];
				float last_size = null_font::text_size_w(a).x;
				float centre_pos = working_rect.min.x + get_text_offset(get_id_under_cursor()) + (last_size / 2.f);

				pos_in_text = null_input::mouse_pos.x <= centre_pos ? get_id_under_cursor() : get_id_under_cursor() + 1;
			}
		}

		void text_input_info::select_text() {
			vec2 text_size = null_font::text_size_w(converted_value);
			if (null_input::get_key(null_input::key_id::mouse_left)->down() && !null_input::click_mouse_in_region(working_rect.min, working_rect.max)) {
				reset_select();
				return;
			}

			if (null_input::get_key(null_input::key_id::mouse_left)->clicked() && null_input::mouse_in_region(working_rect.min, working_rect.max)) {
				reset_select();
			}

			if (null_input::get_key(null_input::key_id::mouse_left)->double_clicked() || (null_input::get_key(null_input::key_id::ctrl)->down() && null_input::get_key(null_input::key_id::a)->down())) {
				select_min = 0;
				select_max = converted_value.size();
				selecting = select_type::all;
			}

			if (null_input::get_key(null_input::key_id::mouse_left)->down() &&  selecting != select_type::all && converted_value.size() > 0) {
				std::wstring a_t = L""; a_t += converted_value[get_id_under_cursor()];
				float last_size = null_font::text_size_w(a_t).x;
				float centre_pos = working_rect.min.x + get_text_offset(get_id_under_cursor()) + (last_size / 2.f);
				int b = null_input::mouse_pos.x <= centre_pos ? get_id_under_cursor() : get_id_under_cursor() + 1;
				int a = pos_in_text;

				if (a != b) {
					select_max = math::max(a, b);
					select_min = math::min(a, b);
					selecting = select_type::down;
				} else {
					reset_select();
				}
			}
		}

		int text_input_info::get_id_under_cursor() {
			int id = 0;
			vec2 size = null_font::text_size_w(converted_value);
			std::wstring temp = converted_value;

			if (null_input::mouse_pos.x >= working_rect.min.x + size.x)
				return converted_value.size() - 1;

			for (int i = converted_value.size() - 1; i >= 0; i--) {
				if(null_input::mouse_pos.x < working_rect.min.x + size.x)
					id = i;

				temp.pop_back();
				size = null_font::text_size_w(temp);
			}
			return id;
		}

		float text_input_info::get_text_offset(int offset) {
			std::wstring text = std::wstring(converted_value.begin(), converted_value.begin() + math::clamp(offset, 0, (int)converted_value.size())) + L".";
			return null_font::text_size_w(text).x  - null_font::text_size(".").x;
		}

		std::string parse_format(std::string format) {
			std::string ret = "%";

			int find_position = format.find('%');
			std::string aut;
			if (find_position != std::string::npos) {
				for (int i = find_position; i < format.size(); i++) {
					if ((format[i] > '0' && format[i] < '9') || (format[i] > 'a' && format[i] < 'z') || format[i] == '.')
						ret.push_back(format[i]);

					if (format[i] > 'a' && format[i] < 'z')
						break;
				}
			}

			return ret;
		}

		bool window_exist(std::string name) {
			return find_window(name) != nullptr;
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

			if (wnd->flags.contains(window_flags::popup)) wnd->flags.add(window_flags::no_move);

			wnd->idx = wnd->flags.contains(window_flags::group) ? 0 : windows.size();

			if (windows.size() == 0) windows.push_back(wnd);
			else {
				if (wnd->flags.contains(window_flags::popup) || wnd->flags.contains(window_flags::group)) {
					if (wnd->flags.contains(window_flags::popup)) current_window->child_popup_window.push_back(wnd);
					else current_window->child_group_window.push_back(wnd);
				}
				windows.insert(wnd->flags.contains(window_flags::group) ? windows.begin() : windows.end(), wnd);
			}

			return wnd;
		}

		bool can_use_item(rect item_rect, std::string item_name) {
			window* wnd = deeps::current_window;
			if (!wnd) return false;
			return (hovered_name == "" || hovered_name == item_name) && ((null_input::mouse_in_region(item_rect) && null_input::mouse_in_region(wnd->draw_list->get_clip_rect()) && deeps::mouse_in_current_windows()) || active_name == item_name);
		}

		bool text_input_behavior(rect size, bool* hovered, bool* pressed, std::string name) {
			window* wnd = deeps::current_window;
			bool _hovered = false;
			bool _pressed = false;

			if (can_use_item(size, name)) {
				hovered_name = name;

				if (!null_input::get_key(null_input::key_id::mouse_left)->down()) _hovered = true;
				if (null_input::click_mouse_in_region(size) && null_input::get_key(null_input::key_id::mouse_left)->pressed()) active_name = name;
			}

			if (active_name == name) {
				if (null_input::get_key(null_input::key_id::mouse_left)->down()) _pressed = true;
			}

			if (((null_input::get_key(null_input::key_id::mouse_left)->down() && !null_input::click_mouse_in_region(size)) || null_input::get_key(null_input::key_id::enter)->down()) && active_name == name) {
				active_name = "";
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = _pressed;
			return active_name == name;
		}

		void scroll_behavior(rect size, bool* hovered, bool* pressed) {
			window* wnd = deeps::current_window;
			bool _hovered = false;
			bool _pressed = false;
			std::string name = utils::format("scrollbar##%s", wnd->name.c_str());

			if (can_use_item(size, name)) {
				hovered_name = name;

				if (!null_input::get_key(null_input::key_id::mouse_left)->down()) _hovered = true;
				if (null_input::get_key(null_input::key_id::mouse_left)->clicked()) active_name = name;

				if (active_name == name && null_input::get_key(null_input::key_id::mouse_left)->down()) _pressed = true;
			}

			if (!null_input::get_key(null_input::key_id::mouse_left)->down() && active_name == name) {
				active_name = "";
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = _pressed;
		}
		
		bool key_bind_behavior(null_input::bind_key* bind, rect size, bool* hovered, std::string name) {
			window* wnd = deeps::current_window;
			bool _hovered = false;

			if (can_use_item(size, name)) {
				hovered_name = name;

				if (!bind->binding) {
					if (!null_input::get_key(null_input::key_id::mouse_left)->down()) _hovered = true;
					if (null_input::get_key(null_input::key_id::mouse_right)->clicked()) active_name = name;
					if (null_input::get_key(null_input::key_id::mouse_left)->clicked()) {
						active_name = name;
						bind->binding = true;
						null_input::last_press_key = 0;
					}
				}
			}

			if (active_name == name && null_input::get_key(null_input::key_id::mouse_right)->pressed() && null_input::click_mouse_in_region(size) && wnd->can_open_tooltip()) {
				deeps::add_window(utils::format("##%s keybind tooltip", name.c_str()), vec2(size.max.x, size.min.y), vec2(0.f, 0.f), { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size });
			}

			if (bind->binding) {
				if ((!null_input::mouse_in_region(size) || !deeps::mouse_in_current_windows()) && null_input::get_key(null_input::key_id::mouse_left)->clicked()) {
					active_name = "";
					bind->binding = false;
				}
			} else if(active_name == name && !null_input::get_key(null_input::key_id::mouse_right)->down()) {
				active_name = "";
			}

			if (bind->binding && active_name == name) {
				if (null_input::get_key(null_input::key_id::escape)->clicked()) {
					bind->key = null_input::get_key(0);
					active_name = "";
					bind->binding = false;
				} else {
					if (null_input::last_press_key != 0) {
						bind->key = null_input::get_key(null_input::last_press_key);
						bind->binding = false;
						active_name = "";
					}
				}
			}

			if (hovered) *hovered = _hovered;
			return active_name == name;
		}

		bool button_behavior(rect size, bool* hovered, bool* pressed, std::string name) {
			window* wnd = deeps::current_window;
			bool _active = false;
			bool _hovered = false;
			bool _pressed = false;

			if (can_use_item(size, name)) {
				hovered_name = name;

				if (!null_input::get_key(null_input::key_id::mouse_left)->down()) _hovered = true;
				if (null_input::get_key(null_input::key_id::mouse_left)->clicked()) active_name = name;
			}

			if (active_name == name) {
				if (null_input::get_key(null_input::key_id::mouse_left)->down()) _pressed = true;
				if (null_input::mouse_in_region(size) && deeps::mouse_in_current_windows() && null_input::get_key(null_input::key_id::mouse_left)->pressed()) _active = true;
			}

			if (!null_input::get_key(null_input::key_id::mouse_left)->down() && active_name == name) {
				active_name = "";
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = _pressed;
			return _active;
		}

		void slider_behavior(rect size, bool* hovered, bool* pressed, std::string name) {
			window* wnd = deeps::current_window;
			bool _hovered = false;
			
			if (can_use_item(size, name)) {
				hovered_name = name;

				if (!null_input::get_key(null_input::key_id::mouse_left)->down()) _hovered = true;
				if (null_input::get_key(null_input::key_id::mouse_left)->clicked()) active_name = name;
			}

			if (!null_input::get_key(null_input::key_id::mouse_left)->down() && active_name == name) {
				active_name = "";
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = active_name == name;
		}

		bool combo_behavior(rect size, bool* hovered, bool* pressed, std::string name, std::vector<window_flags>& flags) {
			window* wnd = deeps::current_window;
			bool _active = false;
			bool _hovered = false;

			if (can_use_item(size, name)) {
				hovered_name = name;

				if (!null_input::get_key(null_input::key_id::mouse_left)->down()) _hovered = true;
				if (null_input::get_key(null_input::key_id::mouse_left)->clicked()) active_name = name;

				if (null_input::get_key(null_input::key_id::mouse_left)->pressed() && null_input::click_mouse_in_region(size) && null_input::mouse_in_region(size) && wnd->can_open_tooltip()) {
					deeps::add_window(name, vec2(size.min.x, size.max.y), vec2(size.max.x - size.min.x, 0.f), flags);
					_active = true;
				}
			}

			if (!null_input::get_key(null_input::key_id::mouse_left)->down() && active_name == name) {
				active_name = "";
			}

			if (hovered) *hovered = _hovered;
			if (pressed) *pressed = active_name == name;
			return _active;
		}

		void colorpicker_behavior(rect size, std::string name_item, std::string name, std::string tooltip, std::vector<window_flags> flags, bool alpha_bar) {
			window* wnd = deeps::current_window;

			if (can_use_item(size, name)) {
				hovered_name = name_item;

				if (null_input::click_mouse_in_region(size)) {
					if (null_input::get_key(null_input::key_id::mouse_left)->down() || null_input::get_key(null_input::key_id::mouse_right)->down()) active_name = name_item;
					if (null_input::get_key(null_input::key_id::mouse_left)->pressed() && wnd->can_open_tooltip()) {
						flags.push_back(window_flags::no_title_line);
						deeps::add_window(name, vec2(size.max.x, size.min.y), vec2(0.f, 0.f), flags);
					} else if (null_input::get_key(null_input::key_id::mouse_right)->pressed() && wnd->can_open_tooltip()) {
						deeps::add_window(tooltip, vec2(size.max.x, size.min.y), vec2(0.f, 0.f), flags);
					}
				}
			}
			

			if (!null_input::get_key(null_input::key_id::mouse_left)->down() && !null_input::get_key(null_input::key_id::mouse_right)->down() && active_name == name_item) {
				active_name = "";
			}
		}

		bool colorpicker_sliders_behavior(rect size, std::string name) {
			window* wnd = deeps::current_window;
			bool _pressed = false;

			if (can_use_item(size, name)) {
				hovered_name = name;

				if (null_input::get_key(null_input::key_id::mouse_left)->clicked() && active_name == "") active_name = name;
			}

			if (active_name == name) _pressed = true;

			if (!null_input::get_key(null_input::key_id::mouse_left)->down() && active_name == name) {
				active_name = "";
			}

			return _pressed;
		}

		void add_item(vec2 size, std::string name) {
			window* wnd = current_window;
			if (!wnd || wnd->dont_add_item) return;
			size += vec2(gui_settings::item_spacing, 0.f);
			wnd->last_draw_item_pos = wnd->draw_item_pos;
			wnd->draw_item_pos_prev = wnd->draw_item_pos + vec2(size.x, 0.f);
			
			if (wnd->draw_item_pos_same_line != vec2(0, 0)) {
				wnd->draw_item_pos.x = wnd->draw_item_pos_same_line.x;
				wnd->draw_item_pos.y = math::max(wnd->draw_item_pos_same_line.y, wnd->draw_item_pos.y + size.y + gui_settings::item_spacing);
			} else {
				wnd->draw_item_pos.x = wnd->pos.x + (wnd->column_offset == 0.f ? gui_settings::window_padding.x : wnd->column_offset);
				wnd->draw_item_pos.y += size.y + gui_settings::item_spacing;
			}

			if (wnd->max_size.x < wnd->draw_item_pos_prev.x - deeps::current_window->pos.x + wnd->get_scroll_thickness()) wnd->max_size.x = wnd->draw_item_pos_prev.x - deeps::current_window->pos.x + wnd->get_scroll_thickness();
			if (wnd->max_size.y < wnd->draw_item_pos.y - deeps::current_window->pos.y) wnd->max_size.y = wnd->draw_item_pos.y - deeps::current_window->pos.y;

			wnd->draw_item_pos_same_line = vec2(0, 0);
			if(!name.empty()) last_item_name = name;
		}

		std::string format_item(std::string text) {
			std::string ret = text;

			int find_position = text.find('#');
			std::string aut;
			if (find_position != std::string::npos) {
				if (find_position < text.size() - 2 && text[find_position + 1] == '#')
					ret = std::string(text.begin(), text.begin() + find_position);
			}

			return ret;
		}

		void set_active_item(std::string item_name) {
			active_name = item_name;
		}

		bool mouse_in_current_windows() {
			return deeps::hovered_window == (deeps::current_window->flags.contains(window_flags::group) ? deeps::current_window->get_main_window() : deeps::current_window);
		}

		void focus_current_window() {
			if(current_window->flags.contains(window_flags::group)) current_window->get_main_window()->focus_window();
			else current_window->focus_window();
		}

		void close_current_window() {
			current_window->close_call = true;
		}

		void window_control() {
			for (int i = deeps::windows.size() - 1; i >= 0; i--) { //getting hovered window and group
				window* wnd = deeps::windows[i];
				if (!wnd->visible) continue;

				if (!null_input::mouse_in_region(wnd->pos, wnd->pos + wnd->size)) {
					wnd->hovered_group = nullptr;
				} else {
					hovered_window = wnd->flags.contains(window_flags::group) ? wnd->get_main_window() : wnd;
					hovered_window->hovered_group = hovered_window->get_hovered_group();
					break;
				}
			}

			if (null_input::get_key(null_input::key_id::mouse_left)->clicked() && (active_name.empty() && active_window_name.empty())) { //window moving
				for (int i = windows.size() - 1; i >= 0; --i) {
					window* wnd = windows[i];

					if (wnd->flags.contains(window_flags::no_move)) continue;

					if (!wnd->dragging && hovered_window == wnd && null_input::mouse_in_region(wnd->pos, wnd->pos + vec2(wnd->size.x, gui_settings::move_window_on_title_bar ? gui_settings::window_title_size : wnd->size.y)) && wnd->visible) {
						wnd->drag_offset = null_input::mouse_pos - wnd->pos;
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

				if (wnd->flags.contains(window_flags::popup)) {
					if (!deeps::window_exist(wnd->parent_window->name) || !wnd->parent_window->visible) {
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

	void new_line() {
		deeps::add_item(vec2(0.f, gui_settings::new_line_size), "");
	}

	void same_line() {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		wnd->draw_item_pos = wnd->draw_item_pos_prev + vec2(gui_settings::item_spacing, 0.f);
	}

	void tooltip(std::string tooltip_text) {
		functional_tooltip([=]() { text(tooltip_text.c_str()); });
	}

	void functional_tooltip(std::function<void()> func) {
		window* wnd = deeps::current_window;
		if (!wnd) return;
		if (deeps::active_window_name != "" || deeps::active_name != "" || !wnd->can_open_tooltip()) return;
		bool close = deeps::hovered_name == deeps::last_item_name;
		deeps::push_var(&gui_settings::window_padding, vec2(5.f, 5.f)); {
 			if (begin_window("##tooltip", null_input::mouse_pos + vec2(10, 10), vec2(0.f, 0.f), { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size }, &close)) {
				deeps::focus_current_window();
				func();
				end_window();
			}
		} deeps::pop_var();
	}
}