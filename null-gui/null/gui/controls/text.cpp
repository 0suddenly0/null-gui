#include "../gui.h"

namespace null {
	namespace gui {
		void text(std::string text) {
			window* wnd = detail::current_window;
			if(!wnd) return;

			vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());

			wnd->draw_list->draw_text(text, draw_pos, style::text, false);

			detail::add_item(font::text_size(text), text);
		}
	}
}