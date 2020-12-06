#include "../null-gui.h"

namespace null_gui {
	void begin_scroll() {
		window* wnd = deeps::current_window;
		if (!wnd) return;
		if (wnd->have_flag(window_flags::auto_size) || wnd->max_scroll == 0) return;

		rect size(vec2(wnd->pos.x + wnd->size.x - gui_settings::scrollbar_thickness - gui_settings::scrollbar_padding.x, wnd->pos.y + gui_settings::window_title_size + gui_settings::scrollbar_padding.y), vec2(wnd->pos.x + wnd->size.x - gui_settings::scrollbar_padding.x, wnd->pos.y + wnd->size.y - gui_settings::scrollbar_padding.y));
		float size_bar = null_math::clamp(size.size().y * (wnd->size.y / wnd->max_size.y), 2.f, size.size().y);
		rect draw_size(vec2(size.min.x, size.min.y + size_bar / 2), vec2(size.max.x, size.max.y - size_bar / 2));
		float pos = draw_size.size().y * (abs(wnd->scroll_offset) / wnd->max_scroll);

		bool hovered, pressed;
		deeps::get_scroll_behavior(size, &hovered, &pressed);

		if (!pressed) {
			wnd->scroll_mouse_offset = 0.f;

			if (null_input::mouse_in_region(draw_size)) {
				if (null_input::mouse_in_region(draw_size.min + vec2(0.f, pos - size_bar / 2), vec2(draw_size.max.x, draw_size.min.y) + vec2(0.f, pos + size_bar / 2))) {
					wnd->scroll_mouse_offset = pos - (null_input::mouse_pos().y - draw_size.min.y);
				}
			} else if(null_input::mouse_in_region(size)) {
				wnd->scroll_mouse_offset = (null_input::mouse_pos().y < draw_size.min.y ? draw_size.min.y : draw_size.max.y) - null_input::mouse_pos().y;
			}
		} else if (pressed) {
			float mouse_pos = null_input::mouse_pos().y - draw_size.min.y + wnd->scroll_mouse_offset;
			wnd->scroll_offset = null_math::clamp(-(wnd->max_scroll * (mouse_pos / draw_size.size().y)), -(wnd->max_scroll), 0.f);
		}

		wnd->draw_list->add_rect(size.min, size.max, gui_settings::button_bg);
		wnd->draw_list->add_rect(vec2(draw_size.min.x, draw_size.min.y + pos - size_bar / 2), vec2(draw_size.max.x, draw_size.min.y + pos + size_bar / 2), gui_settings::main_color);
	}

	void end_scroll() {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		bool can_scroll = wnd->max_size.y > wnd->size.y && wnd->have_flag(window_flags::group) ? wnd->get_main_window()->hovered_group == wnd : deeps::hovered_window == wnd && wnd->hovered_group == nullptr;
		if (!deeps::hovered_window) {
			null_input::vars::mouse_wheel = 0;
		} else {
			if (null_input::vars::mouse_wheel != 0 && can_scroll) {
				if (wnd->have_flag(window_flags::auto_size)) {
					null_input::vars::mouse_wheel = 0;
					return;
				}
				wnd->scroll_offset += null_input::vars::mouse_wheel * 10.f;
				null_input::vars::mouse_wheel = 0;
			}
		}

		if (wnd->max_size.y > wnd->size.y && !wnd->have_flag(null_gui::window_flags::auto_size)) {
			wnd->max_scroll = ((wnd->max_size.y + gui_settings::window_padding.y - gui_settings::item_spacing) - wnd->size.y);
			wnd->scroll_offset = null_math::clamp(wnd->scroll_offset, -wnd->max_scroll, 0.f);
		} else {
			wnd->max_scroll = 0.f;
		}
	}
}