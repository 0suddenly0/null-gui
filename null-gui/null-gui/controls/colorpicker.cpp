#include "../null-gui.h"

std::vector<color> colors = { { 255, 0, 0, 255 }, {255, 255, 0, 255}, {0, 255, 0, 255}, {0, 255, 255, 255}, {0, 0, 255, 255}, {255, 0, 255, 255}, {255, 0, 0, 255} };
static color coped;
namespace null_gui {
	color colorpicker_sv(color clr, std::string name) {
		window* wnd = deeps::current_window;
		if (!wnd) return clr;

		color ret = clr;
		vec2 draw_pos = wnd->draw_item_pos;
		vec2 dot_pos = vec2(math::clamp(round(draw_pos.x + math::clamp(ret.g(), 0.f, 1.f) * gui_settings::colorpicker_size), draw_pos.x + 2, draw_pos.x + gui_settings::colorpicker_size - 2), math::clamp(round(draw_pos.y + math::clamp(1 - ret.b(), 0.f, 1.f) * gui_settings::colorpicker_size), draw_pos.y + 2, draw_pos.y + gui_settings::colorpicker_size - 2));
		rect size(draw_pos, draw_pos + vec2(gui_settings::colorpicker_size, gui_settings::colorpicker_size));
		bool pressed = deeps::colorpicker_sliders_behavior(size, name);

		if (pressed) {
			ret.g() = math::clamp((null_input::mouse_pos.x - size.min.x) / (gui_settings::colorpicker_size - 1), 0.f, 1.f);
			ret.b() = 1.0f - math::clamp((null_input::mouse_pos.y - size.min.y) / (gui_settings::colorpicker_size - 1), 0.f, 1.f);
		}

		wnd->draw_list->draw_rect_filled_multi_color(size.min, size.max, { color(255, 255, 255, 255), color(clr.r(), 1.f, 1.f).hsv_to_rgb() }, { color(255, 255, 255, 255), color(clr.r(), 1.f, 1.f).hsv_to_rgb() });
		wnd->draw_list->draw_rect_filled_multi_color(size.min, size.max, { color(0, 0, 0, 0), color(0, 0, 0, 0) }, { color(0, 0, 0, 255), color(0, 0, 0, 255) });

		wnd->draw_list->draw_circle_filled(dot_pos, color(45, 45, 45, 100), 5.f);
		wnd->draw_list->draw_circle_filled(dot_pos, color(clr.hsv_to_rgb(), 1.f), 3.f);

		deeps::add_item(size.size(), name);
		return ret;
	}

	float colorpicker_slider_h(color clr, std::string name) {
		window* wnd = deeps::current_window;
		if (!wnd) return clr.r();

		vec2 draw_pos = wnd->draw_item_pos;
		rect size(draw_pos, draw_pos + vec2(gui_settings::colorpicker_thickness, gui_settings::colorpicker_size));
		float ret = clr.r();
		float calc_pos = round(draw_pos.y + (ret / 360.f) * size.size().y);
		bool pressed = deeps::colorpicker_sliders_behavior(size, name);

		if (pressed) ret = math::clamp((null_input::mouse_pos.y - draw_pos.y) / (gui_settings::colorpicker_size - 1), 0.f, 1.f) * 360.f;

		for (int i = 0; i < 6; i++) {
			rect size_temp(vec2(draw_pos.x, draw_pos.y + (i * (size.size().y / 6))), vec2(size.max.x, draw_pos.y + ((i + 1) * (size.size().y / 6))));
			wnd->draw_list->draw_rect_filled_multi_color(size_temp.min, size_temp.max, { colors[i], colors[i] }, { colors[i + 1], colors[i + 1] });
		}

		wnd->draw_list->draw_rect_filled(vec2(size.min.x - 2, calc_pos - 1), vec2(size.max.x + 2, calc_pos + 1), color(255, 255, 255));

		deeps::add_item(size.size(), name);
		return ret;
	}

