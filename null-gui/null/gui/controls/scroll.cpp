#include "../gui.h"

void calc_scroll(rect& size, rect& draw_size, float& size_bar, float& pos) {
	null::gui::window* wnd = null::gui::detail::current_window;
	size = rect(vec2(wnd->pos.x + wnd->size.x - null::gui::style::scrollbar_thickness - null::gui::style::scrollbar_padding.x, wnd->pos.y + wnd->get_title_size() + null::gui::style::scrollbar_padding.y), vec2(wnd->pos.x + wnd->size.x - null::gui::style::scrollbar_padding.x, wnd->pos.y + wnd->size.y - null::gui::style::scrollbar_padding.y));
	size_bar = math::clamp(size.size().y * (wnd->size.y / wnd->max_size.y), 2.f, size.size().y);
	draw_size = rect(vec2(size.min.x, size.min.y + size_bar / 2), vec2(size.max.x, size.max.y - size_bar / 2));
	pos = draw_size.size().y * (abs(wnd->scroll_offset) / wnd->max_scroll);
}

namespace null {
	namespace gui {
		void begin_scroll() {
			window* wnd = detail::current_window;
			if(!wnd) return;
			if((wnd->flags.contains(window_flags::auto_size) && wnd->arg_size.y < 1.f) || wnd->max_scroll == 0) return;

			rect size, draw_size;
			float size_bar, pos;
			calc_scroll(size, draw_size, size_bar, pos);

			bool hovered, pressed;
			detail::controls_behavior::scroll(size, &hovered, &pressed);

			if(!pressed) {
				wnd->scroll_mouse_offset = 0.f;

				if(input::mouse_in_region(draw_size)) {
					if(input::mouse_in_region(draw_size.min + vec2(0.f, pos - size_bar / 2), vec2(draw_size.max.x, draw_size.min.y) + vec2(0.f, pos + size_bar / 2))) {
						wnd->scroll_mouse_offset = pos - (input::mouse_pos.y - draw_size.min.y);
					}
				} else if(input::mouse_in_region(vec2(vec2(draw_size.min.x, draw_size.min.y + pos - size_bar / 2)), vec2(draw_size.max.x, draw_size.min.y + pos + size_bar / 2))) {
					wnd->scroll_mouse_offset = draw_size.min.y + pos - input::mouse_pos.y;
				} else if(input::mouse_in_region(size)) {
					wnd->scroll_mouse_offset = (input::mouse_pos.y < draw_size.min.y ? draw_size.min.y : draw_size.max.y) - input::mouse_pos.y;
				}
			} else if(pressed) {
				float mouse_pos = input::mouse_pos.y - draw_size.min.y + wnd->scroll_mouse_offset;
				wnd->scroll_target = -(wnd->max_scroll * (mouse_pos / draw_size.size().y));
			}
		}

		void end_scroll() {
			window* wnd = detail::current_window;
			if(!wnd) return;

			if((!wnd->flags.contains(window_flags::auto_size) || wnd->arg_size.y > 1.f) && wnd->max_scroll != 0) {
				rect size, draw_size;
				float size_bar, pos;
				calc_scroll(size, draw_size, size_bar, pos);

				if(style::show_scrollbar_background) wnd->draw_list->draw_rect_filled(size.min, size.max, style::button_bg, style::scrollbar_rounding);
				wnd->draw_list->draw_rect_filled(vec2(draw_size.min.x, draw_size.min.y + pos - size_bar / 2), vec2(draw_size.max.x, draw_size.min.y + pos + size_bar / 2), style::main_color, style::scrollbar_rounding);
			}

			bool can_scroll = wnd->max_size.y > wnd->size.y && wnd->flags.contains(window_flags::group) ? wnd->get_main_window()->hovered_group == wnd : detail::hovered_window == wnd;
			if(!detail::hovered_window) {
				input::mouse_wheel = 0;
			} else {
				if(input::mouse_wheel != 0 && can_scroll) {
					if(wnd->flags.contains(window_flags::auto_size) && wnd->arg_size.y < 1.f) {
						input::mouse_wheel = 0;
						return;
					}
					wnd->scroll_target += input::mouse_wheel * 20.f;
					input::mouse_wheel = 0;
				}
			}

			if(wnd->max_size.y > wnd->size.y && (!wnd->flags.contains(window_flags::auto_size) || wnd->arg_size.y > 1.f)) {
				wnd->scroll_offset = math::clamp(roundf(math::lerp(wnd->scroll_offset, wnd->scroll_target, 0.2f)), -wnd->max_scroll, 0.f);

				wnd->max_scroll = ((wnd->max_size.y + style::window_padding.y - style::item_spacing) - wnd->size.y);
				wnd->scroll_target = math::clamp(wnd->scroll_target, -wnd->max_scroll, 0.f);
				wnd->scroll_offset = math::clamp(wnd->scroll_offset, -wnd->max_scroll, 0.f);
			} else {
				wnd->max_scroll = 0.f;
			}
		}
		namespace detail {
			namespace controls_behavior {
				void scroll(rect size, bool* hovered, bool* pressed) {
					window* wnd = detail::current_window;
					bool _hovered = false;
					bool _pressed = false;
					std::string name = utils::format("scrollbar##%s", wnd->name.c_str());

					if(can_use_item(size, name)) {
						hovered_name = name;

						if(!input::get_key(input::key_id::mouse_left)->down()) _hovered = true;
						if(input::get_key(input::key_id::mouse_left)->clicked()) active_name = name;

						if(active_name == name && input::get_key(input::key_id::mouse_left)->down()) _pressed = true;
					}

					if(!input::get_key(input::key_id::mouse_left)->down() && active_name == name) {
						active_name = "";
					}

					if(hovered) *hovered = _hovered;
					if(pressed) *pressed = _pressed;
				}
			}
		}
	}
}