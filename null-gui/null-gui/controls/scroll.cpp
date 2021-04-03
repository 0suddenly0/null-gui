#include "../null-gui.h"

void calc_scroll(rect& size, rect& draw_size, float& size_bar, float& pos) {
	null_gui::window* wnd = null_gui::deeps::current_window;
	size = rect(vec2(wnd->pos.x + wnd->size.x - null_gui::gui_settings::scrollbar_thickness - null_gui::gui_settings::scrollbar_padding.x, wnd->pos.y + wnd->get_title_size() + null_gui::gui_settings::scrollbar_padding.y), vec2(wnd->pos.x + wnd->size.x - null_gui::gui_settings::scrollbar_padding.x, wnd->pos.y + wnd->size.y - null_gui::gui_settings::scrollbar_padding.y));
	size_bar = math::clamp(size.size().y * (wnd->size.y / wnd->max_size.y), 2.f, size.size().y);
	draw_size = rect(vec2(size.min.x, size.min.y + size_bar / 2), vec2(size.max.x, size.max.y - size_bar / 2));
	pos = draw_size.size().y * (abs(wnd->scroll_offset) / wnd->max_scroll);
}

namespace null_gui {
	void begin_scroll() {
		window* wnd = deeps::current_window;
		if (!wnd) return;
		if ((wnd->flags.contains(window_flags::auto_size) && wnd->arg_size.y < 1.f) || wnd->max_scroll == 0) return;

		rect size, draw_size;
		float size_bar, pos;
		calc_scroll(size, draw_size, size_bar, pos);

		bool hovered, pressed;
		deeps::scroll_behavior(size, &hovered, &pressed);

		if (!pressed) {
			wnd->scroll_mouse_offset = 0.f;

			if (null_input::mouse_in_region(draw_size)) {
				if (null_input::mouse_in_region(draw_size.min + vec2(0.f, pos - size_bar / 2), vec2(draw_size.max.x, draw_size.min.y) + vec2(0.f, pos + size_bar / 2))) {
					wnd->scroll_mouse_offset = pos - (null_input::mouse_pos.y - draw_size.min.y);
				}
			} else if (null_input::mouse_in_region(vec2(vec2(draw_size.min.x, draw_size.min.y + pos - size_bar / 2)), vec2(draw_size.max.x, draw_size.min.y + pos + size_bar / 2))) {
				wnd->scroll_mouse_offset = draw_size.min.y + pos - null_input::mouse_pos.y;
			} else if (null_input::mouse_in_region(size)) {
				wnd->scroll_mouse_offset = (null_input::mouse_pos.y < draw_size.min.y ? draw_size.min.y : draw_size.max.y) - null_input::mouse_pos.y;
			}
		} else if (pressed) {
			float mouse_pos = null_input::mouse_pos.y - draw_size.min.y + wnd->scroll_mouse_offset;
			wnd->scroll_target = -(wnd->max_scroll * (mouse_pos / draw_size.size().y));
		}
	}

	void end_scroll() {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		if ((!wnd->flags.contains(window_flags::auto_size) || wnd->arg_size.y > 1.f) && wnd->max_scroll != 0) {
			rect size, draw_size;
			float size_bar, pos;
			calc_scroll(size, draw_size, size_bar, pos);

			if(gui_settings::show_scrollbar_background) wnd->draw_list->draw_rect_filled(size.min, size.max, gui_settings::button_bg, gui_settings::scrollbar_rounding);
			wnd->draw_list->draw_rect_filled(vec2(draw_size.min.x, draw_size.min.y + pos - size_bar / 2), vec2(draw_size.max.x, draw_size.min.y + pos + size_bar / 2), gui_settings::main_color, gui_settings::scrollbar_rounding);
		}

		bool can_scroll = wnd->max_size.y > wnd->size.y && wnd->flags.contains(window_flags::group) ? wnd->get_main_window()->hovered_group == wnd : deeps::hovered_window == wnd;
		if (!deeps::hovered_window) {
			null_input::mouse_wheel = 0;
		} else {
			if (null_input::mouse_wheel != 0 && can_scroll) {
				if (wnd->flags.contains(window_flags::auto_size) && wnd->arg_size.y < 1.f) {
					null_input::mouse_wheel = 0;
					return;
				}
				wnd->scroll_target += null_input::mouse_wheel * 20.f;
				null_input::mouse_wheel = 0;
			}
		}
		
		if (wnd->max_size.y > wnd->size.y && (!wnd->flags.contains(window_flags::auto_size) || wnd->arg_size.y > 1.f)) {
			wnd->scroll_offset = math::clamp(roundf(math::lerp(wnd->scroll_offset, wnd->scroll_target, 0.2f)), -wnd->max_scroll, 0.f);

			wnd->max_scroll = ((wnd->max_size.y + gui_settings::window_padding.y - gui_settings::item_spacing) - wnd->size.y);
			wnd->scroll_target = math::clamp(wnd->scroll_target, -wnd->max_scroll, 0.f);
			wnd->scroll_offset = math::clamp(wnd->scroll_offset, -wnd->max_scroll, 0.f);
		} else {
			wnd->max_scroll = 0.f;
		}
	}
}