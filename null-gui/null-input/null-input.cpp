#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <windowsx.h>

#include "null-input.h"
#include "../null-gui/null-gui.h"

bool process_mouse_message(UINT u_msg, WPARAM w_param, LPARAM l_param) {
    int id = 0;
    bool state = false;

    switch (u_msg) {
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
        state = u_msg == WM_MBUTTONUP ? false : true;
		id = null_input::key_data::get_array_id("mouse midle");
        break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        state = u_msg == WM_RBUTTONUP ? false : true;
		id = null_input::key_data::get_array_id("mouse right");
        break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        state = u_msg == WM_LBUTTONUP ? false : true;
		id = null_input::key_data::get_array_id("mouse left");
        break;
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
        state = u_msg == WM_XBUTTONUP ? false : true;
		id = (HIWORD(w_param) == XBUTTON1 ? null_input::key_data::get_array_id("mouse x1") : null_input::key_data::get_array_id("mouse x2"));
        break;
    default:
        return false;
    }

	null_input::input_key* key = &null_input::key_list[id];

	if (key->callback && ((key->callback_state == null_input::key_state::down && state == true) || (key->callback_state == null_input::key_state::up && state == false)))
		key->callback();

	key->state_down = state;
	if (state) {
		null_input::click_mouse_pos = null_input::mouse_pos;
		null_input::last_press_key = id;
	}

    return true;
}

bool process_keybd_message(UINT u_msg, WPARAM w_param, LPARAM l_param) {
    int id = null_input::key_data::get_array_id(w_param);
	null_input::input_key* key = &null_input::key_list[id];
    bool state = false;

    switch (u_msg) {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        state = true;
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        state = false;
        break;
    }

	if (key->callback != nullptr && ((key->callback_state == null_input::key_state::down && state == true) || (key->callback_state == null_input::key_state::up && state == false)))
		key->callback();

	key->state_down = state;
	if (state) {
		null_gui::deeps::text_input_info::win_poc(id);
		null_input::last_press_key = id;
	}

    return true;
}

