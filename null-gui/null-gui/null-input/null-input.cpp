#define _CRT_SECURE_NO_WARNINGS
#include "null-input.h"
#include "../null-gui.h"
#include <windowsx.h>

bool process_mouse_message(UINT u_msg, WPARAM w_param, LPARAM l_param) {
    int id = 0;
    bool state = false;
    switch (u_msg) {
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
        state = u_msg == WM_MBUTTONUP ? false : true;
		id = null_input::key_names::get_id("mouse midle");
        break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        state = u_msg == WM_RBUTTONUP ? false : true;
		id = null_input::key_names::get_id("mouse right");
        break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        state = u_msg == WM_LBUTTONUP ? false : true;
		id = null_input::key_names::get_id("mouse left");
        break;
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
        state = u_msg == WM_XBUTTONUP ? false : true;
		id = (HIWORD(w_param) == XBUTTON1 ? null_input::key_names::get_id("mouse x1") : null_input::key_names::get_id("mouse x2"));
        break;
    default:
        return false;
    }

    null_input::vars::keys[id].state_down = state;
    return true;
}

bool process_keybd_message(UINT u_msg, WPARAM w_param, LPARAM l_param) {
    int id = w_param;
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

    null_input::vars::keys[id].state_down = state;
	if(state) null_gui::deeps::text_input_info::win_poc(id);
	null_input::vars::last_press_key = id;
    return true;
}

class key_name_t {
public:
	key_name_t(std::string _us, int _id) {
		us = _us;
		id = _id;
	}

	key_name_t(std::string _us, std::string _rus, int _id) {
		us = _us;
		rus = _rus;
		id = _id;
	}

	key_name_t(std::string _us, std::string _rus, std::string _us_shift, int _id) {
		us = _us;
		rus = _rus;
		us_shift = _us_shift;
		id = _id;
	}

	key_name_t(std::string _us, std::string _rus, std::string _us_shift, std::string _rus_shift, int _id) {
		us = _us;
		rus = _rus;
		us_shift = _us_shift;
		rus_shift = _rus_shift;
		id = _id;
	}

	std::string us;
	std::string us_shift;
	std::string rus;
	std::string rus_shift;
	int id;
};

std::vector<key_name_t> names = {
	{ "mouse left", 1 },
	{ "mouse right", 2 },
	{ "cancel", 3 },
	{ "mouse midle", 4 },
	{ "mouse x1", 5 },
	{ "mouse x2", 6 },
	{ "backspace", 8 },
	{ "tab", 9 },
	{ "clear", 12  },
	{ "enter", 13 },
	{ "shift", 16 },
	{ "ctrl", 17 },
	{ "alt", 18 },
	{ "pause", 19 },
	{ "caps lock", 20 },
	{ "escape", 27 },
	{ "space", 32 },
	{ "page up", 33 },
	{ "page down", 34 },
	{ "end", 35 },
	{ "home", 36 },
	{ "left", 37 },
	{ "up", 38 },
	{ "right", 39 },
	{ "down", 40 },
	{ "print screen", 44 },
	{ "insert", 45 },
	{ "del", 46 },
	{ "0", "", ")", 48 },
	{ "1", "", "!", 49 },
	{ "2", "", "@", "\"", 50 },
	{ "3", "", "#", "¹",51 },
	{ "4", "", "$", ";",52 },
	{ "5", "", "%", "%",53 },
	{ "6", "", "^", ":",54 },
	{ "7", "", "&", "?",55 },
	{ "8", "", "*", "*",56 },
	{ "9", "", "(", 57 },
	{ "a", "ô", 65 },
	{ "b", "è", 66 },
	{ "c", "ñ", 67 },
	{ "d", "â", 68 },
	{ "e", "ó", 69 },
	{ "f", "à", 70 },
	{ "g", "ï", 71 },
	{ "h", "ð", 72 },
	{ "i", "ø", 73 },
	{ "j", "î", 74 },
	{ "k", "ë", 75 },
	{ "l", "ä", 76 },
	{ "m", "ü", 77 },
	{ "n", "ò", 78 },
	{ "o", "î", 79 },
	{ "p", "ç", 80 },
	{ "q", "é", 81 },
	{ "r", "ê", 82 },
	{ "s", "û", 83 },
	{ "t", "å", 84 },
	{ "u", "ã", 85 },
	{ "v", "ì", 86 },
	{ "w", "ö", 87 },
	{ "x", "÷", 88 },
	{ "y", "í", 89 },
	{ "z", "ÿ", 90 },
	{ "win", 91 },
	{ "app", 93 },
	{ "num 0", 96 },
	{ "num 1", 97 },
	{ "num 2", 98 },
	{ "num 3", 99 },
	{ "num 4", 100 },
	{ "num 5", 101 },
	{ "num 6", 102 },
	{ "num 7", 103 },
	{ "num 8", 104 },
	{ "num 9", 105 },
	{ "num *", 106 },
	{ "num +", 107 },
	{ "num -", 109 },
	{ "num .", 110 },
	{ "num /", 111 },
	{ "f1", 112 },
	{ "f2", 113 },
	{ "f3", 114 },
	{ "f4", 115 },
	{ "f5", 116 },
	{ "f6", 117 },
	{ "f7", 118 },
	{ "f8", 119 },
	{ "f9", 120 },
	{ "f10", 121 },
	{ "f11", 122 },
	{ "f12", 123 },
	{ "f13", 124 },
	{ "f14", 125 },
	{ "f15", 126 },
	{ "f16", 127 },
	{ "f17", 128 },
	{ "f18", 129 },
	{ "f19", 130 },
	{ "f20", 131 },
	{ "f21", 132 },
	{ "f22", 133 },
	{ "f23", 134 },
	{ "f24", 135 },
	{ "num lock", 144 },
	{ "scroll lock", 145 },
	{ "left shift", 160 },
	{ "right shift", 161 },
	{ "left ctrl", 162 },
	{ "right ctrl", 163 },
	{ "left menu", 164 },
	{ "right menu", 165 },
	{ ";", "æ", ":", 186 },
	{ "=", "", "+", 187 },
	{ ",", "á", "<", 188 },
	{ "-", "", "_", "_", 189 },
	{ ".", "þ", ">", 190 },
	{ "/", ".", "?", ",", 191 },
	{ "`", "¸", "~", 192 },
	{ "[", "õ", "{", 219 },
	{ "\\", "", "|", "/", 220 },
	{ "]", "ú", "}", 221 },
	{ "\'", "ý", "\"", 222 },
};

