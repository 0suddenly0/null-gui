#include "../gui.h"

void draw_check_mark(null::render::draw_list *draw_list, vec2 pos, color col, float size) {
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

namespace null {
	namespace gui {
		void checkbox(std::string text, bool* value) {
			window* wnd = detail::current_window;
			if(!wnd) return;

			std::string draw_text = detail::format_item(text);
			vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
			vec2 text_size = font::text_size(draw_text);
			rect item_rect = rect(draw_pos, draw_pos + vec2(style::checkbox_size + style::text_spacing + text_size.x, math::max(style::checkbox_size, text_size.y)));
			rect box_rect = rect(vec2(item_rect.min.x, item_rect.centre().y - style::checkbox_size / 2), vec2(item_rect.min.x + style::checkbox_size, item_rect.centre().y + style::checkbox_size / 2));

			detail::add_item(item_rect.size(), text);
			if(!wnd->can_draw_item(item_rect))
				return;

			bool hovered, pressed;
			bool active = detail::controls_behavior::button(style::checkbox_hovered_with_text ? item_rect : box_rect, &hovered, &pressed, text);

			if(active) *value = !*value;

			wnd->draw_list->draw_rect_filled(box_rect.min, box_rect.max, hovered || pressed ? style::button_bg_hovered : style::button_bg, style::checkbox_rounding);
			if(*value) {
				wnd->draw_list->draw_rect_filled(box_rect.min, box_rect.max, style::main_color, style::checkbox_rounding);
				float pad = math::max(1.0f, math::floor(style::checkbox_size / style::check_mark_size));
				draw_check_mark(wnd->draw_list.get(), box_rect.min + vec2(pad, pad), color(1.f, 1.f, 1.f), style::checkbox_size - pad * 2.f);
			}
			wnd->draw_list->draw_text(draw_text, vec2(item_rect.min.x + style::checkbox_size + style::text_spacing, item_rect.centre().y), style::text, false, { false , true });
		}
	}
}