std::vector<null_input::input_key> null_input::key_list = {
	(null_input::key_data( 1, "mouse left" )),
	(null_input::key_data( 2, "mouse right" )),
	(null_input::key_data( 3, "cancel" )),
	(null_input::key_data( 4, "mouse midle" )),
	(null_input::key_data( 5, "mouse x1" )),
	(null_input::key_data( 6, "mouse x2" )),
	(null_input::key_data( 8, "backspace" )),
	(null_input::key_data( 9, "tab" )),
	(null_input::key_data( 12, "clear"  )),
	(null_input::key_data( 13, "enter" )),
	(null_input::key_data( 16, "shift" )),
	(null_input::key_data( 17, "ctrl" )),
	(null_input::key_data( 18, "alt" )),
	(null_input::key_data( 19, "pause" )),
	(null_input::key_data( 20, "caps lock" )),
	(null_input::key_data( 27, "escape" )),
	(null_input::key_data( 32, "space" )),
	(null_input::key_data( 33, "page up" )),
	(null_input::key_data( 34, "page down" )),
	(null_input::key_data( 35, "end" )),
	(null_input::key_data( 36, "home" )),
	(null_input::key_data( 37, "left" )),
	(null_input::key_data( 38, "up" )),
	(null_input::key_data( 39, "right" )),
	(null_input::key_data( 40, "down" )),
	(null_input::key_data( 44, "print screen" )),
	(null_input::key_data( 45, "insert" )),
	(null_input::key_data( 46, "del" )),

	(null_input::key_data( 48, "0" )), (null_input::key_data( 49, "1" )), (null_input::key_data( 50, "2" )), (null_input::key_data( 51, "3" )), (null_input::key_data( 52, "4" )),
	(null_input::key_data( 53, "5" )), (null_input::key_data( 54, "6" )), (null_input::key_data( 55, "7" )), (null_input::key_data( 56, "8" )), (null_input::key_data( 57, "9" )),

	(null_input::key_data( 65, "a" )), (null_input::key_data( 66, "b" )), (null_input::key_data( 67, "c" )), (null_input::key_data( 68, "d" )), (null_input::key_data( 69, "e" )),
	(null_input::key_data( 70, "f" )), (null_input::key_data( 71, "g" )), (null_input::key_data( 72, "h" )), (null_input::key_data( 73, "i" )), (null_input::key_data( 74, "j" )),
	(null_input::key_data( 75, "k" )), (null_input::key_data( 76, "l" )), (null_input::key_data( 77, "m" )), (null_input::key_data( 78, "n" )), (null_input::key_data( 79, "o" )),
	(null_input::key_data( 80, "p" )), (null_input::key_data( 81, "q" )), (null_input::key_data( 82, "r" )), (null_input::key_data( 83, "s" )), (null_input::key_data( 84, "t" )),
	(null_input::key_data( 85, "u" )), (null_input::key_data( 86, "v" )), (null_input::key_data( 87, "w" )), (null_input::key_data( 88, "x" )), (null_input::key_data( 89, "y" )),
	(null_input::key_data( 90, "z" )),

	(null_input::key_data( 91, "win" )),
	(null_input::key_data( 93, "app" )),
	
	(null_input::key_data( 96, "num 0" )), (null_input::key_data( 97, "num 1" )), (null_input::key_data( 98, "num 2" )), (null_input::key_data( 99, "num 3" )), (null_input::key_data( 100, "num 4" )),
	(null_input::key_data( 101, "num 5" )), (null_input::key_data( 102, "num 6" )), (null_input::key_data( 103, "num 7" )), (null_input::key_data( 104, "num 8" )), (null_input::key_data( 105, "num 9" )),
	(null_input::key_data( 106, "num *" )), (null_input::key_data( 107, "num +" )), (null_input::key_data( 109, "num -" )), (null_input::key_data( 110, "num ." )), (null_input::key_data( 111, "num /" )),
	
	(null_input::key_data( 112, "f1" )), (null_input::key_data( 113, "f2" )), (null_input::key_data( 114, "f3" )), (null_input::key_data( 115, "f4" )), (null_input::key_data( 116, "f5" )),
	(null_input::key_data( 117, "f6" )), (null_input::key_data( 118, "f7" )), (null_input::key_data( 119, "f8" )), (null_input::key_data( 120, "f9" )),(null_input::key_data( 121, "f10" )),
	(null_input::key_data( 122, "f11" )), (null_input::key_data( 123, "f12" )), (null_input::key_data( 124, "f13" )), (null_input::key_data( 125, "f14" )), (null_input::key_data( 126, "f15" )),
	(null_input::key_data( 127, "f16" )), (null_input::key_data( 128, "f17" )), (null_input::key_data( 129, "f18" )), (null_input::key_data( 130, "f19" )), (null_input::key_data( 131, "f20" )),
	(null_input::key_data( 132, "f21" )), (null_input::key_data( 133, "f22" )), (null_input::key_data( 134, "f23" )), (null_input::key_data( 135, "f24" )),
	
	(null_input::key_data( 144, "num lock" )),
	(null_input::key_data( 145, "scroll lock" )),
	(null_input::key_data( 160, "left shift" )),
	(null_input::key_data( 161, "right shift" )),
	(null_input::key_data( 162, "left ctrl" )),
	(null_input::key_data( 163, "right ctrl" )),
	(null_input::key_data( 164, "left menu" )),
	(null_input::key_data( 165, "right menu" )),
	
	(null_input::key_data( 186, ";" )), (null_input::key_data( 187, "=" )), (null_input::key_data( 188, "," )), (null_input::key_data( 189, "-" )), (null_input::key_data( 190, "." )),
	(null_input::key_data( 191, "/" )), (null_input::key_data( 192, "`" )), (null_input::key_data( 219, "[" )), (null_input::key_data( 220, "\\" )), (null_input::key_data( 221, "]" )),
	(null_input::key_data( 222, "\'" ))
};

namespace null_input {
	bool key_data::char_allowed(int id) {
		return (id <= 0x44F && id >= 0x20) || id == 0x451 || id == 0x401;
	}

	int key_data::get_array_id(std::string name) {
		std::vector<input_key>::iterator ret = std::find_if(key_list.begin(), key_list.end(), [=](input_key& a) { return a.data.name == name; });

		if (ret == key_list.end()) return 0;
		else return std::distance(key_list.begin(), ret);
	}

	int key_data::get_array_id(int id) {
		std::vector<input_key>::iterator ret = std::find_if(key_list.begin(), key_list.end(), [=](input_key& a) { return a.data.id == id; });

		if (ret == key_list.end()) return 0;
		else return std::distance(key_list.begin(), ret);
	}

	std::string key_data::get_name(int id, bool uppercase_check) {
		key_data key = key_list[id].data;
		bool caps = (GetKeyState(get_key(key_id::caps_lock)->data.id) & 0x0001) != 0;

		if (uppercase_check) {
			if (id == (int)key_id::space) return " ";

			if (key.name.size() == 1 && key.name.back() >= 'a' && key.name.back() <= 'z' && (get_key(key_id::shift)->down() || caps)) {
				std::string result = key.name;
				if (get_key(key_id::shift)->down() && caps) return result;
				result.back() = key.name.back() + ('A' - 'a');
				return result;
			}

			return key.name;
		}

		return key.name;
	}


