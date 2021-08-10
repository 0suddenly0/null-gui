#define NOMINMAX
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <iostream>
#include <functional>
#include <locale> 
#include <codecvt>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <regex>
#include <tchar.h>
#include <chrono>

#include "../../null-gui/utils/utils.h"
#include "../../null-gui/null/gui/gui.h"
#include "../../null-gui/null/render/render.h"
#include "../../null-gui/null/render/directx9/directx9.h"

int fps() {
	static int count = 0;
	static auto last = std::chrono::high_resolution_clock::now();
	auto now = std::chrono::high_resolution_clock::now();
	static int fps = 0;

	count++;

	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count() > 1000) {
		fps = count;
		count = 0;
		last = now;
	}

	return fps;
}

#define VAR_TO_STRING(VAR) #VAR

static LPDIRECT3D9 d3d = NULL;
static LPDIRECT3DDEVICE9 d3d_device = NULL;
static D3DPRESENT_PARAMETERS d3dp = {};
static HWND window;

void reset_device_d3d() {
	null::render::directx9::invalidate_device_objects();
	HRESULT hr = d3d_device->Reset(&d3dp);
	if (hr == D3DERR_INVALIDCALL)
		assert(0);
	null::render::directx9::create_device_objects();
}

void cleanup_device_d3d() {
	if (d3d_device) { d3d_device->Release(); d3d_device = NULL; }
	if (d3d) { d3d->Release(); d3d = NULL; }
}

BOOL create_device_d3d(HWND hwnd) {
	if ((d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return FALSE;

	ZeroMemory(&d3dp, sizeof(d3dp));
	d3dp.Windowed = TRUE;
	d3dp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dp.EnableAutoDepthStencil = TRUE;
	d3dp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dp, &d3d_device) < 0)
		return FALSE;
	return TRUE;
}


LRESULT CALLBACK wnd_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {
	null::input::wnd_proc(message, w_param, l_param);

	switch (message) {
	case WM_SIZE:
		if (d3d_device != NULL && w_param != SIZE_MINIMIZED) {
			d3dp.BackBufferWidth = LOWORD(l_param);
			d3dp.BackBufferHeight = HIWORD(l_param);
			reset_device_d3d();
		}
		return false;
	}

	return DefWindowProcA(hwnd, message, w_param, l_param);
}

ATOM my_register_class(HINSTANCE instance, LPCSTR class_name) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wnd_proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(instance, MAKEINTRESOURCE(107));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(109);
	wcex.lpszClassName = class_name;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(108));

	return RegisterClassEx(&wcex);
}

BOOL init_window(HINSTANCE instance, LPCSTR class_name, LPCSTR title) {
	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);

	window = CreateWindowA(class_name, title, WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, instance, NULL);

	if (!window) return FALSE;
	return TRUE;
}

