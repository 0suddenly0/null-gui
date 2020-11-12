#include "../null-gui.h"

namespace null_gui {
	bool selectable(std::string text, bool selected) {
		window* wnd = deeps::current_window;
		if (!wnd) return false;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->scroll_offset);
		vec2 text_size = null_font::text_size(draw_text);
		rect size(draw_pos, draw_pos + vec2(wnd->size.x, text_size.y));
		bool hovered, pressed;
		bool active = null_gui::deeps::get_button_behavior(size, &hovered, &pressed, name);

		wnd->draw_list.add_text(deeps::format_item(draw_text), vec2(size.min.x + (selected ? gui_settings::selectable_active_offset : gui_settings::selectable_offset), draw_pos.y + (size.size().y / 2)), selected ? gui_settings::main_color : hovered || pressed ? gui_settings::text_hovered : gui_settings::text, false, { false , true });
		deeps::add_item(size.size(), name);

		return active;
	}
}