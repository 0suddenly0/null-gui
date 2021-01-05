#include "../null-gui.h"

namespace null_gui {
	bool button(std::string text, vec2 size_arg) {
		window* wnd = deeps::current_window;
		if (!wnd) return false;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 left_spacing(gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_spacing : 0.f, 0.f);
		vec2 right_spacing(gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_spacing : wnd->get_scroll_offset(), 0.f);
		rect size(draw_pos + left_spacing, (size_arg != 0.f ? draw_pos + size_arg : vec2(gui_settings::items_size_full_window ? draw_pos.x + wnd->size.x - gui_settings::window_padding.x - (left_spacing.x * 2 + gui_settings::window_padding.x) : draw_pos.x + (gui_settings::button_padding.x * 2) + text_size.x, draw_pos.y + (gui_settings::button_padding.y * 2) + text_size.y)) + right_spacing);

		bool hovered, pressed;
		bool active = null_gui::deeps::button_behavior(size, &hovered, &pressed, name);

		wnd->draw_list->draw_rect_filled(size.min, size.max, hovered || pressed ? pressed ? gui_settings::main_color : gui_settings::button_bg_hovered : gui_settings::button_bg);
		wnd->draw_list->draw_text(deeps::format_item(draw_text), size.max - size.size() / 2, gui_settings::text, false, { true , true });
		
		deeps::add_item(size.size(), name);
		return active;
	}
}