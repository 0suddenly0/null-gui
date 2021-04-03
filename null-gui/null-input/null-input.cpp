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
		id = null_input::key_name::get_array_id("mouse midle");
        break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        state = u_msg == WM_RBUTTONUP ? false : true;
		id = null_input::key_name::get_array_id("mouse right");
        break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        state = u_msg == WM_LBUTTONUP ? false : true;
		id = null_input::key_name::get_array_id("mouse left");
        break;
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
        state = u_msg == WM_XBUTTONUP ? false : true;
		id = (HIWORD(w_param) == XBUTTON1 ? null_input::key_name::get_array_id("mouse x1") : null_input::key_name::get_array_id("mouse x2"));
        break;
    default:
        return false;
    }

	null_input::input_key* key = &null_input::key_list[id];

	if (key->callback && (key->callback_state == null_input::key_state::down && state == true) || (key->callback_state == null_input::key_state::up && state == false))
		key->callback();

	key->state_down = state;
	if (state) {
		null_input::click_mouse_pos = null_input::mouse_pos;
		null_input::last_press_key = id;
	}

    return true;
}

bool process_keybd_message(UINT u_msg, WPARAM w_param, LPARAM l_param) {
    int id = null_input::key_name::get_array_id(w_param);
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
    default:
        return false;
    }

	if (key->callback && (key->callback_state == null_input::key_state::down && state == true) || (key->callback_state == null_input::key_state::up && state == false))
		key->callback();

	key->state_down = state;
	if (state) {
		null_gui::deeps::text_input_info::win_poc(id);
		null_input::last_press_key = id;
	}

    return true;
}

