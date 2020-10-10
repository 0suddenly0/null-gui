#include "../null-gui.h"

namespace null_gui {
	void slider_int(std::string text, int* value, int min, int max, std::string format) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		std::string formated_value = utils::format(format.c_str(), *value);
		vec2 draw_pos = wnd->draw_item_pos;
		vec2 text_size = null_font::text_size(draw_text);
		vec2 value_size = null_font::text_size(formated_value);
		vec2 left_spacing = vec2(gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_spacing : 0, 0.f);
		rect size(draw_pos + left_spacing, vec2(wnd->pos.x + wnd->size.x - gui_settings::window_padding.x - (left_spacing.x * 2), draw_pos.y + text_size.y + gui_settings::slider_size + gui_settings::text_spacing) + left_spacing);
		rect size_body(vec2(size.min.x, size.max.y - gui_settings::slider_size), size.max);
		int new_value = null_math::clamp(min + (max - min) * (null_input::mouse_pos().x - size.min.x) / (size.max.x - size.min.x), (float)min, (float)max);

		bool hovered, pressed;
		deeps::get_slider_behavior(size_body, &hovered, &pressed, name);

		if (hovered) {
			formated_value = utils::format(format.c_str(), new_value);
			value_size = null_font::text_size(formated_value);
		}

		wnd->draw_list.add_rect(size_body.min, size_body.max, hovered || pressed ? gui_settings::button_bg_hovered : gui_settings::button_bg);
		wnd->draw_list.add_text(draw_text, size.min, gui_settings::text, false);
		wnd->draw_list.add_text(formated_value, vec2(size.max.x - value_size.x, size.min.y), hovered ? color(100, 100, 100, 255) : gui_settings::text, false);

		if (pressed) *value = new_value;

		if (hovered) {
			float slider_size_hovered = ((float)(new_value - min) / (float)(max - min)) * (size.max.x - size.min.x);
			wnd->draw_list.add_rect(size_body.min, vec2(size_body.min.x + slider_size_hovered, size_body.max.y), color(gui_settings::main_color, 100));
		}

		float slider_size = ((float)(*value - min) / (float)(max - min)) * (size.max.x - size.min.x);
		wnd->draw_list.add_rect(size_body.min, vec2(size_body.min.x + slider_size, size_body.max.y), gui_settings::main_color);

		deeps::add_item(size.max - size.min);
	}

	void slider_float(std::string text, float* value, float min, float max, std::string format) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		std::string formated_value = utils::format(format.c_str(), *value);
		vec2 draw_pos = wnd->draw_item_pos;
		vec2 text_size = null_font::text_size(draw_text);
		vec2 value_size = null_font::text_size(formated_value);
		vec2 left_spacing(gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_spacing : 0, 0.f);
		rect size(draw_pos + left_spacing, vec2(wnd->pos.x + wnd->size.x - gui_settings::window_padding.x - (left_spacing.x * 2), draw_pos.y + text_size.y + gui_settings::slider_size + gui_settings::text_spacing) + left_spacing);
		rect size_body(vec2(size.min.x, size.max.y - gui_settings::slider_size), size.max);
		float new_value = null_math::clamp(min + (max - min) * (null_input::mouse_pos().x - size.min.x) / (size.max.x - size.min.x), min, max);

		bool hovered, pressed;
		deeps::get_slider_behavior(size_body, &hovered, &pressed, name);

		if (hovered) {
			formated_value = utils::format(format.c_str(), new_value);
			value_size = null_font::text_size(formated_value);
		}

		wnd->draw_list.add_rect(size_body.min, size_body.max, hovered || pressed ? gui_settings::button_bg_hovered : gui_settings::button_bg);
		wnd->draw_list.add_text(draw_text, size.min, gui_settings::text, false);
		wnd->draw_list.add_text(formated_value, vec2(size.max.x - value_size.x, size.min.y), hovered ? color(100, 100, 100, 255) : gui_settings::text, false);

		if (pressed) *value = new_value;

		if (hovered) {
			float slider_size_hovered = ((new_value - min) / (max - min)) * (size.max.x - size.min.x);
			wnd->draw_list.add_rect(size_body.min, vec2(size_body.min.x + slider_size_hovered, size_body.max.y), color(gui_settings::main_color, 100));
		}

		float slider_size = ((*value - min) / (max - min)) * (size.max.x - size.min.x);
		wnd->draw_list.add_rect(size_body.min, vec2(size_body.min.x + slider_size, size_body.max.y), gui_settings::main_color);

		deeps::add_item(size.max - size.min);
	}
}