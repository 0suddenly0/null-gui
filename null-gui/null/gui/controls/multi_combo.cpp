#include "../gui.h"

namespace null {
	namespace gui {
		void multi_combo(std::string text, std::vector<bool>* values, std::vector<std::string> items) {
			window* wnd = detail::current_window;
			if(!wnd) return;

			std::string draw_text = detail::format_item(text);
			std::array<vec2, 3> arrow = math::calc_arrow(style::combo_arrow_size * wnd->draw_list->shared_data->font_size);
			float arrow_size = arrow.at(0).dist_to(arrow.at(2));
			vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
			vec2 text_size = font::text_size(draw_text);
			vec2 min = text_size + style::combo_size + vec2(arrow_size, style::text_spacing);
			rect item_rect = rect(draw_pos, draw_pos + vec2(style::items_size_full_window ? math::max(min.x, wnd->get_window_size_with_padding().x) : min.x, min.y));
			rect body_rect = rect(vec2(item_rect.min.x, item_rect.min.y + text_size.y + style::text_spacing), item_rect.max);
			flags_list<window_flags> flags = { window_flags::popup, window_flags::set_pos, window_flags::set_size, window_flags::auto_size, window_flags::no_title_line };

			detail::add_item(item_rect.size(), text);
			if(!wnd->can_draw_item(item_rect))
				return;

			std::string popup_window = utils::format("##%s multi_combo popup", text.c_str());

			bool hovered, pressed;
			bool open = detail::controls_behavior::combo(body_rect, &hovered, &pressed, popup_window, flags);

			std::string text_on_combo = "";
			for(int a = 0; a < values->size(); a++) {
				if(values->at(a)) {
					if(a <= values->size() - 1 && text_on_combo.size() > 0) text_on_combo += ", ";
					text_on_combo += items[a];
				}
			}

			if(font::text_size(text_on_combo).x + font::text_size("...").x > item_rect.size().x - (body_rect.max.y - body_rect.min.y)) {
				for(int i = text_on_combo.size() - 1; i >= 0; i--) {
					if(font::text_size(text_on_combo).x + font::text_size("...").x < item_rect.size().x - (body_rect.max.y - body_rect.min.y)) {
						text_on_combo.pop_back();
						text_on_combo += "...";
						break;
					} else {
						text_on_combo.pop_back();
					}
				}
			}

			wnd->draw_list->draw_text(draw_text, item_rect.min, style::text, false);
			wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, hovered || pressed ? pressed ? style::button_bg_active : style::button_bg_hovered : style::button_bg, style::combo_rounding);
			wnd->draw_list->draw_text(text_on_combo, vec2(body_rect.min.x + style::text_spacing, body_rect.max.y - ((body_rect.max.y - body_rect.min.y) / 2)), style::text, false, { false, true });

			vec2 arrow_pos = rect(vec2(body_rect.max.x - body_rect.size().y, body_rect.min.y), body_rect.max).centre();
			wnd->draw_list->draw_triangle_filled({ arrow_pos + arrow.at(0), arrow_pos + arrow.at(1), arrow_pos + arrow.at(2) }, style::main_color);

			if(detail::window_exist(popup_window)) {
				detail::push_var(&style::window_padding, vec2(0.f, 0.f)); {
					detail::push_var(&style::item_spacing, 0.f); {
						if(begin_window(popup_window, vec2(body_rect.min.x, body_rect.max.y + style::combo_window_padding), vec2(body_rect.max.x - body_rect.min.x, 0.f), flags, nullptr)) {
							for(int i = 0; i < items.size(); i++) {
								if(i == style::max_auto_size_combo) {
									detail::current_window->arg_size.y = detail::current_window->size.y = detail::current_window->max_size.y + style::window_padding.y - style::item_spacing;
									detail::current_window->flags.remove(window_flags::auto_size);
								}

								if(selectable(items[i], values->at(i)))
									values->at(i) = !values->at(i);
							}
							end_window();
						}
					} detail::pop_var();
				} detail::pop_var();
			}
		}

		void multi_combo(std::string text, std::vector<bool*> values, std::vector<std::string> items) {
			std::vector<bool> _values;
			for(bool* a : values)
				_values.push_back(*a);

			multi_combo(text, &_values, items);

			for(int i = 0; i < values.size(); i++)
				*values.at(i) = _values.at(i);
		}
	}
}