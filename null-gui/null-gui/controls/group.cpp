#include "../null-gui.h"

namespace null_gui {
	bool begin_group(std::string name, vec2 arg_size) {
		window* wnd = deeps::current_window;
		if (!wnd) return false;

		vec2 window_size = wnd->get_window_size_with_padding();
		vec2 group_size = vec2(arg_size.x == 0.f ? window_size.x : arg_size.x, arg_size.y == 0.f ? window_size.y : arg_size.y);
		bool result = begin_window(name, wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset()), group_size, { window_flags::set_pos, window_flags::set_size, window_flags::no_move, window_flags::group }, nullptr);
		return result;
	}

	void end_group() {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		end_window();

		rect item_rect(wnd->pos, wnd->pos + wnd->size);
		deeps::add_item(item_rect.size(), wnd->name);
	}
}