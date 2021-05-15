#include "../null-gui.h"
#include "../../null-render/directx9/null-render-dx9.h"

namespace null_gui {
	bool begin_window(std::string name, vec2 pos, vec2 size, bool* open) {
		return begin_window(name, pos, size, { window_flags::standart }, open);
	}

	bool begin_window(std::string name, vec2 pos, vec2 size, std::vector<window_flags> flags, bool* open) {
		window* this_window = nullptr;
		if (deeps::window_exist(name)) this_window = deeps::find_window(name);
		else this_window = deeps::add_window(name, pos, size, flags);
		if (!this_window) return false;

		std::string formated_name = deeps::format_item(name);
		bool have_title_bar = !this_window->flags.contains(window_flags::popup) && !this_window->flags.contains(window_flags::no_title_bar);
		color bg_color = this_window->flags.contains(window_flags::popup) ? gui_settings::popup_bg : this_window->flags.contains(window_flags::group) ? gui_settings::group_bg : gui_settings::window_bg;

		if ((this_window->flags.contains(window_flags::popup) || this_window->flags.contains(window_flags::group)) && deeps::current_window != this_window && this_window->parent_window != deeps::current_window) {
			this_window->parent_window = deeps::current_window;
		}

		if (open) this_window->visible = *open;
		if (!this_window->visible) return false;

		if (this_window->flags.contains(window_flags::set_pos)) this_window->pos = pos;
		if (this_window->flags.contains(window_flags::set_size)) {
			if (size.x > 1.f) this_window->size.x = size.x;
			if (size.y > 1.f) this_window->size.y = size.y;
		}

		this_window->clamp_on_screen();

		if (this_window->flags.contains(window_flags::popup) || this_window->close_call) { //logic for popups
			if (((null_input::get_key(null_input::key_id::mouse_left)->clicked() || null_input::get_key(null_input::key_id::mouse_right)->clicked()) && !this_window->in_popup_region()) || this_window->close_call) {
				int window_id = std::distance(this_window->parent_window->child_popup_window.begin(), std::find_if(this_window->parent_window->child_popup_window.begin(), this_window->parent_window->child_popup_window.end(), [=](window* wnd) { return wnd->name == this_window->name; }));
				if (window_id < this_window->parent_window->child_popup_window.size()) {
					this_window->parent_window->child_popup_window.erase(this_window->parent_window->child_popup_window.begin() + window_id);
					deeps::windows.erase(deeps::windows.begin() + this_window->idx);
					if (deeps::active_window_name == this_window->name) deeps::active_window_name = "";
					return false;
				}
			}
		}

		deeps::current_window = this_window;

		if (this_window->dragging && deeps::active_name == "") {
			if (!null_input::get_key(null_input::key_id::mouse_left)->down() || deeps::active_window_name != this_window->name) {
				this_window->drag_offset = vec2(0.f, 0.f);
				this_window->dragging = false;
				if (deeps::active_window_name == this_window->name) deeps::active_window_name = "";
			}
			else {
				this_window->pos = null_input::mouse_pos - this_window->drag_offset;

				this_window->clamp_on_screen();
			}
		}

		if (deeps::active_window_name == this_window->name || ((null_input::get_key(null_input::key_id::mouse_left)->clicked() || null_input::get_key(null_input::key_id::mouse_right)->clicked()) && null_input::mouse_in_region(this_window->pos, this_window->pos + this_window->size) && deeps::mouse_in_current_windows())) {
			deeps::focus_current_window();
		}

		this_window->draw_list->draw_rect_filled(this_window->pos, this_window->pos + this_window->size, bg_color, gui_settings::window_rounding, this_window->get_title_size() == 0.f ? null_render::corner_flags::all : null_render::corner_flags::bot);

		if (!this_window->flags.contains(window_flags::no_title_line)) this_window->draw_list->draw_rect_filled(this_window->pos, this_window->pos + vec2(this_window->size.x, gui_settings::window_title_line_size), gui_settings::main_color, gui_settings::window_title_rounding, null_render::corner_flags::top);
		if (have_title_bar) {
			deeps::push_var(&this_window->pos.y, this_window->pos.y + (this_window->flags.contains(window_flags::no_title_line) ? 0.f : gui_settings::window_title_line_size)); {
				this_window->draw_list->draw_rect_filled(this_window->pos, this_window->pos + vec2(this_window->size.x, gui_settings::window_title_size), gui_settings::window_title_bg);

				this_window->draw_list->push_clip_rect(this_window->pos, this_window->pos + vec2(this_window->size.x, gui_settings::window_title_size), this_window->flags.contains(window_flags::group)); {
					this_window->draw_list->push_clip_rect(this_window->pos, this_window->pos + vec2(this_window->size.x - gui_settings::window_title_size, gui_settings::window_title_size), true); {
						this_window->draw_list->draw_text(this_window->name, this_window->pos + vec2(5.f, gui_settings::window_title_size / 2), color(255, 255, 255, 255), false, { false, true });
						this_window->draw_list->draw_rect_filled_multicolor(this_window->pos + vec2(this_window->size.x - 45 - gui_settings::window_title_size, 0.f), this_window->pos + vec2(this_window->size.x - gui_settings::window_title_size, gui_settings::window_title_size), { color(gui_settings::window_title_bg, 0.f), gui_settings::window_title_bg }, { color(gui_settings::window_title_bg, 0.f), gui_settings::window_title_bg });
					} this_window->draw_list->pop_clip_rect();

					deeps::push_var(&this_window->draw_item_pos, this_window->pos + vec2(this_window->size.x - (gui_settings::window_title_size / 2) - (null_font::text_size("x").x / 2), (gui_settings::window_title_size / 2) - (null_font::text_size("x").y / 2))); {
						deeps::push_var(&this_window->ignore_scroll, true); {
							deeps::push_var(&gui_settings::items_size_full_window, false); {
								if (open && clickable_text("x")) {
									if (deeps::active_window_name == name) deeps::active_window_name = "";
									*open = false;
								}
							} deeps::pop_var();
						} deeps::pop_var();
					} deeps::pop_var();
				} this_window->draw_list->pop_clip_rect();
			} deeps::pop_var();
		}

		this_window->draw_item_pos = this_window->pos + vec2(0.f, this_window->get_title_size()) + gui_settings::window_padding;
		this_window->draw_list->push_clip_rect(this_window->pos + vec2(0.f, this_window->get_title_size()), this_window->pos + this_window->size, true);

		begin_scroll();

		this_window->max_size = vec2(0.f, 0.f);

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