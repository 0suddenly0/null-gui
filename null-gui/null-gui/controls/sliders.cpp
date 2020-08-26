#include "../null-gui.h"
#include "../../utils/utils.h"

namespace null_gui {
	void slider_int(std::string text, int* value, int min, int max, std::string format) {
		window* wnd = deeps::get_current_window();
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

		bool hovered, pressed;
		deeps::get_slider_behavior(size_body, &hovered, &pressed, name);

		wnd->draw_list.add_rect(size_body.min, size_body.max, hovered || pressed ?  gui_settings::button_bg_hovered : gui_settings::button_bg);
		wnd->draw_list.add_text(draw_text, size.min, color(255, 255, 255, 255), false);
		wnd->draw_list.add_text(formated_value, vec2(size.max.x - value_size.x, size.min.y), color(255, 255, 255, 255), false);

		if(pressed) *value = null_math::clamp(min + (max - min) * (null_input::mouse_pos().x - size.min.x) / (size.max.x - size.min.x), (float)min, (float)max);

		float value_min_delta = *value - min;
		float max_min_delta = max - min;

		auto value_modifier = (value_min_delta / max_min_delta) * (size.max.x - size.min.x);

		wnd->draw_list.add_rect(size_body.min, vec2(size_body.min.x + value_modifier, size_body.max.y), gui_settings::main_color);

		deeps::add_item(size.max - size.min);
	}
}