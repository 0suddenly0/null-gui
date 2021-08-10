#include "../gui.h"

namespace null {
	namespace gui {
		bool clickable_text(std::string text) {
			window* wnd = detail::current_window;
			if(!wnd) return false;

			std::string draw_text = detail::format_item(text);
			vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
			vec2 text_size = font::text_size(draw_text.c_str());
			rect item_rect = rect(draw_pos, draw_pos + text_size);

			detail::add_item(item_rect.size(), text);
			if(!wnd->can_draw_item(item_rect))
				return false;

			bool hovered, pressed;
			bool active = detail::controls_behavior::button(item_rect, &hovered, &pressed, text);

			wnd->draw_list->draw_text(draw_text, item_rect.min, hovered || pressed ? style::main_color : style::text, false);

			return active;
		}
	}
}