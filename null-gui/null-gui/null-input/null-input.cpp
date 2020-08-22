#include "null-input.h"
#include "../null-gui.h"
#include <windowsx.h>

bool process_mouse_message(UINT u_msg, WPARAM w_param, LPARAM l_param) {
    int key = 0;
    bool state = false;
    switch (u_msg) {
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
        state = u_msg == WM_MBUTTONUP ? false : true;
        key = 2;
        break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        state = u_msg == WM_RBUTTONUP ? false : true;
        key = 1;
        break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        state = u_msg == WM_LBUTTONUP ? false : true;
        key = 0;
        break;
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
        state = u_msg == WM_XBUTTONUP ? false : true;
        key = (HIWORD(w_param) == XBUTTON1 ? 3 : 4);
        break;
    default:
        return false;
    }

    null_input::vars::last_click_pos = vec2((signed short)(l_param), (signed short)(l_param >> 16));
    null_input::vars::mouse_buttons[key].state_down = state;
    return true;
}

bool process_keybd_message(UINT u_msg, WPARAM w_param, LPARAM l_param) {
    int key = 0;
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

    null_input::vars::keys[int(key)].state_down = state;
    return true;
}

namespace null_input {
    bool key_state::down() {
        return state_down;
    }

    bool key_state::pressed() {
        return state_pressed;
    }

    bool key_state::clicked() {
        return state_clicked;
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
        for (key_state& key : vars::mouse_buttons) {
            key.state_clicked = key.down() && key.down_duration < 0.0f;
            key.state_pressed = !key.down() && key.down_duration >= 0.f;
            key.update_duration();
        }
    }

    bool last_click_in_region(vec2 min, vec2 max) {
        return last_click_pos() < min && last_click_pos() > max;
    }

    bool mouse_in_region(vec2 min, vec2 max) {
        return mouse_pos() < min && mouse_pos() > max;
    }

    key_state get_mouse_key_state(int key) {
        return vars::mouse_buttons[key];
    }
}