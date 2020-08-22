#include "../null-gui.h"

namespace null_gui {
	bool button(std::string text) {
		window* wnd = deeps::get_current_window();
		vec2 text_size = null_font::text_size(text);
		rect size(wnd->draw_item_pos, vec2(wnd->draw_item_pos.x + (settings::button_padding.x * 2) + text_size.x, wnd->draw_item_pos.y + (settings::button_padding.y * 2) + text_size.y));
		bool hovered, pressed;
		bool active = null_gui::deeps::get_button_behavior(size, &hovered, &pressed, text);

		wnd->draw_list.add_rect(size.min, size.max, hovered || pressed ? pressed ? settings::main_color : settings::button_bg_hovered : settings::button_bg);
		wnd->draw_list.add_text(text, size.max - vec2(settings::button_padding.x + (text_size.x / 2), settings::button_padding.y + (text_size.y / 2)), color(255, 255, 255, 255), false, { true , true });
		deeps::add_item(vec2(0.f, text_size.y + (settings::button_padding.y * 2)));

		return active;
	}
}