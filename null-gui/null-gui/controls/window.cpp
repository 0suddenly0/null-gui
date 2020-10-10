#include "../null-gui.h"

namespace null_gui {
	bool begin_window(std::string name, vec2 pos, vec2 size, bool* open) {
		return begin_window(name, pos, size, { window_flags::standart }, open);
	}

	bool begin_window(std::string name, vec2 pos, vec2 size, std::vector<window_flags> flags, bool* open) {
		window* this_window = nullptr;
		if (deeps::find_window(name)) this_window = deeps::find_window(name);
		else this_window = deeps::add_window(name, pos, size, flags);
		if (!this_window) return false;

		std::string formated_name = deeps::format_item(name);
		bool have_title_bar = !this_window->have_flag(window_flags::popup) && !this_window->have_flag(window_flags::no_title_bar);
		color bg_color = this_window->have_flag(window_flags::popup) ? gui_settings::popup_bg : gui_settings::window_bg;

		if (open) {
			this_window->visible = *open;
			if (!*open) {
				return false;
			}
		}

		if (this_window->have_flag(window_flags::set_pos)) this_window->pos = pos;
		if (this_window->have_flag(window_flags::set_size)) {
			if (size.x > 1.f) this_window->size.x = size.x;
			if (size.y > 1.f) this_window->size.y = size.y;
		}

		if (gui_settings::clamp_window_on_screen) {
			this_window->pos.x = std::clamp(this_window->pos.x, 0.f, deeps::display_size.x - this_window->size.x);
			this_window->pos.y = std::clamp(this_window->pos.y, 0.f, deeps::display_size.y - this_window->size.y);
		}

		if (this_window->have_flag(window_flags::popup)) { //logic for popups
			if (null_input::get_mouse_key_state(0).down() && !this_window->in_child_region()) {
				deeps::windows.erase(deeps::windows.begin() + this_window->idx);
				if(deeps::active_window_name == this_window->name) deeps::active_window_name = "";
				return false;
			}
		}

		deeps::current_window = this_window;
		
		if (this_window->dragging && deeps::active_name == "") {
			if (!null_input::get_mouse_key_state(0).down() || deeps::active_window_name != this_window->name) {
				this_window->drag_offset = vec2(0.f, 0.f);
				this_window->dragging = false;
				if (deeps::active_window_name == this_window->name) deeps::active_window_name = "";
			} else {
				this_window->pos = null_input::mouse_pos() - this_window->drag_offset;
			}
		}

		if (deeps::active_window_name == this_window->name || (null_input::get_mouse_key_state(0).clicked() && null_input::mouse_in_region(this_window->pos, this_window->pos + this_window->size) && deeps::mouse_in_current_windows())) {
			deeps::focus_current_window();
		}

		this_window->draw_list.add_rect(this_window->pos, this_window->pos + this_window->size, bg_color);

		if (have_title_bar) {
			this_window->draw_list.add_rect(this_window->pos, this_window->pos + vec2(this_window->size.x, gui_settings::window_title_size), gui_settings::window_title_bg);

			this_window->draw_list.push_clip(this_window->pos, this_window->pos + vec2(this_window->size.x, gui_settings::window_title_size)); {
				this_window->draw_list.add_text(formated_name.c_str(), this_window->pos + vec2(5.f, gui_settings::window_title_size / 2), color(255, 255, 255, 255), false, { false, true });
				this_window->draw_list.add_rect_multicolor(this_window->pos + vec2(this_window->size.x - 45 - (open ? gui_settings::window_title_size - 1 : 0), 0.f), this_window->pos + vec2(this_window->size.x - (open ? gui_settings::window_title_size - 1 : 0), gui_settings::window_title_size - 1), { color(gui_settings::window_title_bg, 0), color(gui_settings::window_title_bg, 0) }, { gui_settings::window_title_bg, gui_settings::window_title_bg });

				deeps::push_var({ &this_window->draw_item_pos, this_window->pos + vec2(this_window->size.x - gui_settings::window_title_size + 1, 0.f) }); {
					deeps::push_var({ &gui_settings::spacing_checkbox_size, false }); {
						if (open && button("x", vec2(gui_settings::window_title_size, gui_settings::window_title_size))) {
							if (deeps::active_window_name == name) deeps::active_window_name = "";
							*open = false;
						}
					} deeps::pop_var();
				} deeps::pop_var();
			} this_window->draw_list.pop_clip();
		}

		if(!this_window->have_flag(window_flags::popup)) this_window->draw_list.add_rect(this_window->pos, this_window->pos + vec2(this_window->size.x, 1.f), gui_settings::main_color);

		this_window->draw_item_pos = this_window->pos + vec2(0.f, have_title_bar ? gui_settings::window_title_size : 0.f) + gui_settings::window_padding;
		this_window->draw_list.push_clip(this_window->pos + vec2(0.f, have_title_bar ? gui_settings::window_title_size : 0.f), this_window->pos + this_window->size);

		return true;
	}

	void end_window() {
		if (!deeps::current_window) return;

		deeps::current_window->draw_list.pop_clip();

		if (deeps::current_window->have_flag(window_flags::auto_size)) { //auto-size logic
			if (deeps::current_window->arg_size.x < 1.f) deeps::current_window->size.x = deeps::current_window->draw_item_pos.x - deeps::current_window->pos.x + gui_settings::window_padding.x - gui_settings::item_spacing;
			if (deeps::current_window->arg_size.y < 1.f) deeps::current_window->size.y = deeps::current_window->draw_item_pos.y - deeps::current_window->pos.y + gui_settings::window_padding.y;
		}

		if (deeps::current_window->have_flag(window_flags::popup)) deeps::current_window = deeps::current_window->parent_window;
		else deeps::current_window = nullptr;
	}
}