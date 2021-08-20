#include "../gui.h"

float draw_item_pos_before = 0.f;
float current_offset = 0.f;

namespace null {
	namespace gui {
		void begin_columns(int count) {
			window* wnd = detail::current_window;
			if(!wnd) return;

			float offset = (wnd->draw_item_pos.x - wnd->pos.x - wnd->get_padding().x);
			detail::push_var(&wnd->size.x, (wnd->size.x - offset) / count);
			detail::push_var(&wnd->draw_item_pos.x, wnd->draw_item_pos.x);
			detail::push_var(&draw_item_pos_before, wnd->draw_item_pos.y);
			detail::push_var(&current_offset, wnd->size.x + offset);
			detail::push_var(&wnd->in_column, true);
			detail::push_var(&wnd->column_offset, wnd->column_offset);
		}

		void next_column() {
			window* wnd = detail::current_window;
			if(!wnd) return;

			wnd->column_offset = current_offset + wnd->get_padding().x;

			detail::push_var(&wnd->draw_item_pos_prev, vec2(wnd->pos.x + current_offset + wnd->get_padding().x, draw_item_pos_before)); {
				detail::push_var(&style::item_spacing, 0.f); {
					wnd->draw_item_pos = wnd->draw_item_pos_prev + vec2(style::item_spacing, 0.f);
				} detail::pop_var();
			} detail::pop_var();

			current_offset += current_offset;
		}

		void end_columns() {
			window* wnd = detail::current_window;
			if(!wnd) return;

			detail::pop_var(); //wnd->column_offset
			detail::pop_var(); //wnd->in_column
			detail::pop_var(); //current_offset
			detail::pop_var(); //draw_item_pos_before
			detail::pop_var(); //wnd->draw_item_pos.x
			detail::pop_var(); //wnd->size.x
		}
	}
}