#include "gui.h"

namespace null {
	namespace gui {
		namespace detail {
			std::string parse_format(std::string format) {
				std::string ret = "%";

				int find_position = format.find('%');
				std::string aut;
				if(find_position != std::string::npos) {
					for(int i = find_position; i < format.size(); i++) {
						if((format[i] > '0' && format[i] < '9') || (format[i] > 'a' && format[i] < 'z') || format[i] == '.')
							ret.push_back(format[i]);

						if(format[i] > 'a' && format[i] < 'z')
							break;
					}
				}

				return ret;
			}

			bool window_exist(std::string name) {
				return find_window(name) != nullptr;
			}

			window* find_window(std::string name) {
				for(int i = 0; i < windows.size(); i++) {
					window* wnd = windows[i].get();
					wnd->idx = i;
					if(wnd->name == name)
						return wnd;
				}

				return nullptr;
			}

			window* add_window(std::string name, vec2 pos, vec2 size, flags_list<window_flags> flags) {
				std::shared_ptr<window> wnd = std::make_shared<window>(name, pos, size, flags);

				if(wnd->flags.contains(window_flags::popup)) wnd->flags.add(window_flags::no_move);

				wnd->idx = wnd->flags.contains(window_flags::group) ? 0 : windows.size();

				if(windows.size() == 0) windows.push_back(wnd);
				else {
					if(wnd->flags.contains(window_flags::popup) || wnd->flags.contains(window_flags::group)) {
						if(wnd->flags.contains(window_flags::popup)) current_window->child_popup_window.push_back(wnd.get());
						else current_window->child_group_window.push_back(wnd.get());
					}
					windows.insert(wnd->flags.contains(window_flags::group) ? windows.begin() : windows.end(), wnd);
				}

				return wnd.get();
			}

			bool can_use_item(rect item_rect, std::string item_name) {
				window* wnd = detail::current_window;
				if(!wnd) return false;
				return (hovered_name == "" || hovered_name == item_name) && ((input::mouse_in_region(item_rect) && input::mouse_in_region(wnd->draw_list->get_clip_rect()) && detail::mouse_in_current_window()) || active_name == item_name);
			}

			void add_item(vec2 size, std::string name) {
				window* wnd = current_window;
				if(!wnd || wnd->dont_add_item) return;
				size += vec2(style::item_spacing, 0.f);
				wnd->last_draw_item_pos = wnd->draw_item_pos;
				wnd->draw_item_pos_prev = wnd->draw_item_pos + vec2(size.x, 0.f);

				wnd->draw_item_pos.x = wnd->pos.x + (wnd->column_offset == 0.f ? style::window_padding.x : wnd->column_offset);
				wnd->draw_item_pos.y += size.y + style::item_spacing;

				if(wnd->max_size.x < wnd->draw_item_pos_prev.x - detail::current_window->pos.x + wnd->get_scroll_thickness()) wnd->max_size.x = wnd->draw_item_pos_prev.x - detail::current_window->pos.x + wnd->get_scroll_thickness();
				if(wnd->max_size.y < wnd->draw_item_pos.y - detail::current_window->pos.y) wnd->max_size.y = wnd->draw_item_pos.y - detail::current_window->pos.y;

				wnd->in_same_line = false;
				if(!name.empty()) last_item_name = name;
			}

			std::string format_item(std::string text) {
				std::string ret = text;

				int find_position = text.find('#');
				std::string aut;
				if(find_position != std::string::npos) {
					if(find_position < text.size() - 2 && text[find_position + 1] == '#')
						ret = std::string(text.begin(), text.begin() + find_position);
				}

				return ret;
			}

			void focus_current_window() {
				if(current_window->flags.contains(window_flags::group)) current_window->get_main_window()->focus_window();
				else current_window->focus_window();
			}

