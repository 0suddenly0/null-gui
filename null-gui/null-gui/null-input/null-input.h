#pragma once
#include <Windows.h>
#include <functional>
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

	enum class bind_type {
		always = 0,
		hold,
		hold_invers,
		toggle
	};

	enum class key_state {
		none = 0,
		up,
		down,
		clicked,
		pressed,
		double_clicked
	};

	class input_key {
	public:
		bool down() { return state_down; }
		bool pressed() { return state_pressed; }
		bool clicked() { return state_clicked; }
		bool double_clicked() { return state_double_clicked; };
		void update_duration();

		void set_callback(key_state _callback_state, std::function<void(void)> _callback) { callback_state = _callback_state; callback = _callback; }

		float down_duration;
		bool state_double_clicked;
		bool state_clicked;
		bool state_pressed;
		bool state_down;

		key_state callback_state;
		std::function<void(void)> callback = nullptr;

		float clicked_time;
	};

	class bind_key {
	public:
		bind_key(std::string _name, int key_id, bool* var, bind_type _type);
		bind_key(std::string _name, std::string key_name, bool* var, bind_type _type);
		bind_key(std::string _name, int key_id, bool* var, bind_type _type, std::array<std::string, 2> _text, std::array<std::function<void(void)>, 3> _callback);
		bind_key(std::string _name, std::string key_name, bool* var, bind_type _type, std::array<std::string, 2> _text, std::array<std::function<void(void)>, 3> _callback);

		void set_callback(std::array<std::function<void(void)>, 3> _callbacks) { callbacks = _callbacks; }

		bool can_show;
		input_key* key;
		std::string name;
		bind_type type;
		std::array<std::string, 2> text;
		std::array<std::function<void(void)>, 3> callbacks;
	};

	namespace vars {
		int last_press_key;
		std::array<input_key, 512> keys;
		std::vector<bind_key*> binds;
		float mouse_wheel;
		vec2 mouse_pos;
		vec2 mouse_click_pos;
	}

	LRESULT null_wnd_proc(UINT msg, WPARAM w_param, LPARAM l_param);

	void bind_control();
	void update_keys_state();

	vec2 mouse_pos() { return vars::mouse_pos; }
	bool mouse_in_region(vec2 min, vec2 max) { return mouse_pos() <= min && mouse_pos() >= max; }
	bool mouse_in_region(rect region) { return mouse_pos() <= region.min && mouse_pos() >= region.max; }
	input_key* get_key(std::string name) { return &vars::keys[key_names::get_id(name)]; }
	input_key* get_key(int id) { return &vars::keys[id]; }

	void create_bind(bool can_show, bind_key* bind);

	void write_clipboard(std::string);
	std::string read_clipboard();
}