#include "../null-gui.h"

namespace null_gui {
	void text(std::string text) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->scroll_offset);
		vec2 left_spacing = vec2(gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_spacing : 0, 0.f);
		wnd->draw_list.add_text(text, draw_pos + left_spacing, gui_settings::text, false);
		deeps::add_item(null_font::text_size(text), name);
	}
}