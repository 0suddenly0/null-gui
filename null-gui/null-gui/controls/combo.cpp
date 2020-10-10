#include "../null-gui.h"

namespace null_gui {
	void combo(std::string text, int* value, std::vector<std::string> items) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos;
		vec2 text_size = null_font::text_size(draw_text);
		vec2 left_spacing(gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_spacing : 0, 0.f);
		rect size(draw_pos + left_spacing, vec2(wnd->pos.x + wnd->size.x - gui_settings::window_padding.x - (left_spacing.x * 2), draw_pos.y + gui_settings::combo_size + text_size.y + gui_settings::text_spacing) + left_spacing);
		rect size_draw(vec2(size.min.x, size.min.y + text_size.y + gui_settings::text_spacing), size.max);
		int clamped_value = null_math::clamp(*value, 0, (int)items.size() - 1);
		std::vector<window_flags> flags = { window_flags::popup, window_flags::set_pos, window_flags::set_size, window_flags::auto_size };

		bool hovered, pressed;
		bool open = deeps::get_combo_behavior(size_draw, 5, &hovered, &pressed, utils::format("##%s comboname", text.c_str()), flags);

		wnd->draw_list.add_text(draw_text, size.min, gui_settings::text, false);
		wnd->draw_list.add_rect(size_draw.min, size_draw.max, gui_settings::button_bg);
		wnd->draw_list.add_rect(vec2(size_draw.max.x - (size_draw.max.y - size_draw.min.y), size_draw.min.y), size_draw.max, hovered || pressed ? gui_settings::button_bg_active : gui_settings::button_bg_hovered);
		wnd->draw_list.add_text(items[clamped_value], vec2(size_draw.min.x + gui_settings::text_spacing, size_draw.max.y - ((size_draw.max.y - size_draw.min.y) / 2)), gui_settings::text, false, { false, true });

		deeps::add_item(size.max - size.min);

		if (deeps::find_window(utils::format("##%s comboname", text.c_str()))) {
			deeps::push_var(gui_var(&gui_settings::window_padding, vec2(0.f, 0.f))); {
				deeps::push_var(gui_var(&gui_settings::item_spacing, 0.f)); {
					if (begin_window(utils::format("##%s comboname", text.c_str()), vec2(size_draw.min.x, size_draw.max.y), vec2(size_draw.max.x - size_draw.min.x, 0.f), flags, nullptr)) {
						for (int i = 0; i < items.size(); i++) {
							if (selectable(items[i], *value == i)) *value = i;
						}
						end_window();
					}
				} deeps::pop_var();
			} deeps::pop_var();
		}
	}
}