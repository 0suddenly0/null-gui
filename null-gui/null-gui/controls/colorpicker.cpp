#include "../null-gui.h"

std::vector<color> colors = { { 255, 0, 0, 255 }, {255, 255, 0, 255}, {0, 255, 0, 255}, {0, 255, 255, 255}, {0, 0, 255, 255}, {255, 0, 255, 255}, {255, 0, 0, 255} };
color temp_copy_of_color;

//rgb - hsv
std::map<std::string, color> colorpicker_list;

namespace null_gui {
	color colorpicker_sv(color clr, std::string name) {
		window* wnd = deeps::current_window;
		if (!wnd) return clr;

		std::string item_name = utils::format("##%s colorpicker_slider_sv", name.c_str());
		float s = clr.g() == 0 ? colorpicker_list[name].g() : clr.g();
		float v = clr.b() == 0 ? colorpicker_list[name].b() : clr.b();
		vec2 draw_pos = wnd->draw_item_pos;
		vec2 dot_pos = vec2(math::clamp(round(draw_pos.x + math::clamp(s, 0.f, 1.f) * gui_settings::colorpicker_size), draw_pos.x + 2, draw_pos.x + gui_settings::colorpicker_size - 2), math::clamp(round(draw_pos.y + math::clamp(1 - v, 0.f, 1.f) * gui_settings::colorpicker_size), draw_pos.y + 2, draw_pos.y + gui_settings::colorpicker_size - 2));
		rect item_rect(draw_pos, draw_pos + vec2(gui_settings::colorpicker_size, gui_settings::colorpicker_size));
		bool pressed = deeps::colorpicker_sliders_behavior(item_rect, item_name);

		if (pressed) {
			s = math::clamp((null_input::mouse_pos.x - item_rect.min.x) / (gui_settings::colorpicker_size - 1), 0.f, 1.f);
			v = 1.0f - math::clamp((null_input::mouse_pos.y - item_rect.min.y) / (gui_settings::colorpicker_size - 1), 0.f, 1.f);

			colorpicker_list[name].g() = s;
			colorpicker_list[name].b() = v;
		}

		wnd->draw_list->draw_rect_filled_multicolor_horizontal(item_rect.min, item_rect.max, color(255, 255, 255, 255), color(clr.r() == 0.f ? colorpicker_list[name].r() : clr.r(), 1.f, 1.f).hsv_to_rgb(), gui_settings::colorpicker_rounding);
		wnd->draw_list->draw_rect_filled_multicolor_vertical(item_rect.min, item_rect.max, color(0, 0, 0, 0), color(0, 0, 0, 255), gui_settings::colorpicker_rounding);

		wnd->draw_list->draw_circle_filled(dot_pos, color(45, 45, 45, 100), 5.f);
		wnd->draw_list->draw_circle_filled(dot_pos, color(clr.hsv_to_rgb(), 1.f), 3.f);

		deeps::add_item(item_rect.size(), item_name);
		return color(clr.r(), s, v);
	}

	float colorpicker_slider_h(color clr, std::string name) {
		window* wnd = deeps::current_window;
		if (!wnd) return clr.r();

		std::string item_name = utils::format("##%s colorpicker_slider_h", name.c_str());
		vec2 draw_pos = wnd->draw_item_pos;
		rect item_rect(draw_pos, draw_pos + vec2(gui_settings::colorpicker_thickness, gui_settings::colorpicker_size));
		float h = clr.r() == 0.f ? colorpicker_list[name].r() : clr.r();
		float calc_pos = round(draw_pos.y + (h / 360.f) * item_rect.size().y);
		bool pressed = deeps::colorpicker_sliders_behavior(item_rect, item_name);

		if (pressed) {
			h = math::clamp((null_input::mouse_pos.y - draw_pos.y) / (gui_settings::colorpicker_size - 1), 0.f, 1.f) * 360.f;
			
			colorpicker_list[name].r() = h;
		}

		for (int i = 0; i < 6; i++) {
			rect segment_size(vec2(draw_pos.x, draw_pos.y + (i * (item_rect.size().y / 6))) - vec2(0.f, i == 5 ? 1.f : 0.f), vec2(item_rect.max.x, draw_pos.y + ((i + 1) * (item_rect.size().y / 6))) + vec2(0.f, i == 0 ? 1.f : 0.f));
			if(i == 0 || i == 5) wnd->draw_list->draw_rect_filled_multicolor_vertical(segment_size.min, segment_size.max, colors[i], colors[i + 1], gui_settings::colorpicker_rounding, i == 0 ? null_render::corner_flags::top : null_render::corner_flags::bot);
			else wnd->draw_list->draw_rect_filled_multicolor(segment_size.min, segment_size.max, { colors[i], colors[i] }, { colors[i + 1], colors[i + 1] });
		}

		wnd->draw_list->draw_rect_filled(vec2(item_rect.min.x - 2, calc_pos - 1), vec2(item_rect.max.x + 2, calc_pos + 1), color(255, 255, 255));

		deeps::add_item(item_rect.size(), item_name);
		return h;
	}

