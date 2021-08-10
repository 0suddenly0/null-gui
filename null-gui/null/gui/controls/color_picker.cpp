#include "../gui.h"

std::vector<color> colors = { { 255, 0, 0, 255 }, {255, 255, 0, 255}, {0, 255, 0, 255}, {0, 255, 255, 255}, {0, 0, 255, 255}, {255, 0, 255, 255}, {255, 0, 0, 255} };
color temp_copy_of_color;

//rgb - hsv
std::map<std::string, color> colorpicker_list;

namespace null {
	namespace gui {
		color color_picker_sv(color clr, std::string name) {
			window* wnd = detail::current_window;
			if(!wnd) return clr;

			std::string item_name = utils::format("##%s colorpicker_slider_sv", name.c_str());
			float s = clr.g() == 0 ? colorpicker_list[name].g() : clr.g();
			float v = clr.b() == 0 ? colorpicker_list[name].b() : clr.b();
			vec2 draw_pos = wnd->draw_item_pos;
			vec2 dot_pos = vec2(math::clamp(round(draw_pos.x + math::clamp(s, 0.f, 1.f) * style::color_picker_size), draw_pos.x + 2, draw_pos.x + style::color_picker_size - 2), math::clamp(round(draw_pos.y + math::clamp(1 - v, 0.f, 1.f) * style::color_picker_size), draw_pos.y + 2, draw_pos.y + style::color_picker_size - 2));
			rect item_rect(draw_pos, draw_pos + vec2(style::color_picker_size, style::color_picker_size));
			bool pressed = detail::controls_behavior::color_picker_sliders(item_rect, item_name);

			if(pressed) {
				s = math::clamp((input::mouse_pos.x - item_rect.min.x) / (style::color_picker_size - 1), 0.f, 1.f);
				v = 1.0f - math::clamp((input::mouse_pos.y - item_rect.min.y) / (style::color_picker_size - 1), 0.f, 1.f);

				colorpicker_list[name].g() = s;
				colorpicker_list[name].b() = v;
			}

			wnd->draw_list->draw_rect_filled_multicolor_horizontal(item_rect.min, item_rect.max, color(255, 255, 255, 255), color(clr.r() == 0.f ? colorpicker_list[name].r() : clr.r(), 1.f, 1.f).hsv_to_rgb(), style::color_picker_rounding);
			wnd->draw_list->draw_rect_filled_multicolor_vertical(item_rect.min, item_rect.max, color(0, 0, 0, 0), color(0, 0, 0, 255), style::color_picker_rounding);

			wnd->draw_list->draw_circle_filled(dot_pos, color(45, 45, 45, 100), 5.f);
			wnd->draw_list->draw_circle_filled(dot_pos, color(clr.hsv_to_rgb(), 1.f), 3.f);

			detail::add_item(item_rect.size(), item_name);
			return color(clr.r(), s, v);
		}

		float color_picker_slider_h(color clr, std::string name) {
			window* wnd = detail::current_window;
			if(!wnd) return clr.r();

			std::string item_name = utils::format("##%s colorpicker_slider_h", name.c_str());
			vec2 draw_pos = wnd->draw_item_pos;
			rect item_rect(draw_pos, draw_pos + vec2(style::color_picker_thickness, style::color_picker_size));
			float h = clr.r() == 0.f ? colorpicker_list[name].r() : clr.r();
			float calc_pos = round(draw_pos.y + (h / 360.f) * item_rect.size().y);
			bool pressed = detail::controls_behavior::color_picker_sliders(item_rect, item_name);

			if(pressed) {
				colorpicker_list[name].r() = h = math::clamp((input::mouse_pos.y - draw_pos.y) / (style::color_picker_size - 1), 0.f, 1.f) * 360.f;
			}

			for(int i = 0; i < 6; i++) {
				rect segment_size(vec2(draw_pos.x, draw_pos.y + (i * (item_rect.size().y / 6))) - vec2(0.f, i == 5 ? 1.f : 0.f), vec2(item_rect.max.x, draw_pos.y + ((i + 1) * (item_rect.size().y / 6))) + vec2(0.f, i == 0 ? 1.f : 0.f));
				if(i == 0 || i == 5) wnd->draw_list->draw_rect_filled_multicolor_vertical(segment_size.min, segment_size.max, colors[i], colors[i + 1], style::color_picker_rounding, i == 0 ? render::corner_flags::top : render::corner_flags::bot);
				else wnd->draw_list->draw_rect_filled_multicolor(segment_size.min, segment_size.max, { colors[i], colors[i] }, { colors[i + 1], colors[i + 1] });
			}

			wnd->draw_list->draw_rect_filled(vec2(item_rect.min.x - 2, calc_pos - 1), vec2(item_rect.max.x + 2, calc_pos + 1), color(255, 255, 255));

			detail::add_item(item_rect.size(), item_name);
			return h;
		}

		float color_picker_slider_alpha(color clr, std::string name) {
			window* wnd = detail::current_window;
			if(!wnd) return clr.a();

			std::string item_name = utils::format("##%s colorpicker_slider_alpha", name.c_str());
			vec2 draw_pos = wnd->draw_item_pos;
			rect item_rect(draw_pos, vec2(draw_pos.x + wnd->size.x - (style::window_padding.x * 2), draw_pos.y + style::color_picker_thickness));
			float ret = clr.a();
			float calc_pos = round(draw_pos.x + ret * item_rect.size().x);
			bool pressed = detail::controls_behavior::color_picker_sliders(item_rect, item_name);

			if(pressed) ret = math::clamp((input::mouse_pos.x - draw_pos.x) / (item_rect.size().x - 1), 0.f, 1.f);

			wnd->draw_list->draw_rect_filled_multicolor_horizontal(item_rect.min, item_rect.max, color(255, 255, 255, 10), color(clr.hsv_to_rgb(), 1.f), style::color_picker_rounding);
			wnd->draw_list->draw_rect_filled(vec2(calc_pos - 1.f, item_rect.min.y - 2), vec2(calc_pos + 1.f, item_rect.max.y + 2), color(255, 255, 255), style::color_picker_rounding);

			detail::add_item(item_rect.size(), item_name);
			return ret;
		}

