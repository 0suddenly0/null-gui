#include "../gui.h"

namespace null {
	namespace gui {
		bool begin_window(std::string name, vec2 pos, vec2 size, bool* open) {
			return begin_window(name, pos, size, { }, open);
		}

		bool begin_window(std::string name, vec2 pos, vec2 size, flags_list<window_flags> flags, bool* open) {
			window* wnd = nullptr;
			if(detail::window_exist(name)) wnd = detail::find_window(name);
			else wnd = detail::add_window(name, pos, size, flags);
			if(!wnd) return false;

			std::string formated_name = detail::format_item(name);
			bool have_title_bar = !wnd->flags.contains(window_flags::popup) && !wnd->flags.contains(window_flags::no_title_bar);
			color bg_color = wnd->flags.contains(window_flags::popup) ? style::popup_bg : wnd->flags.contains(window_flags::group) ? style::group_bg : style::window_bg;

			if((wnd->flags.contains(window_flags::popup) || wnd->flags.contains(window_flags::group)) && detail::current_window != wnd && wnd->parent_window != detail::current_window) {
				wnd->parent_window = detail::current_window;
			}

			if(open) wnd->visible = *open;
			if(!wnd->visible) return false;

			if(wnd->flags.contains(window_flags::set_pos)) wnd->pos = pos;
			if(wnd->flags.contains(window_flags::set_size)) {
				if(size.x > 1.f) wnd->size.x = size.x;
				if(size.y > 1.f) wnd->size.y = size.y;
			}

			wnd->clamp_on_screen();

			if(wnd->flags.contains(window_flags::popup) || wnd->close_call) { //logic for popups
				if(((input::get_key(input::key_id::mouse_left)->clicked() || input::get_key(input::key_id::mouse_right)->clicked()) && !wnd->in_popup_region()) || wnd->close_call) {
					int window_id = std::distance(wnd->parent_window->child_popup_window.begin(), std::find_if(wnd->parent_window->child_popup_window.begin(), wnd->parent_window->child_popup_window.end(), [=](window* _wnd) { return _wnd->name == wnd->name; }));
					if(window_id < wnd->parent_window->child_popup_window.size()) {
						wnd->parent_window->child_popup_window.erase(wnd->parent_window->child_popup_window.begin() + window_id);
						detail::windows.erase(detail::windows.begin() + wnd->idx);
						if(detail::active_window_name == wnd->name) detail::active_window_name = "";
						return false;
					}
				}
			}

			detail::current_window = wnd;

			if(wnd->dragging && detail::active_name == "") {
				if(!input::get_key(input::key_id::mouse_left)->down() || detail::active_window_name != wnd->name) {
					wnd->drag_offset = vec2(0.f, 0.f);
					wnd->dragging = false;
					if(detail::active_window_name == wnd->name) detail::active_window_name = "";
				} else {
					wnd->pos = input::mouse_pos - wnd->drag_offset;

					wnd->clamp_on_screen();
				}
			}

			if(detail::active_window_name == wnd->name || ((input::get_key(input::key_id::mouse_left)->clicked() || input::get_key(input::key_id::mouse_right)->clicked()) && input::mouse_in_region(wnd->pos, wnd->pos + wnd->size) && detail::mouse_in_current_window())) {
				detail::focus_current_window();
			}

			wnd->draw_list->draw_rect_filled(wnd->pos, wnd->pos + wnd->size, bg_color, style::window_rounding, wnd->get_title_size() == 0.f ? render::corner_flags::all : render::corner_flags::bot);

			if(!wnd->flags.contains(window_flags::no_title_line)) wnd->draw_list->draw_rect_filled(wnd->pos, wnd->pos + vec2(wnd->size.x, style::window_title_line_size), style::main_color, style::window_title_rounding, render::corner_flags::top);
			if(have_title_bar) {
				detail::push_var(&wnd->pos.y, wnd->pos.y + (wnd->flags.contains(window_flags::no_title_line) ? 0.f : style::window_title_line_size)); {
					wnd->draw_list->draw_rect_filled(wnd->pos, wnd->pos + vec2(wnd->size.x, style::window_title_size), style::window_title_bg);

					wnd->draw_list->push_clip_rect(wnd->pos, wnd->pos + vec2(wnd->size.x, style::window_title_size), wnd->flags.contains(window_flags::group)); {
						wnd->draw_list->push_clip_rect(wnd->pos, wnd->pos + vec2(wnd->size.x - style::window_title_size, style::window_title_size), true); {
							wnd->draw_list->draw_text(wnd->name, wnd->pos + vec2(5.f, style::window_title_size / 2), color(255, 255, 255, 255), false, { false, true });
							wnd->draw_list->draw_rect_filled_multicolor(wnd->pos + vec2(wnd->size.x - 45 - style::window_title_size, 0.f), wnd->pos + vec2(wnd->size.x - style::window_title_size, style::window_title_size), { color(style::window_title_bg, 0.f), style::window_title_bg }, { color(style::window_title_bg, 0.f), style::window_title_bg });
						} wnd->draw_list->pop_clip_rect();

						detail::push_var(&wnd->draw_item_pos, wnd->pos + vec2(wnd->size.x - (style::window_title_size / 2) - (font::text_size("x").x / 2), (style::window_title_size / 2) - (font::text_size("x").y / 2))); {
							detail::push_var(&wnd->ignore_scroll, true); {
								detail::push_var(&style::items_size_full_window, false); {
									if(open && clickable_text(utils::format("x##%s", wnd->name.c_str()))) {
										if(detail::active_window_name == name) detail::active_window_name = "";
										*open = false;
									}
								} detail::pop_var();
							} detail::pop_var();
						} detail::pop_var();
					} wnd->draw_list->pop_clip_rect();
				} detail::pop_var();
			}

			wnd->draw_item_pos = wnd->pos + vec2(0.f, wnd->get_title_size()) + style::window_padding;
			wnd->draw_list->push_clip_rect(wnd->pos + vec2(0.f, wnd->get_title_size()), wnd->pos + wnd->size, true);

			begin_scroll();

			wnd->max_size = vec2(0.f, 0.f);

			return true;
		}

