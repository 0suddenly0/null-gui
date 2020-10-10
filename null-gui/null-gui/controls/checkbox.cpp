#include "../null-gui.h"

namespace null_gui {
	void checkbox(std::string text, bool* value) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos;
		vec2 text_size = null_font::text_size(draw_text);
		vec2 text_spacing(gui_settings::text_spacing + text_size.x, 0.f);
		rect size(draw_pos, vec2(draw_pos.x + gui_settings::checkbox_size, draw_pos.y + gui_settings::checkbox_size));
		bool hovered, pressed;
		bool active = null_gui::deeps::get_button_behavior(gui_settings::checkbox_hovered_with_text ? rect(size.min, size.max + text_spacing) : size, &hovered, &pressed, name);

		if (active) *value = !*value;

		wnd->draw_list.add_rect(size.min, size.max, hovered || pressed ? gui_settings::button_bg_hovered : gui_settings::button_bg);
		if(*value) wnd->draw_list.add_rect(size.min + vec2(2, 2), size.max - vec2(2, 2), gui_settings::main_color);
		wnd->draw_list.add_text(deeps::format_item(draw_text), vec2(size.max.x + gui_settings::text_spacing, size.min.y + (gui_settings::checkbox_size / 2)), gui_settings::text, false, { false , true });

		deeps::add_item((size.max - size.min) + text_spacing);
	}
}