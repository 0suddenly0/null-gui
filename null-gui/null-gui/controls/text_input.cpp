#include "../null-gui.h"

namespace null_gui {
	bool text_input(std::string text, std::string* value, bool password) {
		window* wnd = deeps::current_window;
		if (!wnd) return false;

		std::string value_text = *value;
		if (password) std::fill(value_text.begin(), value_text.end(), '*');
		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 value_size = null_font::text_size(value_text);
		vec2 min = text_size + gui_settings::text_spacing + gui_settings::combo_size + vec2(0.f, gui_settings::text_input_line_size);
		rect item_rect = rect(draw_pos, draw_pos + vec2(gui_settings::items_size_full_window ? math::max(min.x, wnd->get_window_size_with_padding()) : min.x, min.y));
		rect body_rect = rect(vec2(item_rect.min.x, item_rect.min.y + text_size.y + gui_settings::text_spacing), item_rect.max);
		rect working_rect = rect(body_rect.min + gui_settings::text_input_padding, body_rect.max - vec2(gui_settings::text_input_padding.x, gui_settings::text_input_padding.y + gui_settings::text_input_line_size));
		deeps::text_input_info* input = deeps::text_input_info::add(new deeps::text_input_info(name, value, working_rect));

		if (!input) return false;

		deeps::add_item(item_rect.size(), name);
		if (!wnd->can_draw_item(item_rect))
			return false;

		bool hovered, pressed;
		bool active = deeps::text_input_behavior(body_rect, &hovered, &pressed, name);

		wnd->draw_list->draw_text(draw_text, item_rect.min, gui_settings::text, false);
		wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max - vec2(0.f, gui_settings::text_input_line_size), gui_settings::button_bg, gui_settings::text_input_rounding, { null_render::corner_flags::top });

		wnd->draw_list->push_clip_rect(body_rect.min, body_rect.max, true); {
			if (input->is_selecting()) {
				float min_pos = input->get_text_offset(input->select_min);
				float max_pos = input->get_text_offset(input->select_max);
				wnd->draw_list->draw_rect_filled(working_rect.min + vec2(min_pos, (working_rect.size().y / 2) - (value_size.y / 2)), working_rect.min + vec2(max_pos, (working_rect.size().y / 2) + (value_size.y / 2)), gui_settings::select_text);
			}

			wnd->draw_list->draw_text(input->value_for_render, working_rect.min + vec2(0.f, working_rect.size().y / 2), gui_settings::text, false, { false, true });
			if (active && !input->is_selecting() && input->show_pos) wnd->draw_list->draw_text("|", working_rect.min + vec2(input->get_text_offset(input->pos_in_text) - 2.f, working_rect.size().y / 2), gui_settings::text, false, { false, true });
		} wnd->draw_list->pop_clip_rect();
		wnd->draw_list->draw_rect_filled(vec2(body_rect.min.x, body_rect.max.y - gui_settings::text_input_line_size), body_rect.max, gui_settings::main_color, gui_settings::text_input_rounding, { null_render::corner_flags::bot });

		return active;
	}
}