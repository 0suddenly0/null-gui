#include "../null-gui.h"

namespace null_gui {
	void combo(std::string text, int* value, std::vector<std::string> items) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string draw_text = deeps::format_item(text);
		std::array<vec2, 3> arrow = math::calc_arrow(gui_settings::combo_arrow_size * wnd->draw_list->shared_data->font_size);
		float arrow_size = arrow.at(0).dist_to(arrow.at(2));
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 min = text_size + gui_settings::combo_size + vec2(arrow_size, gui_settings::text_spacing);
		rect item_rect = rect(draw_pos, draw_pos + vec2(gui_settings::items_size_full_window ? math::max(min.x, wnd->get_window_size_with_padding()) : min.x, min.y));
		rect body_rect = rect(vec2(item_rect.min.x, item_rect.min.y + text_size.y + gui_settings::text_spacing), item_rect.max);
		int clamped_value = math::clamp(*value, 0, (int)items.size() - 1);
		std::vector<window_flags> flags = { window_flags::popup, window_flags::set_pos, window_flags::set_size, window_flags::auto_size, window_flags::no_title_line };

		deeps::add_item(item_rect.size(), text);
		if (!wnd->can_draw_item(item_rect))
			return;

		std::string combo_popup_window = utils::format("##%s combo popup", text.c_str());

		bool hovered, pressed;
		bool open = deeps::combo_behavior(body_rect, &hovered, &pressed, combo_popup_window, flags);

		wnd->draw_list->draw_text(draw_text, item_rect.min, gui_settings::text, false);
		wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, hovered || pressed ? pressed ? gui_settings::button_bg_active : gui_settings::button_bg_hovered : gui_settings::button_bg, gui_settings::combo_rounding);

		vec2 arrow_pos = rect(vec2(body_rect.max.x - body_rect.size().y, body_rect.min.y), body_rect.max).centre();
		wnd->draw_list->draw_triangle_filled({ arrow_pos + arrow.at(0), arrow_pos + arrow.at(1), arrow_pos + arrow.at(2) }, gui_settings::main_color);

		wnd->draw_list->push_clip_rect(body_rect.min, body_rect.max - vec2(body_rect.max.y - body_rect.min.y, 0.f), true); {
			wnd->draw_list->draw_text(items[clamped_value], vec2(body_rect.min.x + gui_settings::text_spacing, body_rect.max.y - ((body_rect.max.y - body_rect.min.y) / 2)), gui_settings::text, false, { false, true });
		} wnd->draw_list->pop_clip_rect();

		if (deeps::window_exist(combo_popup_window)) {
			deeps::push_var(&gui_settings::window_padding, vec2(0.f, 0.f)); {
				deeps::push_var(&gui_settings::item_spacing, 0.f); {
					if (begin_window(combo_popup_window, vec2(body_rect.min.x, body_rect.max.y + gui_settings::combo_window_padding), vec2(body_rect.max.x - body_rect.min.x, 0.f), flags, nullptr)) {
						for (int i = 0; i < items.size(); i++) {
							if (i == gui_settings::max_auto_size_combo) {
								deeps::current_window->arg_size.y = deeps::current_window->size.y = deeps::current_window->max_size.y + gui_settings::window_padding.y - gui_settings::item_spacing;
								deeps::current_window->flags.remove(window_flags::auto_size);
							}

							if (selectable(items[i], *value == i)) *value = i;
						}
						end_window();
					}
				} deeps::pop_var();
			} deeps::pop_var();
		}
	}
}