std::vector<null_input::input_key> null_input::key_list = {
	//(null_input::key_name{ "unkown key", 0 }),
	(null_input::key_name{ "mouse left", 1 }),
	(null_input::key_name{ "mouse right", 2 }),
	(null_input::key_name{ "cancel", 3 }),
	(null_input::key_name{ "mouse midle", 4 }),
	(null_input::key_name{ "mouse x1", 5 }),
	(null_input::key_name{ "mouse x2", 6 }),
	//(null_input::key_name{ "unkown key", 7 }),
	(null_input::key_name{ "backspace", 8 }),
	(null_input::key_name{ "tab", 9 }),
	/*(null_input::key_name{ "unkown key", 10 }),
	(null_input::key_name{ "unkown key", 11 }),*/
	(null_input::key_name{ "clear", 12  }),
	(null_input::key_name{ "enter", 13 }),
	/*(null_input::key_name{ "unkown key", 14 }),
	(null_input::key_name{ "unkown key", 15 }),*/
	(null_input::key_name{ "shift", 16 }),
	(null_input::key_name{ "ctrl", 17 }),
	(null_input::key_name{ "alt", 18 }),
	(null_input::key_name{ "pause", 19 }),
	(null_input::key_name{ "caps lock", 20 }),
	/*(null_input::key_name{ "unkown key", 21 }),
	(null_input::key_name{ "unkown key", 22 }),
	(null_input::key_name{ "unkown key", 23 }),
	(null_input::key_name{ "unkown key", 24 }),
	(null_input::key_name{ "unkown key", 25 }),
	(null_input::key_name{ "unkown key", 26 }),*/
	(null_input::key_name{ "escape", 27 }),
	/*(null_input::key_name{ "unkown key", 28 }),
	(null_input::key_name{ "unkown key", 29 }),
	(null_input::key_name{ "unkown key", 30 }),
	(null_input::key_name{ "unkown key", 31 }),*/
	(null_input::key_name{ "space", 32 }),
	(null_input::key_name{ "page up", 33 }),
	(null_input::key_name{ "page down", 34 }),
	(null_input::key_name{ "end", 35 }),
	(null_input::key_name{ "home", 36 }),
	(null_input::key_name{ "left", 37 }),
	(null_input::key_name{ "up", 38 }),
	(null_input::key_name{ "right", 39 }),
	(null_input::key_name{ "down", 40 }),
	/*(null_input::key_name{ "unkown key", 41 }),
	(null_input::key_name{ "unkown key", 42 }),
	(null_input::key_name{ "unkown key", 43 }),*/
	(null_input::key_name{ "print screen", 44 }),
	(null_input::key_name{ "insert", 45 }),
	(null_input::key_name{ "del", 46 }),
	//(null_input::key_name{ "unkown key", 47 }),
	(null_input::key_name{ "0", "", ")", 48 }),
	(null_input::key_name{ "1", "", "!", 49 }),
	(null_input::key_name{ "2", "", "@", "\"", 50 }),
	(null_input::key_name{ "3", "", "#", "¹",51 }),
	(null_input::key_name{ "4", "", "$", ";",52 }),
	(null_input::key_name{ "5", "", "%", "%",53 }),
	(null_input::key_name{ "6", "", "^", ":",54 }),
	(null_input::key_name{ "7", "", "&", "?",55 }),
	(null_input::key_name{ "8", "", "*", "*",56 }),
	(null_input::key_name{ "9", "", "(", 57 }),
	/*(null_input::key_name{ "unkown key", 58 }),
	(null_input::key_name{ "unkown key", 59 }),
	(null_input::key_name{ "unkown key", 60 }),
	(null_input::key_name{ "unkown key", 61 }),
	(null_input::key_name{ "unkown key", 62 }),
	(null_input::key_name{ "unkown key", 63 }),
	(null_input::key_name{ "unkown key", 64 }),*/
	(null_input::key_name{ "a", "ô", 65 }),
	(null_input::key_name{ "b", "è", 66 }),
	(null_input::key_name{ "c", "ñ", 67 }),
	(null_input::key_name{ "d", "â", 68 }),
	(null_input::key_name{ "e", "ó", 69 }),
	(null_input::key_name{ "f", "à", 70 }),
	(null_input::key_name{ "g", "ï", 71 }),
	(null_input::key_name{ "h", "ð", 72 }),
	(null_input::key_name{ "i", "ø", 73 }),
	(null_input::key_name{ "j", "î", 74 }),
	(null_input::key_name{ "k", "ë", 75 }),
	(null_input::key_name{ "l", "ä", 76 }),
	(null_input::key_name{ "m", "ü", 77 }),
	(null_input::key_name{ "n", "ò", 78 }),
	(null_input::key_name{ "o", "î", 79 }),
	(null_input::key_name{ "p", "ç", 80 }),
	(null_input::key_name{ "q", "é", 81 }),
	(null_input::key_name{ "r", "ê", 82 }),
	(null_input::key_name{ "s", "û", 83 }),
	(null_input::key_name{ "t", "å", 84 }),
	(null_input::key_name{ "u", "ã", 85 }),
	(null_input::key_name{ "v", "ì", 86 }),
	(null_input::key_name{ "w", "ö", 87 }),
	(null_input::key_name{ "x", "÷", 88 }),
	(null_input::key_name{ "y", "í", 89 }),
	(null_input::key_name{ "z", "ÿ", 90 }),
	(null_input::key_name{ "win", 91 }),
	//(null_input::key_name{ "unkown key", 92 }),
	(null_input::key_name{ "app", 93 }),
	/*(null_input::key_name{ "unkown key", 94 }),
	(null_input::key_name{ "unkown key", 95 }),*/
	(null_input::key_name{ "num 0", 96 }),
	(null_input::key_name{ "num 1", 97 }),
	(null_input::key_name{ "num 2", 98 }),
	(null_input::key_name{ "num 3", 99 }),
	(null_input::key_name{ "num 4", 100 }),
	(null_input::key_name{ "num 5", 101 }),
	(null_input::key_name{ "num 6", 102 }),
	(null_input::key_name{ "num 7", 103 }),
	(null_input::key_name{ "num 8", 104 }),
	(null_input::key_name{ "num 9", 105 }),
	(null_input::key_name{ "num *", 106 }),
	(null_input::key_name{ "num +", 107 }),
	//(null_input::key_name{ "unkown key", 108 }),
	(null_input::key_name{ "num -", 109 }),
	(null_input::key_name{ "num .", 110 }),
	(null_input::key_name{ "num /", 111 }),
	(null_input::key_name{ "f1", 112 }),
	(null_input::key_name{ "f2", 113 }),
	(null_input::key_name{ "f3", 114 }),
	(null_input::key_name{ "f4", 115 }),
	(null_input::key_name{ "f5", 116 }),
	(null_input::key_name{ "f6", 117 }),
	(null_input::key_name{ "f7", 118 }),
	(null_input::key_name{ "f8", 119 }),
	(null_input::key_name{ "f9", 120 }),
	(null_input::key_name{ "f10", 121 }),
	(null_input::key_name{ "f11", 122 }),
	(null_input::key_name{ "f12", 123 }),
	(null_input::key_name{ "f13", 124 }),
	(null_input::key_name{ "f14", 125 }),
	(null_input::key_name{ "f15", 126 }),
	(null_input::key_name{ "f16", 127 }),
	(null_input::key_name{ "f17", 128 }),
	(null_input::key_name{ "f18", 129 }),
	(null_input::key_name{ "f19", 130 }),
	(null_input::key_name{ "f20", 131 }),
	(null_input::key_name{ "f21", 132 }),
	(null_input::key_name{ "f22", 133 }),
	(null_input::key_name{ "f23", 134 }),
	(null_input::key_name{ "f24", 135 }),
	/*(null_input::key_name{ "unkown key", 136 }),
	(null_input::key_name{ "unkown key", 137 }),
	(null_input::key_name{ "unkown key", 138 }),
	(null_input::key_name{ "unkown key", 139 }),
	(null_input::key_name{ "unkown key", 140 }),
	(null_input::key_name{ "unkown key", 141 }),
	(null_input::key_name{ "unkown key", 142 }),
	(null_input::key_name{ "unkown key", 143 }),*/
	(null_input::key_name{ "num lock", 144 }),
	(null_input::key_name{ "scroll lock", 145 }),
	/*(null_input::key_name{ "unkown key", 146 }),
	(null_input::key_name{ "unkown key", 147 }),
	(null_input::key_name{ "unkown key", 148 }),
	(null_input::key_name{ "unkown key", 149 }),
	(null_input::key_name{ "unkown key", 150 }),
	(null_input::key_name{ "unkown key", 151 }),
	(null_input::key_name{ "unkown key", 152 }),
	(null_input::key_name{ "unkown key", 153 }),
	(null_input::key_name{ "unkown key", 154 }),
	(null_input::key_name{ "unkown key", 155 }),
	(null_input::key_name{ "unkown key", 156 }),
	(null_input::key_name{ "unkown key", 157 }),
	(null_input::key_name{ "unkown key", 158 }),
	(null_input::key_name{ "unkown key", 159 }),*/
	(null_input::key_name{ "left shift", 160 }),
	(null_input::key_name{ "right shift", 161 }),
	(null_input::key_name{ "left ctrl", 162 }),
	(null_input::key_name{ "right ctrl", 163 }),
	(null_input::key_name{ "left menu", 164 }),
	(null_input::key_name{ "right menu", 165 }),
	/*(null_input::key_name{ "unkown key", 166 }),
	(null_input::key_name{ "unkown key", 167 }),
	(null_input::key_name{ "unkown key", 168 }),
	(null_input::key_name{ "unkown key", 169 }),
	(null_input::key_name{ "unkown key", 170 }),
	(null_input::key_name{ "unkown key", 171 }),
	(null_input::key_name{ "unkown key", 172 }),
	(null_input::key_name{ "unkown key", 173 }),
	(null_input::key_name{ "unkown key", 174 }),
	(null_input::key_name{ "unkown key", 175 }),
	(null_input::key_name{ "unkown key", 176 }),
	(null_input::key_name{ "unkown key", 177 }),
	(null_input::key_name{ "unkown key", 178 }),
	(null_input::key_name{ "unkown key", 179 }),
	(null_input::key_name{ "unkown key", 180 }),
	(null_input::key_name{ "unkown key", 181 }),
	(null_input::key_name{ "unkown key", 182 }),
	(null_input::key_name{ "unkown key", 183 }),
	(null_input::key_name{ "unkown key", 184 }),
	(null_input::key_name{ "unkown key", 185 }),
	(null_input::key_name{ "unkown key", 186 }),*/
	(null_input::key_name{ ";", "æ", ":", 186 }),
	(null_input::key_name{ "=", "", "+", 187 }),
	(null_input::key_name{ ",", "á", "<", 188 }),
	(null_input::key_name{ "-", "", "_", "_", 189 }),
	(null_input::key_name{ ".", "þ", ">", 190 }),
	(null_input::key_name{ "/", ".", "?", ",", 191 }),
	(null_input::key_name{ "`", "¸", "~", 192 }),
	/*(null_input::key_name{ "unkown key", 192 }),
	(null_input::key_name{ "unkown key", 193 }),
	(null_input::key_name{ "unkown key", 194 }),
	(null_input::key_name{ "unkown key", 195 }),
	(null_input::key_name{ "unkown key", 196 }),
	(null_input::key_name{ "unkown key", 197 }),
	(null_input::key_name{ "unkown key", 198 }),
	(null_input::key_name{ "unkown key", 199 }),
	(null_input::key_name{ "unkown key", 200 }),
	(null_input::key_name{ "unkown key", 201 }),
	(null_input::key_name{ "unkown key", 202 }),
	(null_input::key_name{ "unkown key", 203 }),
	(null_input::key_name{ "unkown key", 204 }),
	(null_input::key_name{ "unkown key", 205 }),
	(null_input::key_name{ "unkown key", 206 }),
	(null_input::key_name{ "unkown key", 207 }),
	(null_input::key_name{ "unkown key", 208 }),
	(null_input::key_name{ "unkown key", 209 }),
	(null_input::key_name{ "unkown key", 210 }),
	(null_input::key_name{ "unkown key", 211 }),
	(null_input::key_name{ "unkown key", 212 }),
	(null_input::key_name{ "unkown key", 213 }),
	(null_input::key_name{ "unkown key", 214 }),
	(null_input::key_name{ "unkown key", 215 }),
	(null_input::key_name{ "unkown key", 216 }),
	(null_input::key_name{ "unkown key", 217 }),
	(null_input::key_name{ "unkown key", 218 }),*/
	(null_input::key_name{ "[", "õ", "{", 219 }),
	(null_input::key_name{ "\\", "", "|", "/", 220 }),
	(null_input::key_name{ "]", "ú", "}", 221 }),
	(null_input::key_name{ "\'", "ý", "\"", 222 })
};