	float colorpicker_slider_alpha(color clr, std::string name) {
		window* wnd = deeps::current_window;
		if (!wnd) return clr.a();

		std::string item_name = utils::format("##%s colorpicker_slider_alpha", name.c_str());
		vec2 draw_pos = wnd->draw_item_pos;
		rect item_rect(draw_pos, vec2(draw_pos.x + wnd->size.x - (gui_settings::window_padding.x * 2), draw_pos.y + gui_settings::colorpicker_thickness));
		float ret = clr.a();
		float calc_pos = round(draw_pos.x + ret * item_rect.size().x);
		bool pressed = deeps::colorpicker_sliders_behavior(item_rect, item_name);

		if (pressed) ret = math::clamp((null_input::mouse_pos.x - draw_pos.x) / (item_rect.size().x - 1), 0.f, 1.f);

		wnd->draw_list->draw_rect_filled_multicolor_horizontal(item_rect.min, item_rect.max, color(255, 255, 255, 10), color(clr.hsv_to_rgb(), 1.f), gui_settings::colorpicker_rounding);
		wnd->draw_list->draw_rect_filled(vec2(calc_pos - 1.f, item_rect.min.y - 2), vec2(calc_pos + 1.f, item_rect.max.y + 2), color(255, 255, 255), gui_settings::colorpicker_rounding);

		deeps::add_item(item_rect.size(), item_name);
		return ret;
	}

	void colorpicker(std::string text, color* clr, bool alpha_bar) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		color ret = clr->rgb_to_hsv();
		std::string draw_text = deeps::format_item(text);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text.c_str());
		vec2 min = vec2(text_size.x + gui_settings::text_spacing + gui_settings::checkbox_size * 2, math::max(gui_settings::checkbox_size, text_size.y));
		rect item_rect = rect(draw_pos, draw_pos + vec2(gui_settings::items_size_full_window ? math::max(min.x, wnd->get_window_size_with_padding()) : min.x, min.y));
		rect button_rect = rect(item_rect.max - vec2(item_rect.size().y * 2, item_rect.size().y), item_rect.max);
		std::vector<window_flags> flags = { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size };

		deeps::add_item(item_rect.size(), text);
		if (!wnd->can_draw_item(item_rect))
			return;

		std::string colorpicker_window = utils::format("##%s colorpicker", text.c_str());
		std::string colorpicker_tooltip_window = utils::format("##%s colorpicker tooltip", text.c_str());

		deeps::colorpicker_behavior(button_rect, text, colorpicker_window, colorpicker_tooltip_window, flags, alpha_bar);

		color clr_draw = clr->get_convert_to_int();
		null_gui::tooltip(utils::format(alpha_bar ? "r: %.0f; g: %.0f; b: %.0f; a: %.0f" : "r: %.0f; g: %.0f; b: %.0f", clr_draw.r(), clr_draw.g(), clr_draw.b(), clr_draw.a()));

		wnd->draw_list->draw_rect_filled(button_rect.min, button_rect.max, color(*clr, 1.f), gui_settings::colorpicker_rounding);

		wnd->draw_list->draw_text(draw_text, vec2(item_rect.min.x, item_rect.max.y - (item_rect.max.y - item_rect.min.y) / 2), gui_settings::text, false, { false , true });

		if (deeps::window_exist(colorpicker_window)) {
			deeps::push_var(&gui_settings::window_padding, vec2(5.f, 5.f)); {
				if (begin_window(colorpicker_window, vec2(item_rect.max.x, item_rect.min.y), vec2(0.f, 0.f), flags, nullptr)) {
					
					if (colorpicker_list.count(text) == 0) //if this item not exists - initialization
						colorpicker_list.insert(std::pair<std::string, color>(text, ret ));

					color sv = colorpicker_sv(ret, text);
					deeps::push_var(&gui_settings::item_spacing, 0.f); {
						same_line();
					} deeps::pop_var();
					float h = colorpicker_slider_h(ret, text);
					float alpha = alpha_bar ? colorpicker_slider_alpha(ret, text) : ret.a();
					color result = color(h, sv.g(), sv.b(), alpha);

					if (result != ret) ret = result;

					end_window();
				}
			} deeps::pop_var();
		}

		if (deeps::window_exist(colorpicker_tooltip_window)) {
			deeps::push_var(&gui_settings::window_padding, vec2(5.f, 5.f)); {
				deeps::push_var(&gui_settings::items_size_full_window, true); {
					if (begin_window(colorpicker_tooltip_window, vec2(item_rect.max.x, item_rect.min.y), vec2(0.f, 0.f), flags, nullptr)) {
						if (button("copy")) {
							temp_copy_of_color = ret;
							deeps::close_current_window();
						}
						if (button("paste")) {
							ret = temp_copy_of_color;
							deeps::close_current_window();
						}
						end_window();
					}
				} deeps::pop_var();
			} deeps::pop_var();
		}

		*clr = ret.hsv_to_rgb();
	}
}