			void window_control() {
				for(int i = detail::windows.size() - 1; i >= 0; i--) { //getting hovered window and group
					window* wnd = detail::windows[i].get();
					if(!wnd->visible) continue;

					if(!input::mouse_in_region(wnd->pos, wnd->pos + wnd->size)) {
						wnd->hovered_group = nullptr;
					} else {
						hovered_window = wnd->flags.contains(window_flags::group) ? wnd->get_main_window() : wnd;
						hovered_window->hovered_group = hovered_window->get_hovered_group();
						break;
					}
				}

				if(input::get_key(input::key_id::mouse_left)->clicked() && (active_name.empty() && active_window_name.empty())) { //window moving
					for(int i = windows.size() - 1; i >= 0; --i) {
						window* wnd = windows[i].get();

						if(wnd->flags.contains(window_flags::no_move)) continue;

						if(!wnd->dragging && hovered_window == wnd && input::mouse_in_region(wnd->pos, wnd->pos + vec2(wnd->size.x, style::move_window_on_title_bar ? wnd->get_title_size() : wnd->size.y)) && wnd->visible) {
							wnd->drag_offset = input::mouse_pos - wnd->pos;
							wnd->dragging = true;
							active_window_name = wnd->name;
							break;
						}
					}
				}
			}

			void popups_control() {
				for(int i = 0; i < detail::windows.size(); i++) {
					window* wnd = detail::windows[i].get();

					if(wnd->flags.contains(window_flags::popup)) {
						if(!detail::window_exist(wnd->parent_window->name) || !wnd->parent_window->visible) {
							detail::windows.erase(detail::windows.begin() + wnd->idx);
							if(detail::active_window_name == wnd->name) detail::active_window_name = "";
						}
					}
				}
			}
		}

		void initialize(HWND hwnd) {
			detail::hwnd = hwnd;
			::QueryPerformanceFrequency((LARGE_INTEGER*)&detail::ticks_per_second);
			::QueryPerformanceCounter((LARGE_INTEGER*)&detail::time);
		}

		void shutdown() {
			detail::windows.clear();
			detail::pushed_vars.clear();
			detail::text_inputs.clear();

			input::bind_list.clear();
			input::key_list.clear();
		}

		void begin_frame() {
			RECT rect;
			::GetClientRect(detail::hwnd, &rect);
			if(!IsIconic(detail::hwnd)) detail::display_size = vec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

			INT64 current_time;
			::QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
			detail::delta_time = (float)(current_time - detail::time) / detail::ticks_per_second;
			detail::time = current_time;
			detail::real_time += detail::delta_time;

			if(detail::hovered_window && !input::mouse_in_region(detail::hovered_window->pos, detail::hovered_window->pos + detail::hovered_window->size))
				detail::hovered_window = nullptr;

			detail::hovered_name = "";
			detail::last_item_name = "";
			input::bind_control();
			input::update_keys_state();
			detail::window_control();
			detail::popups_control();
			detail::text_input_info::control();
		}

		void new_line() {
			window* wnd = detail::current_window;
			if(!wnd) return;

			detail::add_item(vec2(0.f, style::new_line_size), "");
		}

		void same_line() {
			window* wnd = detail::current_window;
			if(!wnd) return;

			wnd->draw_item_pos = wnd->draw_item_pos_prev + vec2(style::item_spacing, 0.f);
			wnd->in_same_line = true;
		}

		void tooltip(std::string tooltip_text) {
			functional_tooltip([=]() { text(tooltip_text.c_str()); });
		}

		void functional_tooltip(std::function<void()> func) {
			window* wnd = detail::current_window;
			if(!wnd) return;
			if(detail::active_window_name != "" || detail::active_name != "" || !wnd->can_open_tooltip()) return;
			bool close = detail::hovered_name == detail::last_item_name;
			detail::push_var(&style::window_padding, vec2(5.f, 5.f)); {
				if(begin_window("##tooltip", input::mouse_pos + vec2(10, 10), vec2(0.f, 0.f), { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size }, &close)) {
					detail::focus_current_window();
					func();
					end_window();
				}
			} detail::pop_var();
		}
	}
}