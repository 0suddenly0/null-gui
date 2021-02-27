#include "../null-gui.h"

namespace null_gui {
	void text(std::string text) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());

		wnd->draw_list->draw_text(text, draw_pos, gui_settings::text, false);
		
		deeps::add_item(null_font::text_size(text), name);
	}
}