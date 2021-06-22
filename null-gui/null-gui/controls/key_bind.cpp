#include "../null-gui.h"

namespace null_gui{
	void key_bind(std::string text, null_input::bind_key* bind) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string draw_text = deeps::format_item(text);
		std::string bind_text = bind->key->data.name;

		if (bind->binding) bind_text = "...";
		else if (bind->key->data.id == 0) bind_text = "-";

		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 bind_text_size = null_font::text_size(bind_text);
		vec2 min = vec2(text_size.x + (draw_text.empty() ? 0.f : gui_settings::text_spacing) + bind_text_size.x + gui_settings::button_padding.x * 2, text_size.y + gui_settings::button_padding.y * 2);

		if (draw_text.empty()) draw_pos += vec2(wnd->get_window_size_with_padding() - min.x, 0.f);
		
		rect item_rect = rect(draw_pos, draw_pos + vec2(gui_settings::items_size_full_window && !draw_text.empty() ? math::max(min.x, wnd->get_window_size_with_padding()) : min.x, min.y));
		rect button_rect = rect(vec2(item_rect.max.x - (bind_text_size.x + gui_settings::button_padding.x * 2), item_rect.max.y - (text_size.y + gui_settings::button_padding.y * 2 )), item_rect.max);

		if (!draw_text.empty())
			wnd->draw_list->draw_text(draw_text, vec2(item_rect.min.x, item_rect.min.y + item_rect.size().y / 2), gui_settings::text, false, { false, true });

		deeps::add_item(item_rect.size(), text);
		if (!wnd->can_draw_item(item_rect))
			return;

		std::string keybind_tooltip_window = utils::format("##%s keybind tooltip", text.c_str());

		bool hovered;
		bool active = null_gui::deeps::key_bind_behavior(bind, button_rect, &hovered, text);

		wnd->draw_list->draw_rect_filled(button_rect.min, button_rect.max, active ? gui_settings::button_bg_active : hovered ? gui_settings::button_bg_hovered : gui_settings::button_bg, gui_settings::key_bind_rounding);
		wnd->draw_list->draw_text(bind_text, button_rect.min + button_rect.size() / 2, gui_settings::text, false, { true, true });

		if (deeps::window_exist(keybind_tooltip_window)) {
			deeps::push_var(&gui_settings::window_padding, vec2(0.f, 0.f)); {
				deeps::push_var(&gui_settings::item_spacing, 0.f); {
					deeps::push_var(&gui_settings::selectable_active_offset, gui_settings::selectable_active_offset); {
						if (begin_window(keybind_tooltip_window, vec2(button_rect.max.x, button_rect.min.y), vec2(0.f, 0.f), { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size }, nullptr)) {
							std::vector<std::string> bind_types{ "always", "hold on", "hold off", "toggle" };
							for (int i = 0; i < bind_types.size(); i++) {
								if (selectable(bind_types[i], bind->type == (null_input::bind_type)i))
									bind->type = (null_input::bind_type)i;
							}
							end_window();
						}
					} deeps::pop_var();
				} deeps::pop_var();
			} deeps::pop_var();
		}
	}
}