namespace null_input {
	namespace key_names {
		bool for_input(int id) {
			return (id >= 48 && id <= 90) || (id >= 186 && id <= 192) || (id >= 219 && id <= 222) || (id >= 96 && id <= 111) || id == 32;
		}

		int get_id(std::string name) {
			auto ret = std::find_if(names.begin(), names.end(), [&name](key_name_t& a) { return a.us == name || a.rus == name || a.us_shift == name || a.rus_shift == name; });

			if (ret == names.end()) return 0;
			else return (*ret).id;
		}

		std::string get_name(int id, bool language_and_shift) {
			auto ret = std::find_if(names.begin(), names.end(), [&id](key_name_t& a) { return a.id == id; });
			int language = GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL)) == (HKL)67699721 ? 0 : 1;

			if (ret == names.end()) return std::to_string(id);
			else {
				key_name_t key = (*ret);
				bool caps = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

				if (language_and_shift) {
					if (id == get_id("space")) return " ";

					if (language == 0) {
						if (key.us.size() == 1 && key.us.back() >= 'a' && key.us.back() <= 'z' && (get_key_state("shift").down() || caps)) {
							std::string result = key.us;
							if (get_key_state("shift").down() && caps) return result;
							result.back() = key.us.back() + ('A' - 'a');
							return result;
						}
						if (get_key_state("shift").down() && key.us_shift != "") return key.us_shift;

						return key.us;
					} else {
						if (key.rus.size() == 1 && key.rus.back() >= 'à' && key.rus.back() <= 'ÿ' && (get_key_state("shift").down() || caps)) {
							std::string result = key.rus;
							if (get_key_state("shift").down() && caps) return result;
							result.back() = key.rus.back() + ('À' - 'à');
							return result;
						}
						if (get_key_state("shift").down() && key.rus_shift != "") return key.rus_shift;

						return key.rus != "" ? key.rus : key.us;
					}
				}

				return key.us;
			}
		}
	}

    void key_state::update_duration() {
        down_duration = down() ? (down_duration < 0.0f ? 0.0f : down_duration + null_gui::deeps::delta_time) : -1.0f;
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
            vars::mouse_wheel += GET_WHEEL_DELTA_WPARAM(w_param) > 0 ? +1.0f : -1.0f;
            return true;
        case WM_MOUSEMOVE:
            vars::mouse_pos = vec2((signed short)(l_param), (signed short)(l_param >> 16));
            return true;
        }
        return 0;
    }

    void update_keys_state() {
		for (key_state& key : vars::keys) {
			key.state_clicked = key.down() && key.down_duration < 0.0f;
			key.state_pressed = !key.down() && key.down_duration >= 0.f;
			key.update_duration();
			key.state_double_clicked = false;
			if (key.state_clicked) {
				if ((float)(null_gui::deeps::real_time - key.clicked_time) < null_gui::gui_settings::double_click_time) {
					vec2 delta = vars::mouse_pos - vars::mouse_click_pos;
					if (delta.length_sqr() < pow(null_gui::gui_settings::double_click_max_dist, 2)) key.state_double_clicked = true;
					key.clicked_time = -FLT_MAX;
				} else {
					key.clicked_time = null_gui::deeps::real_time;
				}
				vars::mouse_click_pos = vars::mouse_pos;
			}
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