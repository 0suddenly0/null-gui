#include "../null-gui.h"

void draw_check_mark(null_render::draw_list *draw_list, vec2 pos, color col, float size) {
	float thickness = math::max(size / 5.0f, 0.0f);
	size -= thickness * 0.5f;
	pos += vec2(thickness * 0.25f, thickness * 0.25f);

	float third = size / 3.0f;
	float bx = pos.x + third;
	float by = pos.y + size - third * 0.5f;
	draw_list->path_line_to(vec2(bx - third, by - third));
	draw_list->path_line_to(vec2(bx, by));
	draw_list->path_line_to(vec2(bx + third * 2.0f, by - third * 2.0f));
	draw_list->path_stroke(col, false, thickness);
}

namespace null_gui {
	void checkbox(std::string text, bool* value) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		rect size(draw_pos, draw_pos + vec2(gui_settings::checkbox_size + gui_settings::text_spacing + text_size.x, math::max(gui_settings::checkbox_size, text_size.y)));
		rect size_draw(vec2(size.min.x, size.centre().y - gui_settings::checkbox_size / 2), vec2(size.min.x + gui_settings::checkbox_size, size.centre().y + gui_settings::checkbox_size / 2));

		bool hovered, pressed;
		bool active = deeps::button_behavior(gui_settings::checkbox_hovered_with_text ? size : size_draw, &hovered, &pressed, name);

		if (active) *value = !*value;

		wnd->draw_list->draw_rect_filled(size_draw.min, size_draw.max, hovered || pressed ? gui_settings::button_bg_hovered : gui_settings::button_bg, gui_settings::checkbox_rounding);
		if (*value) {
			wnd->draw_list->draw_rect_filled(size_draw.min, size_draw.max, gui_settings::main_color, gui_settings::checkbox_rounding);
			float pad = math::max(1.0f, math::floor(gui_settings::checkbox_size / gui_settings::check_mark_size));
			draw_check_mark(wnd->draw_list, size_draw.min + vec2(pad, pad), color(1.f, 1.f, 1.f), gui_settings::checkbox_size - pad * 2.f);
		}
		wnd->draw_list->draw_text(deeps::format_item(draw_text), vec2(size.min.x + gui_settings::checkbox_size + gui_settings::text_spacing, size.centre().y), gui_settings::text, false, { false , true });

		deeps::add_item(size.size(), name);
	}
}