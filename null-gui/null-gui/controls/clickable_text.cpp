#include "../null-gui.h"

namespace null_gui {
	bool clickable_text(std::string text) {
		window* wnd = deeps::current_window;
		if (!wnd) return false;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		rect size(draw_pos, draw_pos + text_size);
		bool hovered, pressed;
		bool active = deeps::button_behavior(size, &hovered, &pressed, name);

		wnd->draw_list->draw_text(deeps::format_item(draw_text), size.min, hovered || pressed ? gui_settings::main_color : gui_settings::text, false);
		
		deeps::add_item(size.size(), name);
		return active;
	}
}