		void end_window() {
			window* wnd = detail::current_window;
			if(!wnd) return;

			end_scroll();

			wnd->draw_list->pop_clip_rect();

			if(wnd->flags.contains(window_flags::auto_size)) { //auto-size logic
				if(wnd->arg_size.x < 1.f) wnd->size.x = wnd->max_size.x + style::window_padding.x - style::item_spacing;
				if(wnd->arg_size.y < 1.f) wnd->size.y = wnd->max_size.y + style::window_padding.y - style::item_spacing;
			}

			if(wnd->flags.contains(window_flags::popup) || wnd->flags.contains(window_flags::group)) detail::current_window = wnd->parent_window;
			else detail::current_window = nullptr;
		}

		window::window(std::string wnd_name, vec2 wnd_pos, vec2 wnd_size, flags_list<window_flags> wnd_flags) :
			name(wnd_name), pos(wnd_pos), size(wnd_size), arg_size(wnd_size), flags(wnd_flags) {
			if(detail::current_window && (flags.contains(window_flags::group) || flags.contains(window_flags::popup)))
				parent_window = detail::current_window;

			draw_list = flags.contains(window_flags::group) && parent_window ? get_main_window()->draw_list : std::make_shared<render::draw_list>(&render::shared_data);
			draw_list->reset_for_begin_render();
			draw_list->push_texture_id(font::font_atlas->tex_id);
			draw_list->push_clip_rect_full_screen();
		}

		window* window::in_popup_region() {
			if(!parent_window) return nullptr;
			if(parent_window->child_popup_window.size() <= 0) return nullptr;
			for(window* group : parent_window->child_popup_window) {
				if(input::mouse_in_region(group->pos, group->pos + group->size)) return group;
			}
			return nullptr;
		}

		window* window::get_hovered_group() {
			if(child_group_window.size() <= 0) return nullptr;
			for(window* group : child_group_window) {
				if(group->get_hovered_group() != nullptr) return group->get_hovered_group();
				if(input::mouse_in_region(group->pos, group->pos + group->size)) return group;
			}
			return nullptr;
		}

		window* window::get_main_window() {
			window* last = this;

			if(!last->parent_window) return last;

			while(last->flags.contains(window_flags::popup) || last->flags.contains(window_flags::group)) {
				if(!last->parent_window) break;
				last = last->parent_window;
			}

			return last;
		}

		void window::focus_window() {
			if(this == detail::windows.back().get()) return;
			std::vector<std::shared_ptr<window>>::iterator wnd = detail::windows.begin() + idx;
			std::rotate(wnd, wnd + 1, detail::windows.end());
		}

		void window::clamp_on_screen() {
			if(style::clamp_window_on_screen && !flags.contains(window_flags::group)) {
				pos = math::clamp(pos, vec2(0.f, 0.f), detail::display_size - size);
			}
		}

		vec2 window::get_window_size_with_padding() {
			vec2 offset = draw_item_pos - pos - get_padding();
			return size - (get_padding() * 2) - (vec2(get_scroll_thickness(), 0.f) + vec2(!in_column ? offset.x : 0.f, offset.y));
		}

		float window::get_title_size() {
			float result = 0.f;
			result += (flags.contains(window_flags::no_title_bar) || flags.contains(window_flags::popup) ? 0.f : style::window_title_size);
			result += (flags.contains(window_flags::no_title_line) ? 0.f : style::window_title_line_size);
			return result;
		}

		bool window::can_open_tooltip() {
			if(child_popup_window.size() > 1) return false;
			else if(child_popup_window.size() == 1 && child_popup_window.back()->name == "##tooltip") return true;
			else if(child_popup_window.size() == 0) return true;
			return false;
		}

		bool window::can_draw_item(rect item_rect) {
			return (item_rect.min.y > pos.y && item_rect.min.y < pos.y + size.y) || (item_rect.max.y > pos.y && item_rect.max.y < pos.y + size.y);
		}
	}
}