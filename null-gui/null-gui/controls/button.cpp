#include "../null-gui.h"
namespace null_gui {
	bool button(std::string text, vec2 size_arg) {
		window* wnd = deeps::current_window;
		if (!wnd) return false;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 min = text_size + (gui_settings::button_padding * 2);
		float full_size = wnd->size.x - (gui_settings::window_padding.x * 2) - wnd->get_scroll_thickness();
		rect size = rect(draw_pos, draw_pos + math::max(vec2(gui_settings::items_size_full_window ? math::max(min.x + wnd->get_scroll_thickness(), full_size) : min.x + wnd->get_scroll_thickness(), min.y), size_arg));

		bool hovered, pressed;
		bool active = deeps::button_behavior(size, &hovered, &pressed, name);

		wnd->draw_list->draw_rect_filled(size.min, size.max, hovered || pressed ? pressed ? gui_settings::main_color : gui_settings::button_bg_hovered : gui_settings::button_bg, gui_settings::button_rounding);
		wnd->draw_list->draw_text(deeps::format_item(draw_text), size.max - size.size() / 2, gui_settings::text, false, { true , true });
		
		wnd->draw_list->draw_rect_filled(size.min, size.max, color(255, 255, 255, 100));
		
		deeps::add_item(size.size(), name);
		return active;
	}
}