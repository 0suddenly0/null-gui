#include "../null-gui.h"

std::array<vec2, 3> calc_arrow(float scale) {
	null_gui::window* wnd = null_gui::deeps::current_window;
	if (!wnd) return {};

	const float h = wnd->draw_list->_data->font_size * 1.00f;
	float r = h * 0.40f * scale;

	return { vec2(+0.000f, +0.750f) * r, vec2(-0.866f, -0.750f) * r, vec2(+0.866f, -0.750f) * r };
}

namespace null_gui {
	void multicombo(std::string text, std::vector<bool>* values, std::vector<std::string> items) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		std::array<vec2, 3> arrow = calc_arrow(gui_settings::combo_arrow_size);
		float arrow_size = arrow.at(0).dist_to(arrow.at(2));
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 min = text_size + gui_settings::combo_size + vec2(arrow_size, gui_settings::text_spacing);
		rect item_rect = rect(draw_pos, draw_pos + vec2(gui_settings::items_size_full_window ? math::max(min.x, wnd->get_window_size_with_padding()) : min.x, min.y));
		rect body_rect = rect(vec2(item_rect.min.x, item_rect.min.y + text_size.y + gui_settings::text_spacing), item_rect.max);
		std::vector<window_flags> flags = { window_flags::popup, window_flags::set_pos, window_flags::set_size, window_flags::auto_size, window_flags::no_title_line };

		deeps::add_item(item_rect.size(), name);
		if (!wnd->can_draw_item(item_rect))
			return;

		bool hovered, pressed;
		bool open = deeps::combo_behavior(body_rect, &hovered, &pressed, utils::format("##%s comboname", text.c_str()), flags);

		std::string text_on_combo = "";
		for (int a = 0; a < values->size(); a++) {
			if (values->at(a)) {
				if (a <= values->size() - 1 && text_on_combo.size() > 0) text_on_combo += ", ";
				text_on_combo += items[a];
			}
		}

		if (null_font::text_size(text_on_combo).x + null_font::text_size("...").x > item_rect.size().x - (body_rect.max.y - body_rect.min.y)) {
			for (int i = text_on_combo.size() - 1; i >= 0; i--) {
				if (null_font::text_size(text_on_combo).x + null_font::text_size("...").x < item_rect.size().x - (body_rect.max.y - body_rect.min.y)) {
					text_on_combo.pop_back();
					text_on_combo += "...";
					break;
				} else {
					text_on_combo.pop_back();
				}
			}
		}

		wnd->draw_list->draw_text(draw_text, item_rect.min, gui_settings::text, false);
		wnd->draw_list->draw_rect_filled(body_rect.min, body_rect.max, hovered || pressed ? pressed ? gui_settings::button_bg_active : gui_settings::button_bg_hovered : gui_settings::button_bg, gui_settings::combo_rounding);
		wnd->draw_list->draw_text(text_on_combo, vec2(body_rect.min.x + gui_settings::text_spacing, body_rect.max.y - ((body_rect.max.y - body_rect.min.y) / 2)), gui_settings::text, false, { false, true });

		vec2 arrow_pos = rect(vec2(body_rect.max.x - body_rect.size().y, body_rect.min.y), body_rect.max).centre();
		wnd->draw_list->draw_triangle_filled({ arrow_pos + arrow.at(0), arrow_pos + arrow.at(1), arrow_pos + arrow.at(2) }, gui_settings::main_color);

		if (deeps::window_exist(utils::format("##%s comboname", text.c_str()))) {
			deeps::push_var(&gui_settings::window_padding, vec2(0.f, 0.f)); {
				deeps::push_var(&gui_settings::item_spacing, 0.f); {
					if (begin_window(utils::format("##%s comboname", text.c_str()), vec2(body_rect.min.x, body_rect.max.y + gui_settings::combo_window_padding), vec2(body_rect.max.x - body_rect.min.x, 0.f), flags, nullptr)) {
						for (int i = 0; i < items.size(); i++) {
							if (i == gui_settings::max_auto_size_combo) {
								deeps::current_window->arg_size.y = deeps::current_window->size.y = deeps::current_window->max_size.y + gui_settings::window_padding.y - gui_settings::item_spacing;
								deeps::current_window->flags.remove(window_flags::auto_size);
							}

							if (selectable(items[i], values->at(i)))
								values->at(i) = !values->at(i);
						}
						end_window();
					}
				} deeps::pop_var();
			} deeps::pop_var();
		}
	}

	void multicombo(std::string text, std::vector<bool*> values, std::vector<std::string> items) {
		std::vector<bool> _values;
		for (bool* a : values)
			_values.push_back(*a);

		multicombo(text, &_values, items);

		for (int i = 0; i < values.size(); i++)
			*values.at(i) = _values.at(i);
	}
}