#include "../gui.h"

namespace null {
	namespace gui {
		bool text_input(std::string text, void* value, bool password, var_type type, std::string format) {
			window* wnd = detail::current_window;
			if(!wnd) return false;

			std::string draw_text = detail::format_item(text);
			vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
			vec2 text_size = font::text_size(draw_text);
			vec2 min = text_size + style::text_spacing + style::combo_size + vec2(0.f, style::text_input_line_size);
			rect item_rect = rect(draw_pos, draw_pos + vec2(style::items_size_full_window ? math::max(min.x, wnd->get_window_size_with_padding().x) : min.x, min.y));
			rect body_rect = rect(vec2(item_rect.min.x, item_rect.min.y + text_size.y + style::text_spacing), item_rect.max);
			rect working_rect = rect(body_rect.min + style::text_input_padding, body_rect.max - vec2(style::text_input_padding.x, style::text_input_padding.y + style::text_input_line_size));
			detail::text_input_info* input = detail::text_input_info::add(detail::text_input_info(text, value, working_rect, type, format));

			if(!input) return false;

			vec2 value_size = font::text_size_w(input->converted_value);

			detail::add_item(item_rect.size(), text);
			if(!wnd->can_draw_item(item_rect))
				return false;

			bool hovered, pressed;
			bool active = detail::controls_behavior::text_input(body_rect, &hovered, &pressed, text);

			wnd->draw_list->draw_text(draw_text, item_rect.min, style::text, false);
			wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max - vec2(0.f, style::text_input_line_size), style::button_bg, style::text_input_rounding, { render::corner_flags::top });

			wnd->draw_list->push_clip_rect(body_rect.min, body_rect.max, true); {
				if(input->is_selecting()) {
					float min_pos = input->get_text_offset(input->select_min);
					float max_pos = input->get_text_offset(input->select_max);
					wnd->draw_list->draw_rect_filled(working_rect.min + vec2(min_pos, (working_rect.size().y / 2) - (value_size.y / 2)), working_rect.min + vec2(max_pos, (working_rect.size().y / 2) + (value_size.y / 2)), style::select_text);
				}

				wnd->draw_list->draw_text(input->value_for_render, working_rect.min + vec2(0.f, working_rect.size().y / 2), style::text, false, { false, true });
				if(active && !input->is_selecting() && input->show_pos) wnd->draw_list->draw_text("|", working_rect.min + vec2(input->get_text_offset(input->pos_in_text) - 2.f, working_rect.size().y / 2), style::text, false, { false, true });
			} wnd->draw_list->pop_clip_rect();
			wnd->draw_list->draw_rect_filled(vec2(body_rect.min.x, body_rect.max.y - style::text_input_line_size), body_rect.max, style::main_color, style::text_input_rounding, { render::corner_flags::bot });

			return active;
		}

		bool text_input(std::string text, std::string* value, bool password) {
			return text_input(text, value, password, var_type::type_string, "%s");
		}

		bool text_input(std::string text, int* value) {
			return text_input(text, value, false, var_type::type_int, "%d");
		}

		bool text_input(std::string text, float* value, std::string format) {
			return text_input(text, value, false, var_type::type_float, format);
		}

		namespace detail {
			text_input_info::text_input_info(std::string _name, void* _value, rect _working_rect, var_type _type, std::string _format)
				: name(_name), value(_value), working_rect(_working_rect), type(_type) {
				format = detail::parse_format(_format);
				update_string_value();
				pos_in_text = string_value.size();
			}

			text_input_info* text_input_info::add(text_input_info input) {
				text_input_info* finded = get_input(input.name);

				if(finded) {
					finded->name = input.name;
					finded->working_rect = input.working_rect;
					return finded;
				} else text_inputs.push_back(input);

				return &text_inputs.back();
			}

			text_input_info* text_input_info::get_input(std::string name) {
				for(text_input_info& a : text_inputs) if(a.name == name) return &a;
				return nullptr;
			}

