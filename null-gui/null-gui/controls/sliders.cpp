#include "../null-gui.h"

void local_text_input(rect item_rect, std::string item_name) {
	if (null_input::get_key(null_input::key_id::ctrl)->down() && null_input::click_mouse_in_region(item_rect) && null_input::mouse_in_region(item_rect) && null_input::get_key(null_input::key_id::mouse_left)->pressed() && null_gui::deeps::can_use_item(item_rect, item_name))
		null_gui::deeps::set_active_item(item_name);
}

std::string get_formated_value(std::string format, void* value, null_gui::var_type type) {
	switch (type) {
		case null_gui::var_type::type_int: return utils::format(format.c_str(), *(int*)value);
		case null_gui::var_type::type_float: return utils::format(format.c_str(), *(float*)value);
	}
}

bool create_text_input(std::string text_input_name, void* value, null_gui::var_type type, std::string format) {
	switch (type) {
		case null_gui::var_type::type_int: return text_input(text_input_name, value, false, type, format);
		case null_gui::var_type::type_float: return text_input(text_input_name, value, false, type, format);
	}
}

void set(void* value, void* new_value, null_gui::var_type type) {
	switch (type) {
		case null_gui::var_type::type_int: *(int*)value = *(int*)new_value;
		case null_gui::var_type::type_float: *(float*)value = *(float*)new_value;
	}
}

namespace null_gui {
	void slider_int(std::string text, int* value, int min_value, int max_value, std::string format) {
		slider(text, value, &min_value, &max_value, format, 0, var_type::type_int);
	}

	void slider_float(std::string text, float* value, float min_value, float max_value, std::string format, int round) {
		slider(text, value, &min_value, &max_value, format, round, var_type::type_float);
	}

	void slider(std::string text, void* value, void* min_value, void* max_value, std::string format, int round, var_type type) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string text_text_input = utils::format("%s##text_input", text.c_str());
		std::string name_text_input = utils::format("%s##%s", text_text_input.c_str(), wnd->name.c_str());

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		std::string formated_value = get_formated_value(format, value, type);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 value_size = null_font::text_size(formated_value);
		vec2 min = text_size + gui_settings::text_spacing + vec2(null_font::text_size(get_formated_value(format, max_value, type)).x, gui_settings::slider_size);
		rect item_rect = rect(draw_pos, draw_pos + vec2(gui_settings::items_size_full_window ? math::max(min.x, wnd->get_window_size_with_padding()) : min.x, min.y));
		rect body_rect = rect(vec2(item_rect.min.x, item_rect.max.y - gui_settings::slider_size), item_rect.max);
		rect value_rect = rect(vec2(item_rect.max.x - value_size.x, item_rect.min.y), vec2(item_rect.max.x, item_rect.min.y + value_size.y));
		float calced_new_value = math::clamp(*(float*)min_value + (*(float*)max_value - *(float*)min_value) * (null_input::mouse_pos.x - item_rect.min.x) / item_rect.size().x, *(float*)min_value, *(float*)max_value);
		float new_value = round == 0 && type != var_type::type_float ? calced_new_value : floor(calced_new_value * round / (float)round);
		float clamped_value = math::clamp(*(float*)value, *(float*)min_value, *(float*)max_value);

		deeps::add_item(item_rect.size(), name);
		if (!wnd->can_draw_item(item_rect))
			return;

		local_text_input(value_rect, name_text_input);

		if (deeps::active_name == name_text_input) {
			wnd->change_current_item();
			create_text_input(text_text_input, value, type, format);
			return;
		}

		bool hovered, pressed;
		deeps::slider_behavior(body_rect, &hovered, &pressed, name);

		if (hovered) {
			formated_value = get_formated_value(format.c_str(), &new_value, type);
			value_size = null_font::text_size(formated_value);
		}

		wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, hovered || pressed ? gui_settings::button_bg_hovered : gui_settings::button_bg, gui_settings::slider_rounding);
		wnd->draw_list->draw_text(draw_text, item_rect.min, gui_settings::text, false);
		wnd->draw_list->draw_text(formated_value, vec2(item_rect.max.x - value_size.x, item_rect.min.y), hovered ? color(100, 100, 100, 255) : gui_settings::text, false);

		if (pressed) set(value, &new_value, type);

		if (hovered) {
			float slider_size_hovered = ((new_value - *(float*)min_value) / (*(float*)max_value - *(float*)min_value))* item_rect.size().x;
			wnd->draw_list->push_clip_rect(body_rect.min, vec2(body_rect.min.x + slider_size_hovered, body_rect.max.y), true); {
				wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, color(gui_settings::main_color, 100), gui_settings::slider_rounding);
			} wnd->draw_list->pop_clip_rect();
		}

		float slider_size = ((clamped_value - *(float*)min_value) / (*(float*)max_value - *(float*)min_value)) * item_rect.size().x;
		wnd->draw_list->push_clip_rect(body_rect.min, vec2(body_rect.min.x + slider_size, body_rect.max.y), true); {
			wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, gui_settings::main_color, gui_settings::slider_rounding);
		} wnd->draw_list->pop_clip_rect();
	}
}