#include "../gui.h"

namespace null {
	namespace gui {
		bool button(std::string text, vec2 size_arg) {
			window* wnd = detail::current_window;
			if(!wnd) return false;

			std::string draw_text = detail::format_item(text);
			vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
			vec2 text_size = font::text_size(draw_text);
			vec2 min = text_size + (style::button_padding * 2);
			rect button_rect = rect(draw_pos, draw_pos + math::max(vec2(style::items_size_full_window ? math::max(min.x + wnd->get_scroll_thickness(), wnd->get_window_size_with_padding().x) : min.x + wnd->get_scroll_thickness(), min.y), size_arg));

			detail::add_item(button_rect.size(), text);
			if(!wnd->can_draw_item(button_rect))
				return false;

			bool hovered, pressed;
			bool active = detail::controls_behavior::button(button_rect, &hovered, &pressed, text);

			wnd->draw_list->draw_rect_filled(button_rect.min, button_rect.max, hovered || pressed ? pressed ? style::main_color : style::button_bg_hovered : style::button_bg, style::button_rounding);
			wnd->draw_list->draw_text(draw_text, button_rect.centre(), style::text, false, { true , true });

			return active;
		}

		namespace detail {
			namespace controls_behavior {
				bool button(rect size, bool* hovered, bool* pressed, std::string name) {
					window* wnd = detail::current_window;
					bool _active = false;
					bool _hovered = false;
					bool _pressed = false;

					if(can_use_item(size, name)) {
						hovered_name = name;

						if(!input::get_key(input::key_id::mouse_left)->down()) _hovered = true;
						if(input::get_key(input::key_id::mouse_left)->clicked()) active_name = name;
					}

					if(active_name == name) {
						if(input::get_key(input::key_id::mouse_left)->down()) _pressed = true;
						if(input::mouse_in_region(size) && detail::mouse_in_current_window() && input::get_key(input::key_id::mouse_left)->pressed()) _active = true;
					}

					if(!input::get_key(input::key_id::mouse_left)->down() && active_name == name) {
						active_name = "";
					}

					if(hovered) *hovered = _hovered;
					if(pressed) *pressed = _pressed;
					return _active;
				}
			}
		}
	}
}