			void text_input_info::win_poc(input::key_id key) {
				text_input_info* active = get_input(active_name);
				if(!active) return;

				switch(key) {
				case input::key_id::backspace: {
						if(!active->is_selecting()) {
							if(active->pos_in_text > 0) {
								active->string_value.erase(active->string_value.begin() + active->pos_in_text - 1, active->string_value.begin() + active->pos_in_text);
								active->pos_in_text--;
								active->clamp();
							}
						} else {
							active->string_value.erase(active->string_value.begin() + active->select_min, active->string_value.begin() + active->select_max);
							active->pos_in_text = active->select_min;
							active->clamp();
						}
					} break;
				case input::key_id::del: {
						if(!active->is_selecting()) {
							if(active->pos_in_text < active->string_value.size()) {
								active->string_value.erase(active->string_value.begin() + active->pos_in_text, active->string_value.begin() + active->pos_in_text + 1);
							}
						} else {
							active->string_value.erase(active->string_value.begin() + active->select_min, active->string_value.begin() + active->select_max);
							active->pos_in_text = active->select_min;
							active->clamp();
						}
					} break;
				case input::key_id::left: {
						if(input::get_key(input::key_id::ctrl)->down()) active->pos_in_text = 0;
						else {
							if(!active->is_selecting()) active->pos_in_text--;
							else {
								active->pos_in_text = active->select_min;
								active->reset_select();
							}
						}
					} break;
				case input::key_id::right: {
						if(input::get_key(input::key_id::ctrl)->down()) active->pos_in_text = active->string_value.size();
						else {
							if(!active->is_selecting()) active->pos_in_text++;
							else {
								active->pos_in_text = active->select_max;
								active->reset_select();
							}
						}
					} break;
				case input::key_id::c: {
						if(input::get_key(input::key_id::ctrl)->down()) {
							input::write_clipboard(std::string(active->string_value.begin() + active->select_min, active->string_value.begin() + active->select_max));
							active->reset_select();
						}
					} break;
				case input::key_id::v: {
						if(input::get_key(input::key_id::ctrl)->down()) {
							if(active->is_selecting()) {
								active->string_value.erase(active->string_value.begin() + active->select_min, active->string_value.begin() + active->select_max);
								active->pos_in_text = active->select_min;
								active->reset_select();
							}

							active->string_value.insert(active->pos_in_text, input::read_clipboard());
							active->pos_in_text += input::read_clipboard().size();
						}
					} break;
				}

				active->update_value();
				active->update_utf_text();
				active->clamp();
			}

			void text_input_info::add_char() {
				text_input_info* active = get_input(active_name);
				if(!active) return;

				if(input::key_data::char_allowed(input::last_symbol) && !input::get_key(input::key_id::ctrl)->down()) {
					std::string value_for_check = active->string_value;
					char ascii_char = font::detail::utf8_to_ascii(input::last_symbol);
					value_for_check.insert(value_for_check.begin() + active->pos_in_text, ascii_char);
					bool can_write = font::text_size(font::detail::convert_utf8(value_for_check).c_str()).x < active->working_rect.size().x;

					if(!active->is_selecting()) {
						if(can_write) active->string_value = value_for_check;
					} else {
						active->string_value.erase(active->string_value.begin() + active->select_min, active->string_value.begin() + active->select_max);
						active->string_value.insert(active->string_value.begin() + active->select_min, ascii_char);
						active->pos_in_text = active->select_min;
						active->reset_select();
					}

					if(!active->is_selecting() && can_write) active->pos_in_text++;
					active->clamp();
				}

				active->update_value();
				active->update_utf_text();
				active->clamp();
			}

			void text_input_info::control() {
				text_input_info* active_info = get_input(active_name);

				if(!active_info) {
					for(text_input_info& info : text_inputs) {
						info.update_string_value();
						info.update_utf_text();
						info.get_pos_on_cursor();
						info.show_time = 0.f;
					}
				} else {
					if(detail::real_time == 0.f) active_info->show_time = detail::real_time;
					if(detail::real_time - active_info->show_time >= style::show_pos_in_text_cooldown) {
						active_info->show_time = detail::real_time;
						active_info->show_pos = !active_info->show_pos;
					}

					active_info->update_utf_text();
					active_info->get_pos_on_cursor();
					active_info->select_text();
					active_info->clamp();
				}
			}

