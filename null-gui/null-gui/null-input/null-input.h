#pragma once
#include <Windows.h>
#include <array>
#include "../../helpers/vectors.h"

namespace null_input {
	class key_state {
	public:
		bool down();
		bool pressed();
		bool clicked();
		void update_duration();

		float down_duration;
		bool state_clicked;
		bool state_pressed;
		bool state_down;
	};

	namespace vars {
		std::array<key_state, 512> keys;
		std::array<key_state, 5> mouse_buttons;
		float mouse_wheel;
		vec2 mouse_pos;
		vec2 last_click_pos;
	}

	LRESULT null_wnd_proc(UINT msg, WPARAM w_param, LPARAM l_param);

	void update_keys_state();

	vec2 last_click_pos() { return vars::last_click_pos; }
	vec2 mouse_pos() { return vars::mouse_pos; }
	bool last_click_in_region(vec2 min, vec2 max);
	bool mouse_in_region(vec2 min, vec2 max);
	bool mouse_in_region(rect region);
	key_state get_mouse_key_state(int key);
	key_state get_key_state(int key);
}