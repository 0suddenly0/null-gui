#include "../null-gui.h"

namespace null_gui {
	bool selectable(std::string text, bool selected) {
		window* wnd = deeps::current_window;
		if (!wnd) return false;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(text);
		vec2 min = vec2(gui_settings::selectable_active_offset + gui_settings::selectable_offset + null_font::text_size(draw_text).x, text_size.y);
		float full_size = wnd->size.x - (null_gui::gui_settings::window_padding.x * 2) - wnd->get_scroll_thickness();
		rect size = rect(draw_pos, draw_pos + math::max(vec2(null_gui::gui_settings::items_size_full_window ? math::max(min.x, full_size) : min.x, min.y), vec2(0.f, 0.f)));
		rect size_draw = rect(size.min, size.max);

		bool hovered, pressed;
		bool active = deeps::button_behavior(size_draw, &hovered, &pressed, name);

		wnd->draw_list->draw_text(deeps::format_item(draw_text), vec2(size_draw.min.x + (selected ? gui_settings::selectable_active_offset : gui_settings::selectable_offset), draw_pos.y + (size_draw.size().y / 2)), selected ? gui_settings::main_color : hovered || pressed ? gui_settings::text_hovered : gui_settings::text, false, { false , true });

		deeps::add_item(size.size(), name);
		return active;
	}
}