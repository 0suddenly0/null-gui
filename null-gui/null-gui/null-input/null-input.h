#pragma once
#include <Windows.h>
#include <functional>
#include <array>
#include <string>
#include <vector>
#include "../../helpers/vectors.h"

namespace null_input {
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

	class key_name {
	public:
		key_name() {}
		key_name(std::string _us, int _id);
		key_name(std::string _us, std::string _rus, int _id);
		key_name(std::string _us, std::string _rus, std::string _us_shift, int _id);
		key_name(std::string _us, std::string _rus, std::string _us_shift, std::string _rus_shift, int _id);

		static bool for_input(int id);
		static int get_id(std::string name);
		static std::string get_name(int id, bool language_and_shift = false);

		std::string us;
		std::string us_shift;
		std::string rus;
		std::string rus_shift;
		int id;
	};

	class input_key {
	public:
		input_key(key_name _data) {
			data = _data;
			down_duration = 0.f;
			state_double_clicked = false;
			state_clicked = false;
			state_pressed = false;
			state_down = false;
		}

		bool down() { return state_down; }
		bool pressed() { return state_pressed; }
		bool clicked() { return state_clicked; }
		bool double_clicked() { return state_double_clicked; };
		void update_duration();

		void set_callback(key_state _callback_state, std::function<void(void)> _callback) { callback_state = _callback_state; callback = _callback; }

		key_name data;
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
		bool binding;
		input_key* key;
		std::string name;
		bind_type type;
		std::array<std::string, 2> text;
		std::array<std::function<void(void)>, 3> callbacks;
	};

	namespace vars {
		int last_press_key;
		extern std::vector<input_key> keys;
		std::vector<bind_key*> binds;
		float mouse_wheel;
		vec2 mouse_pos;
		vec2 mouse_click_pos;
	}

	void begin_input();
	LRESULT null_wnd_proc(UINT msg, WPARAM w_param, LPARAM l_param);

	void bind_control();
	void update_keys_state();

	vec2 mouse_pos() { return vars::mouse_pos; }
	bool mouse_in_region(vec2 min, vec2 max) { return mouse_pos() <= min && mouse_pos() >= max; }
	bool mouse_in_region(rect region) { return mouse_pos() <= region.min && mouse_pos() >= region.max; }
	input_key* get_key(std::string name) { return &vars::keys[key_name::get_id(name)]; }
	input_key* get_key(int id) { return &vars::keys[id]; }

	void create_bind(bool can_show, bind_key* bind);

	void write_clipboard(std::string);
	std::string read_clipboard();
}