#pragma once
#define NOMINMAX
#include <windows.h>
#include <functional>
#include <array>
#include <string>
#include <vector>
#include "../helpers/vectors.h"

namespace null_input {
	enum class bind_type {
		always = 0,
		hold_on,
		hold_off,
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
		//удалить этот говнокод нахуй
		key_name() {}
		key_name(std::string _us, int _id) : us(_us), id(_id) {};
		key_name(std::string _us, std::string _rus, int _id) : us(_us), rus(_rus), id(_id) {};
		key_name(std::string _us, std::string _rus, std::string _us_shift, int _id) : us(_us), rus(_rus), us_shift(_us_shift), id(_id) {};
		key_name(std::string _us, std::string _rus, std::string _us_shift, std::string _rus_shift, int _id) : us(_us), rus(_rus), us_shift(_us_shift), rus_shift(_rus_shift), id(_id) {};

		static bool for_input(int id);
		static int get_array_id(std::string name);
		static int get_array_id(int id);
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

	int last_press_key;
	extern std::vector<input_key> key_list;
	std::vector<bind_key*> bind_list;
	float mouse_wheel;
	vec2 mouse_pos;
	vec2 click_mouse_pos;

	LRESULT null_wnd_proc(UINT msg, WPARAM w_param, LPARAM l_param);

	void bind_control();
	void update_keys_state();

	bool click_mouse_in_region(vec2 min, vec2 max) { return click_mouse_pos <= min && click_mouse_pos >= max; }
	bool click_mouse_in_region(rect region) { return click_mouse_pos <= region.min && click_mouse_pos >= region.max; }
	bool mouse_in_region(vec2 min, vec2 max) { return mouse_pos <= min && mouse_pos >= max; }
	bool mouse_in_region(rect region) { return mouse_pos <= region.min && mouse_pos >= region.max; }
	input_key* get_key(std::string name) { return &key_list[key_name::get_array_id(name)]; }
	input_key* get_key(int id) { return &key_list[key_name::get_array_id(id)]; }

	void create_bind(bool can_show, bind_key* bind);

	void write_clipboard(std::string);
	std::string read_clipboard();
}