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
#include "../../null-gui/null-gui/null-gui.h"
#include "../../null-gui/null-render/null-render.h"
#include "../../null-gui/null-render/directx9/null-render-dx9.h"

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
	null_render::directx9::invalidate_device_objects();
	HRESULT hr = d3d_device->Reset(&d3dp);
	if (hr == D3DERR_INVALIDCALL)
		assert(0);
	null_render::directx9::create_device_objects();
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
	null_input::null_wnd_proc(message, w_param, l_param);

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

	null_input::get_key(null_input::key_id::insert)->set_callback(null_input::key_state::double_clicked, []() {
		printf("test hotkey callback\n");
		test_int = 100;
		});

	null_render::initialize();
	null_render::directx9::init(d3d_device);

	null_gui::pre_begin_gui(window);
	null_font::font* font = null_font::load_font("C:\\Windows\\fonts\\Tahoma.ttf", 13.0f);

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		null_render::directx9::begin_frame();
		null_render::begin_render(window);
		null_gui::begin_gui();

		null_render::draw_rect_filled(vec2(25, 25), vec2(75, 75), color(255, 255, 255, 100));
		null_render::draw_rect_filled_multicolor_vertical(vec2(100, 200), vec2(200, 500), null_gui::gui_settings::main_color, null_gui::gui_settings::button_bg, null_gui::gui_settings::button_rounding);
		null_render::draw_rect_filled_multicolor(vec2(300, 100), vec2(600, 200), { null_gui::gui_settings::main_color, null_gui::gui_settings::button_bg }, { null_gui::gui_settings::main_color, null_gui::gui_settings::button_bg }, null_gui::gui_settings::button_rounding);
		null_render::draw_rect_filled_multicolor(vec2(300, 300), vec2(600, 400), { null_gui::gui_settings::main_color, null_gui::gui_settings::button_bg }, { null_gui::gui_settings::button_bg_active, null_gui::gui_settings::button_bg_hovered }, null_gui::gui_settings::button_rounding);
		null_render::draw_rect_filled_multicolor(vec2(300, 500), vec2(600, 600), { null_gui::gui_settings::main_color, null_gui::gui_settings::button_bg }, { null_gui::gui_settings::button_bg_active, null_gui::gui_settings::button_bg_hovered }, 0.f);

		null_render::background_draw_list.draw_blur(vec2(0, 0), vec2(100, 100), 1.f, 1.f, 10.f);

		static bool test_bool_bind;
		static null_input::bind_key bind("test_bind", "7", &test_bool_bind, null_input::bind_type::hold_on);

		null_input::create_bind(true, &bind);

		static bool settings_window = false;
		static bool debug_window = true;

		static float test_float = 0.f;
		static int test_int = 0;
		static color test_color(255, 255, 255, 255);
		static float size_window = 150.f;
		static std::vector<bool> test_bools = { false, false, false, false };
		static std::string test_string = "https://github.com/0suddenly0/null-gui";

		if (test_bool_bind)
			null_render::draw_text("test bind", vec2(10, 40), color(255, 255, 255));

		if (null_gui::begin_window("debug window [ window with debug information ]", vec2(290, 20), vec2(300, 300), { null_gui::window_flags::auto_size }, &debug_window)) {
			null_gui::text(utils::format("active item name - '%s'", null_gui::deeps::active_name.c_str()));
			null_gui::text(utils::format("active window name - '%s'", null_gui::deeps::active_window_name.c_str()));
			if (null_gui::deeps::hovered_window) null_gui::text(utils::format("hovered window name - '%s'", null_gui::deeps::hovered_window->name.c_str()));
			null_gui::end_window();
		}

		if (null_gui::begin_window("test size", vec2(0, 0), vec2(0, 0), { null_gui::window_flags::set_size, null_gui::window_flags::auto_size })) {
			null_gui::combo("combo", &test_int, { "nullptr", "null-gui", "https://github.com/0suddenly0/null-gui", "1", "2", "3", "4", "suddenly" });
			null_gui::multicombo("multicombo", &test_bools, { "head", "body", "legs", "arms" });
			null_gui::colorpicker("color", &null_gui::gui_settings::main_color);
			null_gui::end_window();
		}

		if (null_gui::begin_window("settings window", vec2(600, 20), vec2(500, 0), { null_gui::window_flags::set_size, null_gui::window_flags::auto_size }, &settings_window)) {
			null_gui::begin_columns(2); {
				null_gui::begin_group("floats", vec2(0.f, 200.f)); {
					null_gui::slider_float("window_title_size", &null_gui::gui_settings::window_title_size, 0, 100, "%.0f", 1);
					null_gui::slider_float("window_title_line_size", &null_gui::gui_settings::window_title_line_size, 0, 10, "%.0f", 1);
					null_gui::slider_float("item_spacing", &null_gui::gui_settings::item_spacing, 1, 20, "%.0f", 1);
					null_gui::slider_float("text_spacing", &null_gui::gui_settings::text_spacing, 1, 20, "%.0f", 1);
					null_gui::slider_float("checkbox_size", &null_gui::gui_settings::checkbox_size, 1, 50, "%.0f", 1);
					null_gui::slider_float("check_mark_size", &null_gui::gui_settings::check_mark_size, 1, 10, "%.0f", 1);
					null_gui::slider_float("checkbox_body_offset", &null_gui::gui_settings::checkbox_body_offset, 0, 50, "%.0f", 1);
					null_gui::slider_float("combo_size", &null_gui::gui_settings::combo_size, 1, 50, "%.0f", 1);
					null_gui::slider_float("combo_arrow_size", &null_gui::gui_settings::combo_arrow_size, 0.f, 1.f, "%.2f", 0.1f);
					null_gui::slider_float("combo_window_padding", &null_gui::gui_settings::combo_window_padding, 0.f, 20.f, "%.0f", 1);
					null_gui::slider_int("max_auto_size_combo", &null_gui::gui_settings::max_auto_size_combo, 1, 10);
					null_gui::slider_float("slider_size", &null_gui::gui_settings::slider_size, 3, 20, "%.0f", 1);
					null_gui::slider_float("selectable_offset", &null_gui::gui_settings::selectable_offset, 0, 20, "%.0f", 1);
					null_gui::slider_float("selectable_active_offset", &null_gui::gui_settings::selectable_active_offset, 3, 20, "%.0f", 1);
					null_gui::slider_float("colorpicker_size", &null_gui::gui_settings::colorpicker_size, 1, 200, "%.0f", 1);
					null_gui::slider_float("colorpicker_thickness", &null_gui::gui_settings::colorpicker_thickness, 3, 20, "%.0f", 1);
					null_gui::slider_float("scrollbar_thickness", &null_gui::gui_settings::scrollbar_thickness, 1, 10, "%.0f", 1);
					null_gui::slider_float("text_input_line_size", &null_gui::gui_settings::text_input_line_size, 1, 10, "%.0f", 1);
					null_gui::slider_float("new_line_size", &null_gui::gui_settings::new_line_size, 0, 30, "%.0f", 1);

					null_gui::text("roundings");
					null_gui::slider_float("window_rounding", &null_gui::gui_settings::window_rounding, 0, 20, "%.0f", 1);
					null_gui::slider_float("window_title_rounding", &null_gui::gui_settings::window_title_rounding, 0, 20, "%.0f", 1);
					null_gui::slider_float("slider_rounding", &null_gui::gui_settings::slider_rounding, 0, 20, "%.0f", 1);
					null_gui::slider_float("checkbox_rounding", &null_gui::gui_settings::checkbox_rounding, 0, 20, "%.0f", 1);
					null_gui::slider_float("button_rounding", &null_gui::gui_settings::button_rounding, 0, 20, "%.0f", 1);
					null_gui::slider_float("combo_rounding", &null_gui::gui_settings::combo_rounding, 0, 20, "%.0f", 1);
					null_gui::slider_float("scrollbar_rounding", &null_gui::gui_settings::scrollbar_rounding, 0, 20, "%.0f", 1);
					null_gui::slider_float("colorpicker_rounding", &null_gui::gui_settings::colorpicker_rounding, 0, 20, "%.0f", 1);
					null_gui::slider_float("key_bind_rounding", &null_gui::gui_settings::key_bind_rounding, 0, 20, "%.0f", 1);
					null_gui::slider_float("text_input_roundig", &null_gui::gui_settings::text_input_rounding, 0, 20, "%.0f", 1);
				} null_gui::end_group();
				null_gui::begin_group("bools", vec2(0.f, 200.f)); {
					null_gui::checkbox("items_size_full_window", &null_gui::gui_settings::items_size_full_window);
					null_gui::checkbox("checkbox_hovered_with_text", &null_gui::gui_settings::checkbox_hovered_with_text);
					null_gui::checkbox("clamp_window_on_screen", &null_gui::gui_settings::clamp_window_on_screen);
					null_gui::checkbox("move_window_on_title_bar", &null_gui::gui_settings::move_window_on_title_bar);
					null_gui::checkbox("show_scrollbar_background", &null_gui::gui_settings::show_scrollbar_background);
				} null_gui::end_group();
				null_gui::next_column();
				null_gui::begin_group("vectors", vec2(0.f, 200.f)); {
					null_gui::text("button_padding");
					null_gui::begin_columns(2); {
						null_gui::slider_float("x##button_padding", &null_gui::gui_settings::button_padding.x, 0, 50, "%.0f", 1);
						null_gui::next_column();
						null_gui::slider_float("y##button_padding", &null_gui::gui_settings::button_padding.y, 0, 50, "%.0f", 1);
					} null_gui::end_columns();

					null_gui::text("window_padding");
					null_gui::begin_columns(2); {
						null_gui::slider_float("x##window_padding", &null_gui::gui_settings::window_padding.x, 0, 50, "%.0f", 1);
						null_gui::next_column();
						null_gui::slider_float("y##window_padding", &null_gui::gui_settings::window_padding.y, 0, 50, "%.0f", 1);
					} null_gui::end_columns();

					null_gui::text("scrollbar_padding");
					null_gui::begin_columns(2); {
						null_gui::slider_float("x##scrollbar_padding", &null_gui::gui_settings::scrollbar_padding.x, 0, 10, "%.0f", 1);
						null_gui::next_column();
						null_gui::slider_float("y##scrollbar_padding", &null_gui::gui_settings::scrollbar_padding.y, 0, 10, "%.0f", 1);
					} null_gui::end_columns();

					null_gui::text("text_input_padding");
					null_gui::begin_columns(2); {
						null_gui::slider_float("x##text_input_padding", &null_gui::gui_settings::text_input_padding.x, 0, 50, "%.0f", 1);
						null_gui::next_column();
						null_gui::slider_float("y##text_input_padding", &null_gui::gui_settings::text_input_padding.y, 0, 50, "%.0f", 1);
					} null_gui::end_columns();
				} null_gui::end_group();
				null_gui::begin_group("colors", vec2(0.f, 200.f)); {
					null_gui::colorpicker("main_color", &null_gui::gui_settings::main_color);
					null_gui::colorpicker("window_bg", &null_gui::gui_settings::window_bg);
					null_gui::colorpicker("window_title_bg", &null_gui::gui_settings::window_title_bg);
					null_gui::colorpicker("group_bg", &null_gui::gui_settings::group_bg);
					null_gui::colorpicker("button_bg", &null_gui::gui_settings::button_bg);
					null_gui::colorpicker("button_bg_hovered", &null_gui::gui_settings::button_bg_hovered);
					null_gui::colorpicker("button_bg_active", &null_gui::gui_settings::button_bg_active);
					null_gui::colorpicker("popup_bg", &null_gui::gui_settings::popup_bg);
					null_gui::colorpicker("text", &null_gui::gui_settings::text);
					null_gui::colorpicker("text_hovered", &null_gui::gui_settings::text_hovered);
					null_gui::colorpicker("select_text", &null_gui::gui_settings::select_text);
				} null_gui::end_group();
			} null_gui::end_columns();
			null_gui::end_window();
		}

		if (null_gui::begin_window("window", vec2(290, 330), vec2(200.f, 0.f), { null_gui::window_flags::set_size, null_gui::window_flags::auto_size }, nullptr)) {
			null_gui::text(utils::format("%d", test_int));
			null_gui::same_line();
			null_gui::deeps::push_var(&null_gui::gui_settings::items_size_full_window, false); {
				if (null_gui::clickable_text("-")) {
					test_int--;
				}
				null_gui::same_line();
				if (null_gui::clickable_text("+")) {
					test_int++;
				}
			} null_gui::deeps::pop_var();

			null_gui::deeps::push_var(&null_gui::gui_settings::items_size_full_window, false); {
				null_gui::slider_int("test slider", &test_int, 0, 500);
			} null_gui::deeps::pop_var();

			null_gui::checkbox("show debug window", &debug_window);
			null_gui::tooltip("test tooltip");
			null_gui::text(utils::format("%.6f", test_float));
			null_gui::text_input("text input", &test_string);
			null_gui::key_bind("test key bind", &bind);
			null_gui::new_line();
			null_gui::checkbox("show settings window", &settings_window);
			null_gui::text(utils::format("fps : %d", fps()));
			null_gui::text(std::to_string(null_input::last_press_key) + " " + std::to_string(null_input::last_press_key));
			null_gui::end_window();
		}

		null_render::end_render();

		d3d_device->SetRenderState(D3DRS_ZENABLE, FALSE);
		d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		d3d_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(20, 20, 20, 100);
		d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		if (d3d_device->BeginScene() >= 0)
		{
			null_render::render();
			null_render::directx9::render_draw_data();
			d3d_device->EndScene();
		}
		HRESULT result = d3d_device->Present(NULL, NULL, NULL, NULL);

		if (result == D3DERR_DEVICELOST && d3d_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			reset_device_d3d();
		}
	}
	cleanup_device_d3d();

	DestroyWindow(window);
	UnregisterClass(wnd_name, instance);

	return 0;
}
