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
		rect size(draw_pos, vec2(draw_pos.x + wnd->size.x - gui_settings::window_padding.x - gui_settings::window_padding.x, draw_pos.y + gui_settings::combo_size + text_size.y + gui_settings::text_spacing) + wnd->get_scroll_thickness());
		rect size_draw(vec2(size.min.x, size.min.y + text_size.y + gui_settings::text_spacing), size.max);
		rect size_draw_text(size_draw.min, size_draw.max - vec2(2.f, 2.f));
		deeps::text_input_info* input = deeps::text_input_info::add(new deeps::text_input_info{ name, value, value_text, size_draw });

		if (!input) return false;

		bool hovered, pressed;
		bool active = deeps::text_input_behavior(size_draw, &hovered, &pressed, name);

		float pos_in_text = input->get_text_offset(input->pos_in_text) - (null_font::text_size("|").x / 2.f);
		float size_selecting = input->get_size_select();

		wnd->draw_list->draw_text(draw_text, size.min, gui_settings::text, false);
		wnd->draw_list->draw_rect_filled(size_draw.min, size_draw.max, gui_settings::button_bg);

		wnd->draw_list->push_clip_rect(size_draw.min, size_draw_text.max, true); {
			if (input->is_selecting()) wnd->draw_list->draw_rect_filled(size_draw_text.min + vec2(2.f + input->get_text_offset(input->select_min), (size_draw_text.size().y / 2) - (value_size.y / 2)), size_draw_text.min + vec2(2.f + input->get_text_offset(input->select_min) + size_selecting, (size_draw_text.size().y / 2) + (value_size.y / 2)), gui_settings::select_text);
			wnd->draw_list->draw_text(value_text, size_draw_text.min + vec2(2.f, size_draw_text.size().y / 2), gui_settings::text, false, { false, true });
			if (active && !input->is_selecting() && input->show_pos) wnd->draw_list->draw_text("|", size_draw_text.min + vec2(2.f + pos_in_text, size_draw_text.size().y / 2), gui_settings::text, false, { false, true });
		} wnd->draw_list->pop_clip_rect();
		wnd->draw_list->draw_rect_filled(vec2(size_draw.min.x, size_draw.max.y - 1.f), size_draw.max, gui_settings::main_color);

		deeps::add_item(size.size(), name);
		return active;
	}
}