			void text_input_info::set_value(std::string new_value) {
				switch(type) {
				case var_type::type_int: *(int*)value = atoi(new_value.c_str()); break;
				case var_type::type_string: *(std::string*)value = new_value; break;
				case var_type::type_float: *(float*)value = atof(new_value.c_str()); break;
				}
			}

			std::string text_input_info::convert_value_to_string() {
				switch(type) {
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
				if(string_value != last_value) {
					last_value = string_value;
					converted_value = font::detail::convert_wstring(string_value);
					value_for_render = font::detail::convert_string(converted_value);
				}
			}

			void text_input_info::get_pos_on_cursor() {
				if(input::get_key(input::key_id::mouse_left)->clicked() && converted_value.size() > 0) {
					vec2 size = font::text_size_w(converted_value);
					if(!input::click_mouse_in_region(working_rect.min, working_rect.min + size)) {
						if(input::mouse_in_region(working_rect)) pos_in_text = converted_value.size();
						return;
					}

					std::wstring a = L""; a += converted_value[get_id_under_cursor()];
					float last_size = font::text_size_w(a).x;
					float centre_pos = working_rect.min.x + get_text_offset(get_id_under_cursor()) + (last_size / 2.f);

					pos_in_text = input::mouse_pos.x <= centre_pos ? get_id_under_cursor() : get_id_under_cursor() + 1;
				}
			}

			void text_input_info::select_text() {
				vec2 text_size = font::text_size_w(converted_value);
				if(input::get_key(input::key_id::mouse_left)->down() && !input::click_mouse_in_region(working_rect.min, working_rect.max)) {
					reset_select();
					return;
				}

				if(input::get_key(input::key_id::mouse_left)->clicked() && input::mouse_in_region(working_rect.min, working_rect.max)) {
					reset_select();
				}

				if(input::get_key(input::key_id::mouse_left)->double_clicked() || (input::get_key(input::key_id::ctrl)->down() && input::get_key(input::key_id::a)->down())) {
					select_min = 0;
					select_max = converted_value.size();
					selecting = select_type::all;
				}

				if(input::get_key(input::key_id::mouse_left)->down() && selecting != select_type::all && converted_value.size() > 0) {
					std::wstring a_t = L""; a_t += converted_value[get_id_under_cursor()];
					float last_size = font::text_size_w(a_t).x;
					float centre_pos = working_rect.min.x + get_text_offset(get_id_under_cursor()) + (last_size / 2.f);
					int b = input::mouse_pos.x <= centre_pos ? get_id_under_cursor() : get_id_under_cursor() + 1;
					int a = pos_in_text;

					if(a != b) {
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
				vec2 size = font::text_size_w(converted_value);
				std::wstring temp = converted_value;

				if(input::mouse_pos.x >= working_rect.min.x + size.x)
					return converted_value.size() - 1;

				for(int i = converted_value.size() - 1; i >= 0; i--) {
					if(input::mouse_pos.x < working_rect.min.x + size.x)
						id = i;

					temp.pop_back();
					size = font::text_size_w(temp);
				}
				return id;
			}

			float text_input_info::get_text_offset(int offset) {
				std::wstring text = std::wstring(converted_value.begin(), converted_value.begin() + math::clamp(offset, 0, (int)converted_value.size())) + L".";
				return font::text_size_w(text).x - font::text_size(".").x;
			}

			namespace controls_behavior {
				bool text_input(rect size, bool* hovered, bool* pressed, std::string name) {
					window* wnd = detail::current_window;
					bool _hovered = false;
					bool _pressed = false;

					if(can_use_item(size, name)) {
						hovered_name = name;

						if(!input::get_key(input::key_id::mouse_left)->down()) _hovered = true;
						if(input::click_mouse_in_region(size) && input::get_key(input::key_id::mouse_left)->pressed()) active_name = name;
					}

					if(active_name == name) {
						if(input::get_key(input::key_id::mouse_left)->down()) _pressed = true;
					}

					if(((input::get_key(input::key_id::mouse_left)->down() && !input::click_mouse_in_region(size)) || input::get_key(input::key_id::enter)->down()) && active_name == name) {
						active_name = "";
					}

					if(hovered) *hovered = _hovered;
					if(pressed) *pressed = _pressed;
					return active_name == name;
				}
			}
		}
	}
}