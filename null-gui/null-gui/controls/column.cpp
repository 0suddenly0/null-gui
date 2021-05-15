#include "../null-gui.h"

float draw_item_pos_before = 0.f;
float current_offset = 0.f;

namespace null_gui {
	void begin_columns(int count) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		deeps::push_var(&wnd->size.x, wnd->size.x / count);
		deeps::push_var(&wnd->draw_item_pos.x, wnd->draw_item_pos.x);
		deeps::push_var(&draw_item_pos_before, wnd->draw_item_pos.y);
		deeps::push_var(&current_offset, wnd->size.x);
	}

	void next_column() {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		deeps::push_var(&wnd->column_offset, current_offset + gui_settings::window_padding.x);

		deeps::push_var(&wnd->draw_item_pos_prev, vec2(wnd->pos.x + current_offset + gui_settings::window_padding.x, draw_item_pos_before)); {
			deeps::push_var(&gui_settings::item_spacing, 0.f); {
				wnd->draw_item_pos = wnd->draw_item_pos_prev + vec2(gui_settings::item_spacing, 0.f);
			} deeps::pop_var();
		} deeps::pop_var();

		current_offset += current_offset;
	}

	void end_columns() {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		deeps::pop_var(); //wnd->column_offset
		deeps::pop_var(); //current_offset
		deeps::pop_var(); //draw_item_pos_before
		deeps::pop_var(); //wnd->draw_item_pos.x
		deeps::pop_var(); //wnd->size.x
	}
}