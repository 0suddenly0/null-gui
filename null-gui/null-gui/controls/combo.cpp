#include "../null-gui.h"

void render_arrow(null_render::draw_list* draw_list, vec2 pos, color col, float scale) {
	const float h = draw_list->_data->font_size * 1.00f;
	float r = h * 0.40f * scale;

	vec2 a = vec2(+0.000f, +0.750f) * r;
	vec2 b = vec2(-0.866f, -0.750f) * r;
	vec2 c = vec2(+0.866f, -0.750f) * r;

	draw_list->draw_triangle_filled(pos + a, pos + b, pos + c, col);
}

namespace null_gui {
	void combo(std::string text, int* value, std::vector<std::string> items) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll_offset());
		vec2 text_size = null_font::text_size(draw_text);
		rect size(draw_pos, vec2(draw_pos.x + wnd->size.x - gui_settings::window_padding.x - gui_settings::window_padding.x, draw_pos.y + gui_settings::combo_size + text_size.y + gui_settings::text_spacing) + wnd->get_scroll_thickness());
		rect size_draw(vec2(size.min.x, size.min.y + text_size.y + gui_settings::text_spacing), size.max);
		int clamped_value = math::clamp(*value, 0, (int)items.size() - 1);
		std::vector<window_flags> flags = { window_flags::popup, window_flags::set_pos, window_flags::set_size, window_flags::auto_size, window_flags::no_title_line };

		bool hovered, pressed;
		bool open = deeps::combo_behavior(size_draw, items.size(), &hovered, &pressed, utils::format("##%s combo popup", text.c_str()), flags);

		wnd->draw_list->draw_text(draw_text, size.min, gui_settings::text, false);
		wnd->draw_list->draw_rect_filled(size_draw.min, size_draw.max, gui_settings::button_bg, gui_settings::combo_rounding);

		render_arrow(wnd->draw_list, rect(vec2(size_draw.max.x - (size_draw.max.y - size_draw.min.y), size_draw.min.y), size_draw.max).centre(), gui_settings::main_color, gui_settings::combo_arrow_size);

		wnd->draw_list->push_clip_rect(size_draw.min, size_draw.max - vec2(size_draw.max.y - size_draw.min.y, 0.f), true); {
			wnd->draw_list->draw_text(items[clamped_value], vec2(size_draw.min.x + gui_settings::text_spacing, size_draw.max.y - ((size_draw.max.y - size_draw.min.y) / 2)), gui_settings::text, false, { false, true });

		} wnd->draw_list->pop_clip_rect();

		deeps::add_item(size.size(), name);

		if (deeps::window_exist(utils::format("##%s combo popup", text.c_str()))) {
			deeps::push_var(&gui_settings::window_padding, vec2(0.f, 0.f)); {
				deeps::push_var(&gui_settings::item_spacing, 0.f); {
					if (begin_window(utils::format("##%s combo popup", text.c_str()), vec2(size_draw.min.x, size_draw.max.y), vec2(size_draw.max.x - size_draw.min.x, 0.f), flags, nullptr)) {
						for (int i = 0; i < items.size(); i++) {
							if (i == 5) {
								deeps::current_window->arg_size.y = deeps::current_window->size.y = deeps::current_window->max_size.y + gui_settings::window_padding.y - gui_settings::item_spacing;
								deeps::current_window->flags.remove(window_flags::auto_size);
							}

							if (selectable(items[i], *value == i)) *value = i;
						}
						end_window();
					}
				} deeps::pop_var();
			} deeps::pop_var();
		}
	}
}