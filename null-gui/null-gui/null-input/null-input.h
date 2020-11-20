#pragma once
#include <Windows.h>
#include <array>
#include <string>
#include <vector>
#include "../../helpers/vectors.h"

namespace null_input {
	namespace key_names {
		bool for_input(int id);

		int get_id(std::string name);
		std::string get_name(int id, bool language_and_shift = false);
	}

	class key_state {
	public:
		bool down() { return state_down; }
		bool pressed() { return state_pressed; }
		bool clicked() { return state_clicked; }
		bool double_clicked() { return state_double_clicked; };
		void update_duration();

		float down_duration;
		bool state_double_clicked;
		bool state_clicked;
		bool state_pressed;
		bool state_down;

		float clicked_time;
	};

	namespace vars {
		int last_press_key;
		std::array<key_state, 512> keys;
		float mouse_wheel;
		vec2 mouse_pos;
		vec2 mouse_click_pos;
	}

	LRESULT null_wnd_proc(UINT msg, WPARAM w_param, LPARAM l_param);

	void update_keys_state();

	vec2 mouse_pos() { return vars::mouse_pos; }
	bool mouse_in_region(vec2 min, vec2 max) { return mouse_pos() <= min && mouse_pos() >= max; }
	bool mouse_in_region(rect region) { return mouse_pos() <= region.min && mouse_pos() >= region.max; }
	key_state get_key_state(std::string name) { return vars::keys[key_names::get_id(name)]; }
	key_state get_key_state(int id) { return vars::keys[id]; }

	void write_clipboard(std::string);
	std::string read_clipboard();
}