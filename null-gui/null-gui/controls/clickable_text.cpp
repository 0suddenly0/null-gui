#include "../null-gui.h"

namespace null_gui {
	bool clickable_text(std::string text) {
		window* wnd = deeps::current_window;
		if (!wnd) return false;

		std::string draw_text = deeps::format_item(text);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text.c_str());
		rect item_rect = rect(draw_pos, draw_pos + text_size);
		
		deeps::add_item(item_rect.size(), text);
		if (!wnd->can_draw_item(item_rect))
			return false;

		bool hovered, pressed;
		bool active = deeps::button_behavior(item_rect, &hovered, &pressed, text);

		wnd->draw_list->draw_text(draw_text, item_rect.min, hovered || pressed ? gui_settings::main_color : gui_settings::text, false);

		return active;
	}
}