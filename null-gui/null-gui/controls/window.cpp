#include "../null-gui.h"
#include "../../null-render/directx9/null-render-dx9.h"

namespace null_gui {
	bool begin_window(std::string name, vec2 pos, vec2 size, bool* open) {
		return begin_window(name, pos, size, { window_flags::standart }, open);
	}

	bool begin_window(std::string name, vec2 pos, vec2 size, std::vector<window_flags> flags, bool* open) {
		window* wnd = nullptr;
		if (deeps::window_exist(name)) wnd = deeps::find_window(name);
		else wnd = deeps::add_window(name, pos, size, flags);
		if (!wnd) return false;

		std::string formated_name = deeps::format_item(name);
		bool have_title_bar = !wnd->flags.contains(window_flags::popup) && !wnd->flags.contains(window_flags::no_title_bar);
		color bg_color = wnd->flags.contains(window_flags::popup) ? gui_settings::popup_bg : wnd->flags.contains(window_flags::group) ? gui_settings::group_bg : gui_settings::window_bg;

		if ((wnd->flags.contains(window_flags::popup) || wnd->flags.contains(window_flags::group)) && deeps::current_window != wnd && wnd->parent_window != deeps::current_window) {
			wnd->parent_window = deeps::current_window;
		}

		if (open) wnd->visible = *open;
		if (!wnd->visible) return false;

		if (wnd->flags.contains(window_flags::set_pos)) wnd->pos = pos;
		if (wnd->flags.contains(window_flags::set_size)) {
			if (size.x > 1.f) wnd->size.x = size.x;
			if (size.y > 1.f) wnd->size.y = size.y;
		}

		wnd->clamp_on_screen();

		if (wnd->flags.contains(window_flags::popup) || wnd->close_call) { //logic for popups
			if (((null_input::get_key(null_input::key_id::mouse_left)->clicked() || null_input::get_key(null_input::key_id::mouse_right)->clicked()) && !wnd->in_popup_region()) || wnd->close_call) {
				int window_id = std::distance(wnd->parent_window->child_popup_window.begin(), std::find_if(wnd->parent_window->child_popup_window.begin(), wnd->parent_window->child_popup_window.end(), [=](window* wnd) { return wnd->name == wnd->name; }));
				if (window_id < wnd->parent_window->child_popup_window.size()) {
					wnd->parent_window->child_popup_window.erase(wnd->parent_window->child_popup_window.begin() + window_id);
					deeps::windows.erase(deeps::windows.begin() + wnd->idx);
					if (deeps::active_window_name == wnd->name) deeps::active_window_name = "";
					return false;
				}
			}
		}

		deeps::current_window = wnd;

		if (wnd->dragging && deeps::active_name == "") {
			if (!null_input::get_key(null_input::key_id::mouse_left)->down() || deeps::active_window_name != wnd->name) {
				wnd->drag_offset = vec2(0.f, 0.f);
				wnd->dragging = false;
				if (deeps::active_window_name == wnd->name) deeps::active_window_name = "";
			} else {
				wnd->pos = null_input::mouse_pos - wnd->drag_offset;

				wnd->clamp_on_screen();
			}
		}

		if (deeps::active_window_name == wnd->name || ((null_input::get_key(null_input::key_id::mouse_left)->clicked() || null_input::get_key(null_input::key_id::mouse_right)->clicked()) && null_input::mouse_in_region(wnd->pos, wnd->pos + wnd->size) && deeps::mouse_in_current_windows())) {
			deeps::focus_current_window();
		}

		wnd->draw_list->draw_rect_filled(wnd->pos, wnd->pos + wnd->size, bg_color, gui_settings::window_rounding, wnd->get_title_size() == 0.f ? null_render::corner_flags::all : null_render::corner_flags::bot);

		if (!wnd->flags.contains(window_flags::no_title_line)) wnd->draw_list->draw_rect_filled(wnd->pos, wnd->pos + vec2(wnd->size.x, gui_settings::window_title_line_size), gui_settings::main_color, gui_settings::window_title_rounding, null_render::corner_flags::top);
		if (have_title_bar) {
			deeps::push_var(&wnd->pos.y, wnd->pos.y + (wnd->flags.contains(window_flags::no_title_line) ? 0.f : gui_settings::window_title_line_size)); {
				wnd->draw_list->draw_rect_filled(wnd->pos, wnd->pos + vec2(wnd->size.x, gui_settings::window_title_size), gui_settings::window_title_bg);

				wnd->draw_list->push_clip_rect(wnd->pos, wnd->pos + vec2(wnd->size.x, gui_settings::window_title_size), wnd->flags.contains(window_flags::group)); {
					wnd->draw_list->push_clip_rect(wnd->pos, wnd->pos + vec2(wnd->size.x - gui_settings::window_title_size, gui_settings::window_title_size), true); {
						wnd->draw_list->draw_text(wnd->name, wnd->pos + vec2(5.f, gui_settings::window_title_size / 2), color(255, 255, 255, 255), false, { false, true });
						wnd->draw_list->draw_rect_filled_multicolor(wnd->pos + vec2(wnd->size.x - 45 - gui_settings::window_title_size, 0.f), wnd->pos + vec2(wnd->size.x - gui_settings::window_title_size, gui_settings::window_title_size), { color(gui_settings::window_title_bg, 0.f), gui_settings::window_title_bg }, { color(gui_settings::window_title_bg, 0.f), gui_settings::window_title_bg });
					} wnd->draw_list->pop_clip_rect();

					deeps::push_var(&wnd->draw_item_pos, wnd->pos + vec2(wnd->size.x - (gui_settings::window_title_size / 2) - (null_font::text_size("x").x / 2), (gui_settings::window_title_size / 2) - (null_font::text_size("x").y / 2))); {
						deeps::push_var(&wnd->ignore_scroll, true); {
							deeps::push_var(&gui_settings::items_size_full_window, false); {
								if (open && clickable_text("x")) {
									if (deeps::active_window_name == name) deeps::active_window_name = "";
									*open = false;
								}
							} deeps::pop_var();
						} deeps::pop_var();
					} deeps::pop_var();
				} wnd->draw_list->pop_clip_rect();
			} deeps::pop_var();
		}

		wnd->draw_item_pos = wnd->pos + vec2(0.f, wnd->get_title_size()) + gui_settings::window_padding;
		wnd->draw_list->push_clip_rect(wnd->pos + vec2(0.f, wnd->get_title_size()), wnd->pos + wnd->size, true);

		begin_scroll();

		wnd->max_size = vec2(0.f, 0.f);

		return true;
	}

	void end_window() {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		end_scroll();

		wnd->draw_list->pop_clip_rect();

		if (wnd->flags.contains(window_flags::auto_size)) { //auto-size logic
			if (wnd->arg_size.x < 1.f) wnd->size.x = wnd->max_size.x + gui_settings::window_padding.x - gui_settings::item_spacing;
			if (wnd->arg_size.y < 1.f) wnd->size.y = wnd->max_size.y + gui_settings::window_padding.y - gui_settings::item_spacing;
		}

		if (wnd->flags.contains(window_flags::popup) || wnd->flags.contains(window_flags::group)) deeps::current_window = wnd->parent_window;
		else deeps::current_window = nullptr;
	}
}