#include "../null-gui.h"
namespace null_gui {
	bool button(std::string text, vec2 size_arg) {
		window* wnd = deeps::current_window;
		if (!wnd) return false;

		std::string draw_text = deeps::format_item(text);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 min = text_size + (gui_settings::button_padding * 2);
		rect button_rect = rect(draw_pos, draw_pos + math::max(vec2(gui_settings::items_size_full_window ? math::max(min.x + wnd->get_scroll_thickness(), wnd->get_window_size_with_padding()) : min.x + wnd->get_scroll_thickness(), min.y), size_arg));

		deeps::add_item(button_rect.size(), text);
		if (!wnd->can_draw_item(button_rect))
			return false;

		bool hovered, pressed;
		bool active = deeps::button_behavior(button_rect, &hovered, &pressed, text);

		wnd->draw_list->draw_rect_filled(button_rect.min, button_rect.max, hovered || pressed ? pressed ? gui_settings::main_color : gui_settings::button_bg_hovered : gui_settings::button_bg, gui_settings::button_rounding);
		wnd->draw_list->draw_text(draw_text, button_rect.centre(), gui_settings::text, false, { true , true });

		return active;
	}
}