	void input_key::update_duration() {
		down_duration = down() ? (down_duration < 0.0f ? 0.0f : down_duration + null_gui::deeps::delta_time) : -1.0f;
	}


	bind_key::bind_key(std::string _name, int key_id, bool* var, bind_type _type, std::array<std::string, 2> _text, std::array<std::function<void(void)>, 3> _callback) {
		key = get_key(key_id);
		name = _name;
		type = _type;
		text = _text;
		set_callback(_callback);
	}

	bind_key::bind_key(std::string _name, std::string key_name, bool* var, bind_type _type, std::array<std::string, 2> _text, std::array<std::function<void(void)>, 3> _callback) {
		key = get_key(key_name);
		name = _name;
		type = _type;
		text = _text;
		set_callback(_callback);
	}

	LRESULT null_wnd_proc(UINT msg, WPARAM w_param, LPARAM l_param) {
		switch (msg) {
		case WM_CHAR:
			last_symbol = (int)w_param;
			null_gui::deeps::text_input_info::add_char();
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_LBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		case WM_XBUTTONUP:
			return process_mouse_message(msg, w_param, l_param);
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			return process_keybd_message(msg, w_param, l_param);
		case WM_MOUSEWHEEL:
			mouse_wheel += GET_WHEEL_DELTA_WPARAM(w_param) > 0 ? +1.0f : -1.0f;
			return true;
		case WM_MOUSEMOVE:
			mouse_pos = vec2((signed short)(l_param), (signed short)(l_param >> 16));
			return true;
		}
		return false;
	}

	void bind_control() {
		for (bind_key* bind : bind_list) {
			if (bind->binding && bind->type != bind_type::always) continue;
			switch (bind->type) {
			case bind_type::hold_on: {
				if (bind->key->down()) bind->call_callback(0);
				else bind->call_callback(1);
			} break;
			case bind_type::hold_off: {
				if (!bind->key->down()) bind->call_callback(0);
				else bind->call_callback(1);
			} break;
			case bind_type::toggle: if (bind->key->clicked()) bind->call_callback(2); break;
			case bind_type::always: bind->call_callback(0); break;
			}
		}
	}

	void update_keys_state() {
		static vec2 mouse_click_pos;
		for (input_key& key : key_list) {
			key.state_clicked = key.down() && key.down_duration < 0.0f;
			key.state_pressed = !key.down() && key.down_duration >= 0.f;

			if (key.callback && ((key.callback_state == null_input::key_state::clicked && key.state_clicked) || (key.callback_state == null_input::key_state::pressed && key.state_pressed)))
				key.callback();

			key.update_duration();
			key.state_double_clicked = false;
			if (key.state_clicked) {
				if ((float)(null_gui::deeps::real_time - key.clicked_time) < null_gui::gui_settings::double_click_time) {
					vec2 delta = mouse_pos - mouse_click_pos;
					if (delta.length() < pow(null_gui::gui_settings::double_click_max_dist, 2)) {
						key.state_double_clicked = true;

						if (key.callback && key.callback_state == null_input::key_state::double_clicked)
							key.callback();
					}
					key.clicked_time = -FLT_MAX;
				} else {
					key.clicked_time = null_gui::deeps::real_time;
				}
				mouse_click_pos = mouse_pos;
			}
		}
	}

	void create_bind(bool can_show, bind_key* bind) {
		bind->can_show = can_show;

		std::vector<bind_key*>::iterator finded = std::find_if(bind_list.begin(), bind_list.end(), [=](bind_key* first) { return first->name == bind->name; });

		if (can_show) {
			if (finded == bind_list.end()) bind_list.push_back(bind);
		} else {
			bind_list.erase(bind_list.begin() + std::distance(bind_list.begin(), finded));
			bind->callbacks[1]();
		}
	}

	void write_clipboard(std::string value) {
		if (OpenClipboard(null_gui::deeps::hwnd)) {
			EmptyClipboard();
			HGLOBAL hgBuffer = GlobalAlloc(GMEM_DDESHARE, value.length() + 1);
			char* chBuffer = (char*)GlobalLock(hgBuffer);
			strcpy(chBuffer, LPCSTR(value.c_str()));
			GlobalUnlock(hgBuffer);
			SetClipboardData(CF_TEXT, hgBuffer);
			CloseClipboard();
		}
	}

	std::string read_clipboard() {
		std::string result = "";
		if (OpenClipboard(null_gui::deeps::hwnd)) {
			HANDLE hData = GetClipboardData(CF_TEXT);
			if (hData) {
				char* chBuffer = (char*)GlobalLock(hData);
				result = chBuffer;
			}
			GlobalUnlock(hData);
			CloseClipboard();
		}
		return result;
	}
}