int main(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, int cmd_show) {
	ShowWindow(GetConsoleWindow(), SW_SHOWDEFAULT);
	setlocale(LC_ALL, "Russian");

	LPCSTR wnd_name = "null-gui | directx9";
	if (!my_register_class(instance, wnd_name)) return 0;
	if (!init_window(instance, wnd_name, wnd_name)) return 0;

	if (!create_device_d3d(window)) {
		cleanup_device_d3d();
		UnregisterClass(wnd_name, instance);
		return 0;
	}

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	static int test_int = 0;

	null::input::get_key(null::input::key_id::insert)->set_callback(null::input::key_state::double_clicked, []() {
		printf("test hotkey callback\n");
		test_int = 100;
		});

	null::render::directx9::initialize(d3d_device);
	null::render::initialize();
	null::gui::initialize(window);

	null::font::font* font = null::font::load_font("C:\\Windows\\fonts\\Tahoma.ttf", 13.0f);

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		null::render::directx9::begin_frame();
		null::render::begin_frame(window);
		null::gui::begin_frame();

		null::render::draw_rect_filled(vec2(25, 25), vec2(75, 75), color(255, 255, 255, 100));
		null::render::draw_rect_filled_multicolor_vertical(vec2(100, 200), vec2(200, 500), null::gui::style::main_color, null::gui::style::button_bg, null::gui::style::button_rounding);
		null::render::draw_rect_filled_multicolor(vec2(300, 100), vec2(600, 200), { null::gui::style::main_color, null::gui::style::button_bg }, { null::gui::style::main_color, null::gui::style::button_bg }, null::gui::style::button_rounding);
		null::render::draw_rect_filled_multicolor(vec2(300, 300), vec2(600, 400), { null::gui::style::main_color, null::gui::style::button_bg }, { null::gui::style::button_bg_active, null::gui::style::button_bg_hovered }, null::gui::style::button_rounding);
		null::render::draw_rect_filled_multicolor(vec2(300, 500), vec2(600, 600), { null::gui::style::main_color, null::gui::style::button_bg }, { null::gui::style::button_bg_active, null::gui::style::button_bg_hovered }, 0.f);

		null::render::draw_blur(vec2(0, 0), vec2(100, 100), 1.f, 1.f, 40.f);

		static bool test_bool_bind;
		static null::input::bind_key bind("test_bind", null::input::key_id::seven, &test_bool_bind, null::input::bind_type::hold_on);

		null::input::create_bind(true, &bind);

		static bool settings_window = true;
		static bool debug_window = true;

		static float test_float = 0.f;
		static int test_int = 0;
		static color test_color(255, 255, 255, 255);
		static float size_window = 150.f;
		static std::vector<bool> test_bools = { false, false, false, false };
		static std::string test_string = "https://github.com/0suddenly0/null-gui";

		if (test_bool_bind)
			null::render::draw_text("test bind", vec2(10, 40), color(255, 255, 255));

		if (null::gui::begin_window("debug window [ window with debug information ]", vec2(290, 20), vec2(300, 300), { null::gui::window_flags::auto_size }, &debug_window)) {
			null::gui::text(utils::format("active item name - '%s'", null::gui::detail::active_name.c_str()));
			null::gui::text(utils::format("active window name - '%s'", null::gui::detail::active_window_name.c_str()));
			if (null::gui::detail::hovered_window) null::gui::text(utils::format("hovered window name - '%s'", null::gui::detail::hovered_window->name.c_str()));
			null::gui::end_window();
		}

		if (null::gui::begin_window("test size", vec2(0, 0), vec2(0, 0), { null::gui::window_flags::set_size, null::gui::window_flags::auto_size })) {
			null::gui::combo("combo", &test_int, { "nullptr", "null-gui", "https://github.com/0suddenly0/null-gui", "1", "2", "3", "4", "suddenly" });
			null::gui::multi_combo("multi_combo", &test_bools, { "head", "body", "legs", "arms" });
			null::gui::color_picker("color", &null::gui::style::main_color);
			null::gui::end_window();
		}

		if (null::gui::begin_window("settings window", vec2(600, 20), vec2(500, 0), { null::gui::window_flags::set_size, null::gui::window_flags::auto_size }, &settings_window)) {
			null::gui::begin_columns(2); {
				null::gui::begin_group("floats", vec2(0.f, 200.f)); {
					null::gui::slider_float("window_title_size", &null::gui::style::window_title_size, 0, 100, "%.0f", 1);
					null::gui::slider_float("window_title_line_size", &null::gui::style::window_title_line_size, 0, 10, "%.0f", 1);
					null::gui::slider_float("item_spacing", &null::gui::style::item_spacing, 1, 20, "%.0f", 1);
					null::gui::slider_float("text_spacing", &null::gui::style::text_spacing, 1, 20, "%.0f", 1);
					null::gui::slider_float("checkbox_size", &null::gui::style::checkbox_size, 1, 50, "%.0f", 1);
					null::gui::slider_float("check_mark_size", &null::gui::style::check_mark_size, 1, 10, "%.0f", 1);
					null::gui::slider_float("checkbox_body_offset", &null::gui::style::checkbox_body_offset, 0, 50, "%.0f", 1);
					null::gui::slider_float("combo_size", &null::gui::style::combo_size, 1, 50, "%.0f", 1);
					null::gui::slider_float("combo_arrow_size", &null::gui::style::combo_arrow_size, 0.f, 1.f, "%.2f", 0.1f);
					null::gui::slider_float("combo_window_padding", &null::gui::style::combo_window_padding, 0.f, 20.f, "%.0f", 1);
					null::gui::slider_int("max_auto_size_combo", &null::gui::style::max_auto_size_combo, 1, 10);
					null::gui::slider_float("slider_size", &null::gui::style::slider_size, 3, 20, "%.0f", 1);
					null::gui::slider_float("selectable_offset", &null::gui::style::selectable_offset, 0, 20, "%.0f", 1);
					null::gui::slider_float("selectable_active_offset", &null::gui::style::selectable_active_offset, 3, 20, "%.0f", 1);
					null::gui::slider_float("color_picker_size", &null::gui::style::color_picker_size, 1, 200, "%.0f", 1);
					null::gui::slider_float("color_picker_thickness", &null::gui::style::color_picker_thickness, 3, 20, "%.0f", 1);
					null::gui::slider_float("scrollbar_thickness", &null::gui::style::scrollbar_thickness, 1, 10, "%.0f", 1);
					null::gui::slider_float("text_input_line_size", &null::gui::style::text_input_line_size, 1, 10, "%.0f", 1);
					null::gui::slider_float("new_line_size", &null::gui::style::new_line_size, 0, 30, "%.0f", 1);

					null::gui::text("roundings");
					null::gui::slider_float("window_rounding", &null::gui::style::window_rounding, 0, 20, "%.0f", 1);
					null::gui::slider_float("window_title_rounding", &null::gui::style::window_title_rounding, 0, 20, "%.0f", 1);
					null::gui::slider_float("slider_rounding", &null::gui::style::slider_rounding, 0, 20, "%.0f", 1);
					null::gui::slider_float("checkbox_rounding", &null::gui::style::checkbox_rounding, 0, 20, "%.0f", 1);
					null::gui::slider_float("button_rounding", &null::gui::style::button_rounding, 0, 20, "%.0f", 1);
					null::gui::slider_float("combo_rounding", &null::gui::style::combo_rounding, 0, 20, "%.0f", 1);
					null::gui::slider_float("scrollbar_rounding", &null::gui::style::scrollbar_rounding, 0, 20, "%.0f", 1);
					null::gui::slider_float("color_picker_rounding", &null::gui::style::color_picker_rounding, 0, 20, "%.0f", 1);
					null::gui::slider_float("key_bind_rounding", &null::gui::style::key_bind_rounding, 0, 20, "%.0f", 1);
					null::gui::slider_float("text_input_roundig", &null::gui::style::text_input_rounding, 0, 20, "%.0f", 1);
				} null::gui::end_group();
				null::gui::begin_group("bools", vec2(0.f, 200.f)); {
					null::gui::checkbox("items_size_full_window", &null::gui::style::items_size_full_window);
					null::gui::checkbox("checkbox_hovered_with_text", &null::gui::style::checkbox_hovered_with_text);
					null::gui::checkbox("clamp_window_on_screen", &null::gui::style::clamp_window_on_screen);
					null::gui::checkbox("move_window_on_title_bar", &null::gui::style::move_window_on_title_bar);
					null::gui::checkbox("show_scrollbar_background", &null::gui::style::show_scrollbar_background);
				} null::gui::end_group();
				null::gui::next_column();
				null::gui::begin_group("vectors", vec2(0.f, 200.f)); {
					null::gui::text("button_padding");
					null::gui::begin_columns(2); {
						null::gui::slider_float("x##button_padding", &null::gui::style::button_padding.x, 0, 50, "%.0f", 1);
						null::gui::next_column();
						null::gui::slider_float("y##button_padding", &null::gui::style::button_padding.y, 0, 50, "%.0f", 1);
					} null::gui::end_columns();

					null::gui::text("window_padding");
					null::gui::begin_columns(2); {
						null::gui::slider_float("x##window_padding", &null::gui::style::window_padding.x, 0, 50, "%.0f", 1);
						null::gui::next_column();
						null::gui::slider_float("y##window_padding", &null::gui::style::window_padding.y, 0, 50, "%.0f", 1);
					} null::gui::end_columns();

					null::gui::text("scrollbar_padding");
					null::gui::begin_columns(2); {
						null::gui::slider_float("x##scrollbar_padding", &null::gui::style::scrollbar_padding.x, 0, 10, "%.0f", 1);
						null::gui::next_column();
						null::gui::slider_float("y##scrollbar_padding", &null::gui::style::scrollbar_padding.y, 0, 10, "%.0f", 1);
					} null::gui::end_columns();

					null::gui::text("text_input_padding");
					null::gui::begin_columns(2); {
						null::gui::slider_float("x##text_input_padding", &null::gui::style::text_input_padding.x, 0, 50, "%.0f", 1);
						null::gui::next_column();
						null::gui::slider_float("y##text_input_padding", &null::gui::style::text_input_padding.y, 0, 50, "%.0f", 1);
					} null::gui::end_columns();
				} null::gui::end_group();
				null::gui::begin_group("colors", vec2(0.f, 200.f)); {
					null::gui::color_picker("main_color", &null::gui::style::main_color);
					null::gui::color_picker("window_bg", &null::gui::style::window_bg);
					null::gui::color_picker("window_title_bg", &null::gui::style::window_title_bg);
					null::gui::color_picker("group_bg", &null::gui::style::group_bg);
					null::gui::color_picker("button_bg", &null::gui::style::button_bg);
					null::gui::color_picker("button_bg_hovered", &null::gui::style::button_bg_hovered);
					null::gui::color_picker("button_bg_active", &null::gui::style::button_bg_active);
					null::gui::color_picker("popup_bg", &null::gui::style::popup_bg);
					null::gui::color_picker("text", &null::gui::style::text);
					null::gui::color_picker("text_hovered", &null::gui::style::text_hovered);
					null::gui::color_picker("select_text", &null::gui::style::select_text);
				} null::gui::end_group();
			} null::gui::end_columns();
			null::gui::end_window();
		}

		if (null::gui::begin_window("window", vec2(290, 330), vec2(200.f, 0.f), { null::gui::window_flags::set_size, null::gui::window_flags::auto_size }, nullptr)) {
			null::gui::text(utils::format("%d", test_int));
			null::gui::same_line();
			null::gui::detail::push_var(&null::gui::style::items_size_full_window, false); {
				if (null::gui::clickable_text("-")) test_int--;
				null::gui::same_line();
				if (null::gui::clickable_text("+")) test_int++;
			} null::gui::detail::pop_var();

			null::gui::detail::push_var(&null::gui::style::items_size_full_window, false); {
				null::gui::slider_int("test slider", &test_int, 0, 500);
			} null::gui::detail::pop_var();

			null::gui::checkbox("show debug window", &debug_window);
			null::gui::tooltip("test tooltip");
			null::gui::text(utils::format("%.6f", test_float));
			null::gui::text_input("text input", &test_string);
			null::gui::key_bind("test key bind", &bind);
			null::gui::new_line();
			null::gui::checkbox("show settings window", &settings_window);
			null::gui::text(utils::format("fps : %d", fps()));
			null::gui::text(std::to_string(null::input::last_press_key) + " " + std::to_string(null::input::last_press_key));
			null::gui::end_window();
		}

		null::render::end_frame();

		d3d_device->SetRenderState(D3DRS_ZENABLE, FALSE);
		d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		d3d_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(20, 20, 20, 100);
		d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		if (d3d_device->BeginScene() >= 0) {
			null::render::setup_draw_data();
			null::render::directx9::render_draw_data();
			d3d_device->EndScene();
		}
		HRESULT result = d3d_device->Present(NULL, NULL, NULL, NULL);

		if (result == D3DERR_DEVICELOST && d3d_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			reset_device_d3d();
	}

	null::render::directx9::shutdown();
	null::render::shutdown();
	null::gui::shutdown();

	cleanup_device_d3d();

	DestroyWindow(window);
	UnregisterClass(wnd_name, instance);

	return 0;
}
