#include "../null-gui.h"
//#include "../../utils/utils.h"

namespace null_gui {
	bool button(std::string text, vec2 size_arg) {
		window* wnd = deeps::get_current_window();
		if (!wnd) return false;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos;
		vec2 text_size = null_font::text_size(draw_text);
		vec2 left_spacing = vec2(gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_spacing : 0, 0.f);
		rect size(draw_pos + left_spacing, (size_arg != 0.f ? draw_pos + size_arg : vec2(gui_settings::items_size_full_window ? wnd->pos.x + wnd->size.x - gui_settings::window_padding.x - (left_spacing.x * 2) : draw_pos.x + (gui_settings::button_padding.x * 2) + text_size.x, draw_pos.y + (gui_settings::button_padding.y * 2) + text_size.y)) + left_spacing);
		bool hovered, pressed;
		bool active = null_gui::deeps::get_button_behavior(size, &hovered, &pressed, name);

		wnd->draw_list.add_rect(size.min, size.max, hovered || pressed ? pressed ? gui_settings::main_color : gui_settings::button_bg_hovered : gui_settings::button_bg);
		wnd->draw_list.add_text(deeps::format_item(draw_text), size.max - (size.max - size.min) / 2, color(255, 255, 255, 255), false, { true , true });
		deeps::add_item(size.max - size.min);

		return active;
	}
}