#include "../null-gui.h"

namespace null_gui {
	void text(std::string text) {
		window* wnd = deeps::get_current_window();
		if (!wnd) return;

		vec2 draw_pos = wnd->draw_item_pos;
		vec2 left_spacing = vec2(gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_checkbox_spacing : 0, 0.f);
		wnd->draw_list.add_text(text, draw_pos + left_spacing, color(255, 255, 255, 255), false);
		deeps::add_item(null_font::text_size(text));
	}
}