#include "../null-gui.h"

namespace null_gui {
	void slider_int(std::string text, int* value, int min_value, int max_value, std::string format, int step, int ctrl_step) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		std::string formated_value = utils::format(format.c_str(), *value);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 plus_size = null_font::text_size("+ ");
		vec2 minus_size = null_font::text_size("- ");
		vec2 value_size = null_font::text_size(formated_value);
		vec2 min = text_size + gui_settings::text_spacing + vec2(plus_size.x + minus_size.x + null_font::text_size(utils::format(format.c_str(), max_value)).x, gui_settings::slider_size);
		float full_size = wnd->size.x - (null_gui::gui_settings::window_padding.x * 2) - wnd->get_scroll_thickness();
		rect item_rect = rect(draw_pos, draw_pos + vec2(null_gui::gui_settings::items_size_full_window ? math::max(min.x, full_size) : min.x, min.y));
		rect body_rect(vec2(item_rect.min.x, item_rect.max.y - gui_settings::slider_size), item_rect.max);
		int new_value = math::clamp(min_value + (max_value - min_value) * (null_input::mouse_pos.x - item_rect.min.x) / item_rect.size().x, (float)min_value, (float)max_value);
		int clamped_value = math::clamp(*value, min_value, max_value);

		bool hovered, pressed;
		deeps::slider_behavior(body_rect, &hovered, &pressed, name);

		if (hovered) {
			formated_value = utils::format(format.c_str(), new_value);
			value_size = null_font::text_size(formated_value);
		}

		wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, hovered || pressed ? gui_settings::button_bg_hovered : gui_settings::button_bg, gui_settings::slider_rounding);
		wnd->draw_list->draw_text(draw_text, item_rect.min, gui_settings::text, false);
		wnd->draw_list->draw_text(formated_value, vec2(item_rect.max.x - value_size.x, item_rect.min.y), hovered ? color(100, 100, 100, 255) : gui_settings::text, false);

		if (null_input::mouse_in_region(item_rect)) {
			deeps::push_var(&wnd->draw_item_pos, vec2(item_rect.max.x - value_size.x - null_font::text_size("+ ").x, item_rect.min.y - wnd->get_scroll_offset())); {
				if (clickable_text(utils::format("+##%s", name.c_str()))) {
					if (null_input::get_key("ctrl")->down()) *value += ctrl_step;
					else *value += step;
				}
			} deeps::pop_var();

			deeps::push_var(&wnd->draw_item_pos, vec2(item_rect.max.x - value_size.x - null_font::text_size("+ ").x - null_font::text_size("- ").x, item_rect.min.y - wnd->get_scroll_offset())); {
				if (clickable_text(utils::format("-##%s", name.c_str()))) {
					if (null_input::get_key("ctrl")->down()) *value -= ctrl_step;
					else *value -= step;
				}
			} deeps::pop_var();

			*value = math::clamp(*value, min_value, max_value);
		}

		if (pressed) *value = new_value;

		if (hovered) {
			float slider_size_hovered = ((float)(new_value - min_value) / (float)(max_value - min_value)) * item_rect.size().x;
			wnd->draw_list->push_clip_rect(body_rect.min, vec2(body_rect.min.x + slider_size_hovered, body_rect.max.y), true); {
				wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, color(gui_settings::main_color, 100), gui_settings::slider_rounding);
			} wnd->draw_list->pop_clip_rect();
		}

		float slider_size = ((float)(clamped_value - min_value) / (float)(max_value - min_value)) * item_rect.size().x;
		wnd->draw_list->push_clip_rect(body_rect.min, vec2(body_rect.min.x + slider_size, body_rect.max.y), true); {
			wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, gui_settings::main_color, gui_settings::slider_rounding);
		} wnd->draw_list->pop_clip_rect();

		deeps::add_item(item_rect.size(), name);
	}

	void slider_float(std::string text, float* value, float min_value, float max_value, std::string format, float step, float ctrl_step) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		std::string formated_value = utils::format(format.c_str(), *value);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 plus_size = null_font::text_size("+ ");
		vec2 minus_size = null_font::text_size("- ");
		vec2 value_size = null_font::text_size(formated_value);
		vec2 min = text_size + gui_settings::text_spacing + vec2(plus_size.x + minus_size.x + null_font::text_size(utils::format(format.c_str(), max_value)).x, gui_settings::slider_size);
		float full_size = wnd->size.x - (gui_settings::window_padding.x * 2) - wnd->get_scroll_thickness();
		rect item_rect = rect(draw_pos, draw_pos + vec2(gui_settings::items_size_full_window ? math::max(min.x, full_size) : min.x, min.y));
		rect body_rect = rect(vec2(item_rect.min.x, item_rect.max.y - gui_settings::slider_size), item_rect.max);
		float new_value = math::clamp(min_value + (max_value - min_value) * (null_input::mouse_pos.x - item_rect.min.x) / item_rect.size().x, min_value, max_value);
		float clamped_value = math::clamp(*value, min_value, max_value);

		bool hovered, pressed;
		deeps::slider_behavior(body_rect, &hovered, &pressed, name);

		if (hovered) {
			formated_value = utils::format(format.c_str(), new_value);
			value_size = null_font::text_size(formated_value);
		}

		wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, hovered || pressed ? gui_settings::button_bg_hovered : gui_settings::button_bg, gui_settings::slider_rounding);
		wnd->draw_list->draw_text(draw_text, item_rect.min, gui_settings::text, false);
		wnd->draw_list->draw_text(formated_value, vec2(item_rect.max.x - value_size.x, item_rect.min.y), hovered ? color(100, 100, 100, 255) : gui_settings::text, false);

		if (null_input::mouse_in_region(item_rect)) {
			deeps::push_var(&wnd->draw_item_pos, vec2(item_rect.max.x - value_size.x - plus_size.x, item_rect.min.y - wnd->get_scroll_offset())); {
				if (clickable_text(utils::format("+##%s", name.c_str()))) {
					if (null_input::get_key("ctrl")->down()) *value += ctrl_step;
					else *value += step;
				}
			} deeps::pop_var();

			deeps::push_var(&wnd->draw_item_pos, vec2(item_rect.max.x - value_size.x - plus_size.x - minus_size.x, item_rect.min.y - wnd->get_scroll_offset())); {
				if (clickable_text(utils::format("-##%s", name.c_str()))) {
					if (null_input::get_key("ctrl")->down()) *value -= ctrl_step;
					else *value -= step;
				}
			} deeps::pop_var();

			*value = math::clamp(*value, min_value, max_value);
		}

		if (pressed) *value = new_value;

		if (hovered) {
			float slider_size_hovered = ((new_value - min_value) / (max_value - min_value)) * item_rect.size().x;
			wnd->draw_list->push_clip_rect(body_rect.min, vec2(body_rect.min.x + slider_size_hovered, body_rect.max.y), true); {
				wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, color(gui_settings::main_color, 100), gui_settings::slider_rounding);
			} wnd->draw_list->pop_clip_rect();
		}

		float slider_size = ((clamped_value - min_value) / (max_value - min_value)) * item_rect.size().x;
		wnd->draw_list->push_clip_rect(body_rect.min, vec2(body_rect.min.x + slider_size, body_rect.max.y), true); {
			wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, gui_settings::main_color, gui_settings::slider_rounding);
		} wnd->draw_list->pop_clip_rect();

		deeps::add_item(item_rect.size(), name);
	}
}