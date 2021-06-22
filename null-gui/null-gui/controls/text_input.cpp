#include "../null-gui.h"

namespace null_gui {
	bool text_input(std::string text, void* value, bool password, var_type type, std::string format) {
		window* wnd = deeps::current_window;
		if (!wnd) return false;

		std::string draw_text = deeps::format_item(text);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 min = text_size + gui_settings::text_spacing + gui_settings::combo_size + vec2(0.f, gui_settings::text_input_line_size);
		rect item_rect = rect(draw_pos, draw_pos + vec2(gui_settings::items_size_full_window ? math::max(min.x, wnd->get_window_size_with_padding()) : min.x, min.y));
		rect body_rect = rect(vec2(item_rect.min.x, item_rect.min.y + text_size.y + gui_settings::text_spacing), item_rect.max);
		rect working_rect = rect(body_rect.min + gui_settings::text_input_padding, body_rect.max - vec2(gui_settings::text_input_padding.x, gui_settings::text_input_padding.y + gui_settings::text_input_line_size));
		deeps::text_input_info* input = deeps::text_input_info::add(deeps::text_input_info(text, value, working_rect, type, format));

		if (!input) return false;

		vec2 value_size = null_font::text_size_w(input->converted_value);

		deeps::add_item(item_rect.size(), text);
		if (!wnd->can_draw_item(item_rect))
			return false;

		bool hovered, pressed;
		bool active = deeps::text_input_behavior(body_rect, &hovered, &pressed, text);

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

	bool text_input(std::string text, std::string* value, bool password) {
		return text_input(text, value, password, var_type::type_string, "%s");
	}

	bool text_input(std::string text, int* value) {
		return text_input(text, value, false, var_type::type_int, "%d");
	}

	bool text_input(std::string text, float* value, std::string format) {
		return text_input(text, value, false, var_type::type_float, format);
	}
}