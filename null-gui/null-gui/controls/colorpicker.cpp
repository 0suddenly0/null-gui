#include "../null-gui.h"

std::vector<color> colors = { { 255, 0, 0, 255 }, {255, 255, 0, 255}, {0, 255, 0, 255}, {0, 255, 255, 255}, {0, 0, 255, 255}, {255, 0, 255, 255}, {255, 0, 0, 255} };

namespace null_gui {
	color colorpicker_sv(color clr, std::string name) {
		window* wnd = deeps::current_window;
		if (!wnd) return clr;

		color ret = clr;
		vec2 draw_pos = wnd->draw_item_pos;
		vec2 dot_pos = vec2(null_math::clamp(round(draw_pos.x + null_math::clamp(ret.g(), 0.f, 1.f) * gui_settings::colorpicker_size), draw_pos.x + 2, draw_pos.x + gui_settings::colorpicker_size - 2), null_math::clamp(round(draw_pos.y + null_math::clamp(1 - ret.b(), 0.f, 1.f) * gui_settings::colorpicker_size), draw_pos.y + 2, draw_pos.y + gui_settings::colorpicker_size - 2));
		rect size(draw_pos, draw_pos + vec2(gui_settings::colorpicker_size, gui_settings::colorpicker_size));
		bool pressed = deeps::get_colorpicker_sliders_behavior(size, name);

		if (pressed) {
			ret.g() = null_math::clamp((null_input::mouse_pos().x - size.min.x) / (gui_settings::colorpicker_size - 1), 0.f, 1.f);
			ret.b() = 1.0f - null_math::clamp((null_input::mouse_pos().y - size.min.y) / (gui_settings::colorpicker_size - 1), 0.f, 1.f);
		}

		wnd->draw_list.add_rect_multicolor(size.min, size.max, { color(255, 255, 255, 255), color(clr.r(), 1.f, 1.f).hsv_to_rgb() }, { color(255, 255, 255, 255), color(clr.r(), 1.f, 1.f).hsv_to_rgb() });
		wnd->draw_list.add_rect_multicolor(size.min, size.max, { color(0, 0, 0, 0), color(0, 0, 0, 0) }, { color(0, 0, 0, 255), color(0, 0, 0, 255) });

		wnd->draw_list.add_rect(dot_pos - vec2(4.f, 4.f), dot_pos + vec2(4.f, 4.f), color(45, 45, 45)); //need change to circle
		wnd->draw_list.add_rect(dot_pos - vec2(2.f, 2.f), dot_pos + vec2(2.f, 2.f), color(clr.hsv_to_rgb(), 1.f));

		deeps::add_item(size.size());
		return ret;
	}

	float colorpicker_slider_h(color clr, std::string name) {
		window* wnd = deeps::current_window;
		if (!wnd) return clr.r();

		vec2 draw_pos = wnd->draw_item_pos;
		rect size(draw_pos, draw_pos + vec2(gui_settings::colorpicker_thickness, gui_settings::colorpicker_size));
		float ret = clr.r();
		float calc_pos = round(draw_pos.y + (ret / 360.f) * size.size().y);
		bool pressed = deeps::get_colorpicker_sliders_behavior(size, name);

		if (pressed) ret = null_math::clamp((null_input::mouse_pos().y - draw_pos.y) / (gui_settings::colorpicker_size - 1), 0.f, 1.f) * 360.f;

		for (int i = 0; i < 6; i++) {
			rect size_temp(vec2(draw_pos.x, draw_pos.y + (i * (size.size().y / 6))), vec2(size.max.x, draw_pos.y + ((i + 1) * (size.size().y / 6))));
			wnd->draw_list.add_rect_multicolor(size_temp.min, size_temp.max, { colors[i], colors[i] }, { colors[i + 1], colors[i + 1] });
		}

		wnd->draw_list.add_rect(vec2(size.min.x - 2, calc_pos - 1), vec2(size.max.x + 2, calc_pos + 1), color(255, 255, 255));

		deeps::add_item(size.size());
		return ret;
	}

	float colorpicker_slider_alpha(color clr, std::string name) {
		window* wnd = deeps::current_window;
		if (!wnd) return clr.a();

		vec2 draw_pos = wnd->draw_item_pos;
		rect size(draw_pos, vec2(wnd->pos.x + wnd->size.x - gui_settings::window_padding.x, draw_pos.y + gui_settings::colorpicker_thickness));
		float ret = clr.a();
		float calc_pos = round(draw_pos.x + ret * size.size().x);
		bool pressed = deeps::get_colorpicker_sliders_behavior(size, name);

		if (pressed) ret = null_math::clamp((null_input::mouse_pos().x - draw_pos.x) / (size.size().x - 1), 0.f, 1.f);

		wnd->draw_list.add_rect_multicolor(size.min, size.max, { color(255, 255, 255, 10), color(clr.hsv_to_rgb(), 1.f) }, { color(255, 255, 255, 10), color(clr.hsv_to_rgb(), 1.f) });
		wnd->draw_list.add_rect(vec2(calc_pos - 1.f, size.min.y - 2), vec2(calc_pos + 1.f, size.max.y + 2), color(255, 255, 255));

		deeps::add_item(size.size());
		return ret;
	}

	void colorpicker(std::string text, color* clr, bool alpha_bar) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		color ret = clr->rgb_to_hsv();
		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos;
		vec2 text_size = null_font::text_size(draw_text);
		vec2 left_spacing(gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_spacing : 0, 0.f);
		rect size(draw_pos + left_spacing, vec2(wnd->pos.x + wnd->size.x - gui_settings::window_padding.x - left_spacing.x, draw_pos.y + text_size.y + gui_settings::text_spacing));
		rect size_open(size.max - (size.max.y - size.min.y), size.max);
		std::vector<window_flags> flags = { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size };
		
		deeps::get_colorpicker_behavior(size_open, utils::format("##%s colorpicker", text.c_str()), flags);

		wnd->draw_list.add_rect(size.max - size.size().y, size.max, gui_settings::button_bg);
		wnd->draw_list.add_rect(size.max - size.size().y + vec2(2.f, 2.f), size.max - vec2(2.f, 2.f), color(*clr, 1.f));
		wnd->draw_list.add_text(deeps::format_item(draw_text), vec2(size.min.x, size.max.y - (size.max.y - size.min.y) / 2), gui_settings::text, false, { false , true });

		deeps::add_item(size.size());


		if (deeps::find_window(utils::format("##%s colorpicker", text.c_str())) != nullptr) {
			deeps::push_var({ &gui_settings::window_padding, vec2(5.f, 5.f) }); {
				if (begin_window(utils::format("##%s colorpicker", text.c_str()), vec2(size.max.x, size.min.y), vec2(0.f, 0.f), flags, nullptr)) {
					color sv = colorpicker_sv(ret, utils::format("##%s colorpicker_slider_sv", text.c_str()));
					same_line();
					float h = colorpicker_slider_h(ret, utils::format("##%s colorpicker_slider_h", text.c_str()));
					float alpha = alpha_bar ? colorpicker_slider_alpha(ret, utils::format("##%s colorpicker_slider_alpha", text.c_str())) : ret.a();
					ret = color(h, sv.g(), sv.b(), alpha);
					end_window();
				}
			} deeps::pop_var();
		}

		*clr = ret.hsv_to_rgb();
	}
}