		void color_picker(std::string text, color* clr, bool alpha_bar) {
			window* wnd = detail::current_window;
			if(!wnd) return;

			color ret = clr->rgb_to_hsv();
			std::string draw_text = detail::format_item(text);
			vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
			vec2 text_size = font::text_size(draw_text.c_str());
			vec2 min = vec2(text_size.x + style::text_spacing + style::checkbox_size * 2, math::max(style::checkbox_size, text_size.y));
			rect item_rect = rect(draw_pos, draw_pos + vec2(style::items_size_full_window ? math::max(min.x, wnd->get_window_size_with_padding().x) : min.x, min.y));
			rect button_rect = rect(item_rect.max - vec2(item_rect.size().y * 2, item_rect.size().y), item_rect.max);
			flags_list<window_flags> flags = { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size };

			detail::add_item(item_rect.size(), text);
			if(!wnd->can_draw_item(item_rect))
				return;

			std::string window = utils::format("##%s color_picker", text.c_str());
			std::string tooltip_window = utils::format("##%s color_picker tooltip", text.c_str());

			detail::controls_behavior::color_picker(button_rect, text, window, tooltip_window, flags, alpha_bar);

			color clr_draw = clr->get_convert_to_int();
			tooltip(utils::format(alpha_bar ? "r: %.0f; g: %.0f; b: %.0f; a: %.0f" : "r: %.0f; g: %.0f; b: %.0f", clr_draw.r(), clr_draw.g(), clr_draw.b(), clr_draw.a()));

			wnd->draw_list->draw_rect_filled(button_rect.min, button_rect.max, color(*clr, 1.f), style::color_picker_rounding);

			wnd->draw_list->draw_text(draw_text, vec2(item_rect.min.x, item_rect.max.y - (item_rect.max.y - item_rect.min.y) / 2), style::text, false, { false , true });

			if(detail::window_exist(window)) {
				detail::push_var(&style::window_padding, vec2(5.f, 5.f)); {
					if(begin_window(window, vec2(item_rect.max.x, item_rect.min.y), vec2(0.f, 0.f), flags, nullptr)) {
						if(colorpicker_list.count(text) == 0) //if this item not exists - initialization
							colorpicker_list.insert(std::pair<std::string, color>(text, ret));

						color sv = color_picker_sv(ret, text);
						detail::push_var(&style::item_spacing, 0.f); {
							same_line();
						} detail::pop_var();
						float h = color_picker_slider_h(ret, text);
						float alpha = alpha_bar ? color_picker_slider_alpha(ret, text) : ret.a();
						color result = color(h, sv.g(), sv.b(), alpha);

						if(result != ret) ret = result;

						end_window();
					}
				} detail::pop_var();
			}

			if(detail::window_exist(tooltip_window)) {
				detail::push_var(&style::window_padding, vec2(5.f, 5.f)); {
					detail::push_var(&style::items_size_full_window, true); {
						if(begin_window(tooltip_window, vec2(item_rect.max.x, item_rect.min.y), vec2(0.f, 0.f), flags, nullptr)) {
							if(button("copy")) {
								temp_copy_of_color = ret;
								detail::close_current_window();
							}
							if(button("paste")) {
								ret = temp_copy_of_color;
								detail::close_current_window();
							}
							end_window();
						}
					} detail::pop_var();
				} detail::pop_var();
			}

			*clr = ret.hsv_to_rgb();
		}

		namespace detail {
			namespace controls_behavior {
				void color_picker(rect size, std::string name_item, std::string name, std::string tooltip, flags_list<window_flags> flags, bool alpha_bar) {
					window* wnd = detail::current_window;

					if(can_use_item(size, name)) {
						hovered_name = name_item;

						if(input::click_mouse_in_region(size)) {
							if(input::get_key(input::key_id::mouse_left)->down() || input::get_key(input::key_id::mouse_right)->down()) active_name = name_item;
							if(input::get_key(input::key_id::mouse_left)->pressed() && wnd->can_open_tooltip()) {
								flags.add(window_flags::no_title_line);
								detail::add_window(name, vec2(size.max.x, size.min.y), vec2(0.f, 0.f), flags);
							} else if(input::get_key(input::key_id::mouse_right)->pressed() && wnd->can_open_tooltip()) {
								detail::add_window(tooltip, vec2(size.max.x, size.min.y), vec2(0.f, 0.f), flags);
							}
						}
					}


					if(!input::get_key(input::key_id::mouse_left)->down() && !input::get_key(input::key_id::mouse_right)->down() && active_name == name_item) {
						active_name = "";
					}
				}

				bool color_picker_sliders(rect size, std::string name) {
					window* wnd = detail::current_window;
					bool _pressed = false;

					if(can_use_item(size, name)) {
						hovered_name = name;

						if(input::get_key(input::key_id::mouse_left)->clicked() && active_name == "") active_name = name;
					}

					if(active_name == name) _pressed = true;

					if(!input::get_key(input::key_id::mouse_left)->down() && active_name == name) {
						active_name = "";
					}

					return _pressed;
				}
			}
		}
	}
}