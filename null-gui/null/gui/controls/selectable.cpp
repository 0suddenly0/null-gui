#include "../gui.h"

namespace null {
	namespace gui {
		bool selectable(std::string text, bool selected) {
			window* wnd = detail::current_window;
			if(!wnd) return false;

			std::string draw_text = detail::format_item(text);
			vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
			vec2 text_size = font::text_size(text);
			vec2 min = vec2(style::selectable_active_offset + style::selectable_offset + font::text_size(draw_text).x, text_size.y);
			rect item_rect = rect(draw_pos, draw_pos + math::max(vec2(style::items_size_full_window ? math::max(min.x, wnd->get_window_size_with_padding().x) : min.x, min.y), vec2(0.f, 0.f)));

			detail::add_item(item_rect.size(), text);
			if(!wnd->can_draw_item(item_rect))
				return false;

			bool hovered, pressed;
			bool active = detail::controls_behavior::button(item_rect, &hovered, &pressed, text);

			wnd->draw_list->draw_text(draw_text, vec2(item_rect.min.x + (selected ? style::selectable_active_offset : style::selectable_offset), item_rect.centre().y), selected ? style::main_color : hovered || pressed ? style::text_hovered : style::text, false, { false , true });

			return active;
		}
	}
}