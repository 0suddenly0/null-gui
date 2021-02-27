#include "../null-gui.h"

float y_on_start = 0.f;
float first_size = 0.f;

namespace null_gui {
	void begin_columns(int count) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		deeps::push_var(&wnd->size.x, wnd->size.x / count);
		deeps::push_var(&wnd->draw_item_pos.x, wnd->draw_item_pos.x);
		y_on_start = wnd->draw_item_pos.y;
		first_size = wnd->size.x;
	}

	void next_column() {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		wnd->column_offset = first_size + gui_settings::window_padding.x;

		deeps::push_var(&wnd->draw_item_pos_prev, vec2(wnd->pos.x + first_size + gui_settings::window_padding.x, y_on_start)); {
			deeps::push_var(&gui_settings::item_spacing, 0.f); {
				//same_line();
				wnd->draw_item_pos = wnd->draw_item_pos_prev + vec2(gui_settings::item_spacing, 0.f);
			} deeps::pop_var();
		} deeps::pop_var();

		first_size += first_size;
	}

	void end_columns() {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		wnd->column_offset = 0.f;

		deeps::pop_var();
		deeps::pop_var();
	}
}