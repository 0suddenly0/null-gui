#include "../null-gui.h"

namespace null_gui {
	bool selectable(std::string text, bool selected) {
		window* wnd = deeps::current_window;
		if (!wnd) return false;

		std::string draw_text = deeps::format_item(text);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(text);
		vec2 min = vec2(gui_settings::selectable_active_offset + gui_settings::selectable_offset + null_font::text_size(draw_text).x, text_size.y);
		rect item_rect = rect(draw_pos, draw_pos + math::max(vec2(null_gui::gui_settings::items_size_full_window ? math::max(min.x, wnd->get_window_size_with_padding()) : min.x, min.y), vec2(0.f, 0.f)));

		deeps::add_item(item_rect.size(), text);
		if (!wnd->can_draw_item(item_rect))
			return false;

		bool hovered, pressed;
		bool active = deeps::button_behavior(item_rect, &hovered, &pressed, text);

		wnd->draw_list->draw_text(draw_text, vec2(item_rect.min.x + (selected ? gui_settings::selectable_active_offset : gui_settings::selectable_offset), item_rect.centre().y), selected ? gui_settings::main_color : hovered || pressed ? gui_settings::text_hovered : gui_settings::text, false, { false , true });

		return active;
	}
}