#include "../null-gui.h"
#include "../../utils/utils.h"

namespace null_gui {
	bool button(std::string text, vec2 size_arg) {
		window* wnd = deeps::get_current_window();
		if (!wnd) return false;
		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 text_size = null_font::text_size(draw_text);
		rect size(wnd->draw_item_pos, size_arg != 0.f ? wnd->draw_item_pos + size_arg : vec2(wnd->draw_item_pos.x + (gui_settings::button_padding.x * 2) + text_size.x, wnd->draw_item_pos.y + (gui_settings::button_padding.y * 2) + text_size.y));
		bool hovered, pressed;
		bool active = null_gui::deeps::get_button_behavior(size, &hovered, &pressed, name);

		wnd->draw_list.add_rect(size.min, size.max, hovered || pressed ? pressed ? gui_settings::main_color : gui_settings::button_bg_hovered : gui_settings::button_bg);
		wnd->draw_list.add_text(deeps::format_item(draw_text), size.max - (size.max - size.min) / 2, color(255, 255, 255, 255), false, { true , true });
		deeps::add_item(vec2(0.f, text_size.y + (gui_settings::button_padding.y * 2)));

		return active;
	}
}