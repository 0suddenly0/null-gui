#include "../null-gui.h"

namespace null_gui {
	bool begin_window(std::string name, vec2 pos, vec2 size) {
		window* this_window = nullptr;
		int window_id = 0;
		if (deeps::find_window(name, &window_id)) this_window = deeps::find_window(name);
		else this_window = deeps::add_window(name, pos, size);
		if (!this_window) return false;

		deeps::current_window = this_window;

		if (deeps::active_name == name) {
			auto it = deeps::windows.begin() + window_id;
			std::rotate(it, it + 1, deeps::windows.end());
		}

		if (settings::clamp_window_on_screen) {
			this_window->pos.x = std::clamp(this_window->pos.x, 0.f, deeps::display_size.x - this_window->size.x);
			this_window->pos.y = std::clamp(this_window->pos.y, 0.f, deeps::display_size.y - this_window->size.y);
		}

		this_window->draw_list.add_rect(this_window->pos, this_window->pos + this_window->size, settings::window_bg);
		this_window->draw_list.add_rect(this_window->pos, this_window->pos + vec2(this_window->size.x, settings::window_title_size), settings::window_title_bg);
		this_window->draw_list.add_rect(this_window->pos + vec2(0.f, settings::window_title_size - 1), this_window->pos + vec2(this_window->size.x, settings::window_title_size), settings::main_color);
		this_window->draw_list.push_clip(this_window->pos, this_window->pos + vec2(this_window->size.x, settings::window_title_size));
		this_window->draw_list.add_text(name, this_window->pos + vec2(5.f, settings::window_title_size / 2), color(255, 255, 255, 255), false, { false, true });
		this_window->draw_list.pop_clip();

		if (null_input::get_mouse_key_state(0).clicked() && deeps::active_name.empty()) {
			for (int i = deeps::windows.size() - 1; i >= 0; i--) {
				window* wnd = deeps::windows[i];

				if (!wnd->dragging && null_input::mouse_in_region(wnd->pos, wnd->pos + vec2(wnd->size.x, settings::move_window_on_title_bar ? settings::window_title_size : wnd->size.y))) {
					wnd->drag_offset = null_input::mouse_pos() - wnd->pos;
					wnd->dragging = true;
					deeps::active_name = wnd->name;
					break;
				}
			}
		}

		this_window->draw_item_pos = this_window->pos + vec2(0.f, settings::window_title_size) + settings::window_padding;
		this_window->draw_list.push_clip(this_window->pos + vec2(0.f, settings::window_title_size), this_window->pos + this_window->size);

		if (this_window->dragging) {
			if (!null_input::get_mouse_key_state(0).down()) {
				this_window->drag_offset = vec2(0.f, 0.f);
				this_window->dragging = false;
				deeps::active_name = "";
			} else {
				this_window->pos = null_input::mouse_pos() - this_window->drag_offset;
			}
		}

		return true;
	}

	void end_window() {
		if (!deeps::get_current_window()) return;
		deeps::get_current_window()->draw_list.pop_clip();
		deeps::current_window = nullptr;
	}
}