#include "../null-gui.h"

namespace null_gui{
	void key_bind(std::string text, null_input::bind_key* bind) {
		window* wnd = deeps::current_window;
		if (!wnd) return;

		std::string name = utils::format("%s##%s", text.c_str(), wnd->name.c_str());
		std::string draw_text = deeps::format_item(name);
		std::string bind_text = bind->key->data.us;

		if (bind->binding) bind_text = "...";
		else if (bind->key->data.id == 0) bind_text = "-";

		vec2 draw_pos = wnd->draw_item_pos + vec2(0.f, wnd->get_scroll());
		vec2 text_size = null_font::text_size(draw_text);
		vec2 bind_text_size = null_font::text_size(bind_text);
		vec2 left_spacing = vec2(gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_spacing : 0, 0.f);
		vec2 right_spacing(gui_settings::spacing_checkbox_size ? gui_settings::checkbox_size + gui_settings::text_spacing : wnd->get_scroll_offset(), 0.f);
		rect size(!draw_text.empty() ? draw_pos + left_spacing : vec2(draw_pos.x + wnd->size.x - (gui_settings::window_padding.x * 2) - gui_settings::combo_size + left_spacing.x, draw_pos.y), vec2(draw_pos.x + wnd->size.x - gui_settings::window_padding.x - (left_spacing.x * 2 + gui_settings::window_padding.x), draw_pos.y + gui_settings::combo_size) + right_spacing);
		rect size_draw(vec2(size.max.x - bind_text_size.x - gui_settings::text_spacing * 2, size.max.y - size.size().y), size.max);

		if (!draw_text.empty())
			wnd->draw_list->draw_text(draw_text, vec2(size.min.x, size.min.y + size.size().y / 2), gui_settings::text, false, { false, true });

		bool hovered;
		bool active = null_gui::deeps::key_bind_behavior(bind, size_draw, &hovered, name);

		wnd->draw_list->draw_rect_filled(size_draw.min, size_draw.max, active ? gui_settings::button_bg_active : hovered ? gui_settings::button_bg_hovered : gui_settings::button_bg);
		wnd->draw_list->draw_text(bind_text, size_draw.min + size_draw.size() / 2, gui_settings::text, false, { true, true });

		deeps::add_item(size.size(), name);

		if (deeps::find_window(utils::format("##%s keybind tooltip", name.c_str())) != nullptr) {
			deeps::push_var({ &gui_settings::window_padding, vec2(5.f, 5.f) }); {
				deeps::push_var({ &gui_settings::spacing_checkbox_size, false }); {
					if (begin_window(utils::format("##%s keybind tooltip", name.c_str()), vec2(size_draw.max.x, size_draw.min.y), vec2(0.f, 0.f), { window_flags::no_move, window_flags::no_title_bar, window_flags::popup, window_flags::set_pos, window_flags::auto_size }, nullptr)) {
						//pohui, potom fixanu
						if (button("always", vec2(100, 16))) {
							bind->type = null_input::bind_type::always;
							deeps::close_current_window();
						}

						if(button("hold", vec2(100, 16))) {
							bind->type = null_input::bind_type::hold;
							deeps::close_current_window();
						}

						if (button("inversed hold", vec2(100, 16))) {
							bind->type = null_input::bind_type::hold_invers;
							deeps::close_current_window();
						}

						if (button("toggle", vec2(100, 16))) {
							bind->type = null_input::bind_type::toggle;
							deeps::close_current_window();
						}
						end_window();
					}
				} deeps::pop_var();
			} deeps::pop_var();
		}
	}
}