	float colorpicker_slider_alpha(color clr, std::string name) {
		window* wnd = deeps::current_window;
		if (!wnd) return clr.a();

		vec2 draw_pos = wnd->draw_item_pos;
		rect size(draw_pos, vec2(draw_pos.x + wnd->size.x - (gui_settings::window_padding.x * 2), draw_pos.y + gui_settings::colorpicker_thickness));
		float ret = clr.a();
		float calc_pos = round(draw_pos.x + ret * size.size().x);
		bool pressed = deeps::colorpicker_sliders_behavior(size, name);

		if (pressed) ret = math::clamp((null_input::mouse_pos.x - draw_pos.x) / (size.size().x - 1), 0.f, 1.f);

		wnd->draw_list->draw_rect_filled_multi_color(size.min, size.max, { color(255, 255, 255, 10), color(clr.hsv_to_rgb(), 1.f) }, { color(255, 255, 255, 10), color(clr.hsv_to_rgb(), 1.f) });
		wnd->draw_list->draw_rect_filled(vec2(calc_pos - 1.f, size.min.y - 2), vec2(calc_pos + 1.f, size.max.y + 2), color(255, 255, 255));

		deeps::add_item(size.size(), name);
		return ret;
	}

	void colorpicker(std::string text, color* clr, bool alpha_bar) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		color ret = clr->rgb_to_hsv();
		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		rect size(draw_pos, vec2(draw_pos.x + wnd->size.x - gui_settings::window_padding.x - (wnd->get_scroll_thickness() + gui_settings::window_padding.x), draw_pos.y + text_size.y + gui_settings::text_spacing));
		rect size_open(size.max - (size.max.y - size.min.y), size.max);
		std::vector<window_flags> flags = { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size };

		deeps::colorpicker_behavior(clr, size_open, name, utils::format("##%s colorpicker", text.c_str()), utils::format("##%s colorpicker tooltip", text.c_str()), flags, alpha_bar);
		deeps::add_item(size.size(), name);

		null_gui::tooltip([=]() {
			/*if (alpha_bar) null_gui::text(utils::format("r: %.0f; g: %.0f; g: %.0f; a: %.0f", clr->r() * 255.f, clr->g() * 255.f, clr->b() * 255.f, clr->a() * 255.f));
			else null_gui::text(utils::format("r: %.0f; g: %.0f; b: %.0f", clr->r() * 255.f, clr->g() * 255.f, clr->b() * 255.f));*/
			clr->to_int();
			if (alpha_bar) null_gui::text(utils::format("r: %.0f; g: %.0f; g: %.0f; a: %.0f", clr->r(), clr->g(), clr->b(), clr->a()));
			else null_gui::text(utils::format("r: %.0f; g: %.0f; b: %.0f", clr->r(), clr->g(), clr->b()));
			clr->to_float();
			});

		wnd->draw_list->draw_rect_filled(size.max - size.size().y, size.max, gui_settings::button_bg);	
		wnd->draw_list->draw_rect_filled(size.max - size.size().y, size.max - size.size().y / 2, gui_settings::button_bg_active);
		wnd->draw_list->draw_rect_filled(size.max - size.size().y / 2, size.max, gui_settings::button_bg_active);
		wnd->draw_list->draw_rect_filled(size.max - size.size().y, size.max, *clr);
		wnd->draw_list->draw_text(deeps::format_item(name), vec2(size.min.x, size.max.y - (size.max.y - size.min.y) / 2), gui_settings::text, false, { false , true });

		if (deeps::window_exist(utils::format("##%s colorpicker", text.c_str()))) {
			deeps::push_var(&gui_settings::window_padding, vec2(5.f, 5.f)); {
				if (begin_window(utils::format("##%s colorpicker", text.c_str()), vec2(size.max.x, size.min.y), vec2(0.f, 0.f), flags, nullptr)) {
					color sv = colorpicker_sv(ret, utils::format("##%s colorpicker_slider_sv", text.c_str()));
					deeps::push_var(&gui_settings::item_spacing, 0.f); {
						same_line();
					} deeps::pop_var();
					float h = colorpicker_slider_h(ret, utils::format("##%s colorpicker_slider_h", text.c_str()));
					float alpha = alpha_bar ? colorpicker_slider_alpha(ret, utils::format("##%s colorpicker_slider_alpha", text.c_str())) : ret.a();
					ret = color(h, sv.g(), sv.b(), alpha);
					end_window();
				}
			} deeps::pop_var();
		}

		if (deeps::window_exist(utils::format("##%s colorpicker tooltip", text.c_str()))) {
			deeps::push_var(&gui_settings::window_padding, vec2(5.f, 5.f)); {
				deeps::push_var(&gui_settings::items_size_full_window, true); {
					if (begin_window(utils::format("##%s colorpicker tooltip", text.c_str()), vec2(size.max.x, size.min.y), vec2(0.f, 0.f), flags, nullptr)) {
						if (button("copy")) {
							coped = ret;
							deeps::close_current_window();
						}
						if (button("paste")) {
							ret = coped;
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