namespace null_input {
	bool key_name::for_input(int id) {
		return (id >= 48 && id <= 90) || (id >= 186 && id <= 192) || (id >= 219 && id <= 222) || (id >= 96 && id <= 111) || id == 32;
	}

	int key_name::get_array_id(std::string name) {
		std::vector<input_key>::iterator ret = std::find_if(key_list.begin(), key_list.end(), [=](input_key& a) { return a.data.us == name || a.data.rus == name || a.data.us_shift == name || a.data.rus_shift == name; });

		if (ret == key_list.end()) return 0;
		else return std::distance(key_list.begin(), ret);
	}

	int key_name::get_array_id(int id) {
		std::vector<input_key>::iterator ret = std::find_if(key_list.begin(), key_list.end(), [=](input_key& a) { return a.data.id == id; });

		if (ret == key_list.end()) return 0;
		else return std::distance(key_list.begin(), ret);
	}

	std::string key_name::get_name(int id, bool language_and_shift) {
		int language = GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL)) == (HKL)67699721 ? 0 : 1;

		key_name key = key_list[id].data;
		bool caps = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

		if (language_and_shift) {
			if (key.us == "space") return " ";

			if (language == 0) {
				if (key.us.size() == 1 && key.us.back() >= 'a' && key.us.back() <= 'z' && (get_key("shift")->down() || caps)) {
					std::string result = key.us;
					if (get_key("shift")->down() && caps) return result;
					result.back() = key.us.back() + ('A' - 'a');
					return result;
				}
				if (get_key("shift")->down() && key.us_shift != "") return key.us_shift;

				return key.us;
			} else {
				if (key.rus.size() == 1 && key.rus.back() >= 'à' && key.rus.back() <= 'ÿ' && (get_key("shift")->down() || caps)) {
					std::string result = key.rus;
					if (get_key("shift")->down() && caps) return result;
					result.back() = key.rus.back() + ('À' - 'à');
					return result;
				}
				if (get_key("shift")->down() && (key.rus_shift != "" || key.us_shift != "")) return key.rus_shift != "" ? key.rus_shift : key.us_shift != "" ? key.us_shift : "";

				return key.rus != "" ? key.rus : key.us;
			}
		}

		return key.us;
	}


	void input_key::update_duration() {
		down_duration = down() ? (down_duration < 0.0f ? 0.0f : down_duration + null_gui::deeps::delta_time) : -1.0f;
	}

	bind_key::bind_key(std::string _name, int key_id, bool* var, bind_type _type) {
		key = get_key(key_id);
		name = _name;
		type = _type;
		text = { "enable", "disable" };
		set_callback({ [var]() { *var = true; }, [var]() { *var = false; }, [var]() { *var = !*var; } });
	}

	bind_key::bind_key(std::string _name, std::string key_name, bool* var, bind_type _type) {
		key = get_key(key_name);
		name = _name;
		type = _type;
		text = { "enable", "disable" };
		set_callback({ [var]() { *var = true; }, [var]() { *var = false; }, [var]() {*var = !*var; } });
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
				if (bind->key->down()) bind->callbacks[0]();
				else bind->callbacks[1]();
			} break;
			case bind_type::hold_off: {
				if (!bind->key->down()) bind->callbacks[0]();
				else bind->callbacks[1]();
			} break;
			case bind_type::toggle: if (bind->key->clicked()) bind->callbacks[2](); break;
			case bind_type::always: bind->callbacks[0](); break;
			}
		}
	}

	void update_keys_state() {
		static vec2 mouse_click_pos;
		for (input_key& key : key_list) {
			key.state_clicked = key.down() && key.down_duration < 0.0f;
			key.state_pressed = !key.down() && key.down_duration >= 0.f;

			if (key.callback && (key.callback_state == null_input::key_state::clicked && key.state_clicked) || (key.callback_state == null_input::key_state::pressed && key.state_pressed))
				key.callback();

			key.update_duration();
			key.state_double_clicked = false;
			if (key.state_clicked) {
				if ((float)(null_gui::deeps::real_time - key.clicked_time) < null_gui::gui_settings::double_click_time) {
					vec2 delta = mouse_pos - mouse_click_pos;
					if (delta.length_sqr() < pow(null_gui::gui_settings::double_click_max_dist, 2)) {
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