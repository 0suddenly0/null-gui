#include "../gui.h"

namespace null {
	namespace gui {
		void key_bind(std::string text, input::bind_key* bind) {
			window* wnd = detail::current_window;
			if(!wnd) return;

			std::string draw_text = detail::format_item(text);
			std::string bind_text = bind->key->data.name;

			if(bind->binding) bind_text = "...";
			else if(bind->key->data.id == 0) bind_text = "-";

			vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
			vec2 text_size = font::text_size(draw_text);
			vec2 bind_text_size = font::text_size(bind_text);
			vec2 min = vec2(text_size.x + (draw_text.empty() ? 0.f : style::text_spacing) + bind_text_size.x + style::button_padding.x * 2, text_size.y + style::button_padding.y * 2);

			if(draw_text.empty()) draw_pos += vec2(wnd->get_window_size_with_padding().x - min.x, 0.f);

			rect item_rect = rect(draw_pos, draw_pos + vec2(style::items_size_full_window && !draw_text.empty() ? math::max(min.x, wnd->get_window_size_with_padding().x) : min.x, min.y));
			rect button_rect = rect(vec2(item_rect.max.x - (bind_text_size.x + style::button_padding.x * 2), item_rect.max.y - (text_size.y + style::button_padding.y * 2)), item_rect.max);

			if(!draw_text.empty())
				wnd->draw_list->draw_text(draw_text, vec2(item_rect.min.x, item_rect.min.y + item_rect.size().y / 2), style::text, false, { false, true });

			detail::add_item(item_rect.size(), text);
			if(!wnd->can_draw_item(item_rect))
				return;

			std::string tooltip_window = utils::format("##%s key_bind tooltip", text.c_str());

			bool hovered;
			bool active = detail::controls_behavior::key_bind(bind, button_rect, &hovered, text);

			wnd->draw_list->draw_rect_filled(button_rect.min, button_rect.max, active ? style::button_bg_active : hovered ? style::button_bg_hovered : style::button_bg, style::key_bind_rounding);
			wnd->draw_list->draw_text(bind_text, button_rect.min + button_rect.size() / 2, style::text, false, { true, true });

			if(detail::window_exist(tooltip_window)) {
				detail::push_var(&style::window_padding, vec2(0.f, 0.f)); {
					detail::push_var(&style::item_spacing, 0.f); {
						detail::push_var(&style::selectable_active_offset, style::selectable_active_offset); {
							if(begin_window(tooltip_window, vec2(button_rect.max.x, button_rect.min.y), vec2(0.f, 0.f), { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size }, nullptr)) {
								std::vector<std::string> bind_types{ "always", "hold on", "hold off", "toggle" };
								for(int i = 0; i < bind_types.size(); i++) {
									if(selectable(bind_types[i], bind->type == (input::bind_type)i))
										bind->type = (input::bind_type)i;
								}
								end_window();
							}
						} detail::pop_var();
					} detail::pop_var();
				} detail::pop_var();
			}
		}

		namespace detail {
			namespace controls_behavior {
				bool key_bind(input::bind_key* bind, rect size, bool* hovered, std::string name) {
					window* wnd = detail::current_window;
					bool _hovered = false;

					if(can_use_item(size, name)) {
						hovered_name = name;

						if(!bind->binding) {
							if(!input::get_key(input::key_id::mouse_left)->down()) _hovered = true;
							if(input::get_key(input::key_id::mouse_right)->clicked()) active_name = name;
							if(input::get_key(input::key_id::mouse_left)->clicked()) {
								active_name = name;
								bind->binding = true;
								input::last_press_key = 0;
							}
						}
					}

					if(active_name == name && input::get_key(input::key_id::mouse_right)->pressed() && input::click_mouse_in_region(size) && wnd->can_open_tooltip()) {
						detail::add_window(utils::format("##%s key_bind tooltip", name.c_str()), vec2(size.max.x, size.min.y), vec2(0.f, 0.f), { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size });
					}

					if(bind->binding) {
						if((!input::mouse_in_region(size) || !detail::mouse_in_current_window()) && input::get_key(input::key_id::mouse_left)->clicked()) {
							active_name = "";
							bind->binding = false;
						}
					} else if(active_name == name && !input::get_key(input::key_id::mouse_right)->down()) {
						active_name = "";
					}

					if(bind->binding && active_name == name) {
						if(input::get_key(input::key_id::escape)->clicked()) {
							bind->key = input::get_key(0);
							active_name = "";
							bind->binding = false;
						} else {
							if(input::last_press_key != 0) {
								bind->key = input::get_key(input::last_press_key);
								bind->binding = false;
								active_name = "";
							}
						}
					}

					if(hovered) *hovered = _hovered;
					return active_name == name;
				}
			}
		}
	}
}