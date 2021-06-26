#pragma once
#define NOMINMAX
#include <windows.h>
#include <functional>
#include <array>
#include <string>
#include <vector>

#include "../helpers/vectors.h"

namespace null_input {
	enum class key_id : int {
		mouse_left,
		mouse_right,
		cancel,
		mouse_midle,
		mouse_x1,
		mouse_x2,
		backspace,
		tab,
		clear,
		enter,
		shift,
		ctrl,
		alt,
		pause,
		caps_lock,
		escape,
		space,
		page_up,
		page_down,
		end,
		home,
		left,
		up,
		right,
		down,
		print_screen,
		insert,
		del,

		zero, one, two, three, four,
		five, six, seven, eight, nine,

		a, b, c, d, e,
		f, g, h, i, j,
		k, l, m, n, o,
		p, q, r, s, t,
		u, v, w, x, y,
		z,

		win,
		app,

		num_0, num_1, num_2, num_3, num_4,
		num_5, num_6, num_7, num_8, num_9,
		num_multiply, num_add, num_subtract, num_decimal, num_divide,

		f1, f2, f3, f4, f5,
		f6, f7, f8, f9, f10,
		f11, f12, f13, f14, f15,
		f16, f17, f18, f19, f20,
		f21, f22, f23, f24,

		num_lock,
		scroll_lock,
		left_shift,
		right_shift,
		left_ctrl,
		right_ctrl,
		left_menu,
		right_menu,

		oem_1, oem_plus, oem_comma, oem_minus, oem_period,
		oem_2, oem_3, oem_4, oem_5, oem_6, oem_7
	};

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

	class key_data {
	public:
		key_data() {}
		key_data(int _id, std::string _name) : id(_id), name(_name) {}

		static bool char_allowed(int id) { return (id <= 0x44F && id >= 0x20) || id == 0x451 || id == 0x401; }
		static int get_array_id(std::string name);
		static int get_array_id(int id);
		static std::string get_name(int id, bool uppercase_check = false);

		std::string name;
		int id;
	};

	class input_key {
	public:
		input_key(key_data _data) : data(_data) { }

		bool down() { return state_down; }
		bool pressed() { return state_pressed; }
		bool clicked() { return state_clicked; }
		bool double_clicked() { return state_double_clicked; };
		void update_duration();

		void set_callback(key_state _callback_state, std::function<void(void)> _callback)  { callback_state = _callback_state; callback = _callback; }

		key_data data;
		float down_duration = 0.f;
		bool state_double_clicked = false;
		bool state_clicked = false;
		bool state_pressed = false;
		bool state_down = false;

		key_state callback_state;
		std::function<void(void)> callback = nullptr;

		float clicked_time;
	};

	extern std::vector<input_key> key_list;

	class bind_key {
	public:
		bind_key() {}
		bind_key(std::string _name, key_id key_id, bool* var, bind_type _type, std::array<std::string, 2> _text = { "enable", "disable" }, std::array<std::function<void(void)>, 3> _callbacks = { nullptr, nullptr, nullptr }) :
			name(_name), key(&key_list[(int)key_id]), type(_type), text(_text) { set_callbacks(_callbacks, { [var](){ *var = true; }, [var](){ *var = false; }, [var](){ *var = !*var; } }); };
		bind_key(std::string _name, std::string key_name, bool* var, bind_type _type, std::array<std::string, 2> _text = { "enable", "disable" }, std::array<std::function<void(void)>, 3> _callbacks = { nullptr, nullptr, nullptr }) :
			name(_name), key(&key_list[key_data::get_array_id(key_name)]), type(_type), text(_text) { set_callbacks(_callbacks, { [var](){ *var = true; }, [var](){ *var = false; }, [var](){ *var = !*var; } }); };

		void set_callbacks(std::array<std::function<void(void)>, 3> _callbacks, std::array<std::function<void(void)>, 3> default_callbacks) { for (int i = 0; i < _callbacks.size(); i++) callbacks[i] = _callbacks[i] == nullptr ? default_callbacks[i] : _callbacks[i]; }
		void call_callback(int id) { if(callbacks[id]) callbacks[id](); }

		bool can_show;
		bool binding;
		input_key* key;
		std::string name;
		bind_type type;
		std::array<std::string, 2> text;
		std::array<std::function<void(void)>, 3> callbacks;
	};

	namespace settings {
		inline float double_click_time = 0.30f;
		inline float double_click_max_dist = 6.f;
	}

	inline int last_symbol;
	inline int last_press_key;
	inline std::vector<bind_key*> bind_list;
	inline float mouse_wheel;
	inline vec2 mouse_pos;
	inline vec2 click_mouse_pos;

	bool process_mouse_message(UINT msg, WPARAM w_param, LPARAM l_param);
	bool process_keybd_message(UINT msg, WPARAM w_param, LPARAM l_param);
	LRESULT wnd_proc(UINT msg, WPARAM w_param, LPARAM l_param);

	void bind_control();
	void update_keys_state();

	static bool click_mouse_in_region(vec2 min, vec2 max) { return click_mouse_pos >= min && click_mouse_pos <= max; }
	static bool click_mouse_in_region(rect region) { return click_mouse_pos >= region.min && click_mouse_pos <= region.max; }
	static bool mouse_in_region(vec2 min, vec2 max) { return mouse_pos >= min && mouse_pos <= max; }
	static bool mouse_in_region(rect region) { return mouse_pos >= region.min && mouse_pos <= region.max; }
	static input_key* get_key(std::string name) { return &key_list[key_data::get_array_id(name)]; }
	static input_key* get_key(int id) { return &key_list[id]; }
	static input_key* get_key(key_id id) { return &key_list[(int)id]; }

	void create_bind(bool can_show, bind_key* bind);

	void write_clipboard(std::string);
	std::string read_clipboard();
}