#include "../null-gui.h"

namespace null_gui {
	void text(std::string text) {
		window* wnd = deeps::get_current_window();
		if (!wnd) return;
		wnd->draw_list.add_text(text, wnd->draw_item_pos, color(255, 255, 255, 255), false);
		deeps::add_item(null_font::text_size(text));
	}
}