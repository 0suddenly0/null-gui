#include <iostream>
#include <functional>
#include "null-gui/null-gui.h"
#include "utils/utils.h"

null_font::font main_font;

static LPDIRECT3D9 d3d = NULL;
static LPDIRECT3DDEVICE9 d3d_device = NULL;
static D3DPRESENT_PARAMETERS d3dp = {};
static HWND window;

void cleanup_device_d3d() {
	if (d3d_device) {
		d3d_device->Release();
		d3d_device = NULL;
	}

	if (d3d) {
		d3d->Release();
		d3d = NULL;
	}
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

void setup_render_states(std::function< void() > func) {
	IDirect3DStateBlock9* d3d9_state_block = NULL;
	if (d3d_device->CreateStateBlock(D3DSBT_ALL, &d3d9_state_block) < 0)
		return;

	RECT rect;
	GetClientRect(window, &rect);
	vec2 display_size = vec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

	d3d_device->SetPixelShader(NULL);
	d3d_device->SetVertexShader(NULL);
	d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3d_device->SetRenderState(D3DRS_LIGHTING, false);
	d3d_device->SetRenderState(D3DRS_ZENABLE, false);
	d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	d3d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	d3d_device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
	d3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	d3d_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	d3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	D3DVIEWPORT9 vp;
	vp.X = vp.Y = 0;
	vp.Width = (DWORD)display_size.x;
	vp.Height = (DWORD)display_size.y;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	d3d_device->SetViewport(&vp);

	float L = 0.5f, R = display_size.x + 0.5f, T = 0.5f, B = display_size.y + 0.5f;
	D3DMATRIX mat_identity = { { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f } };
	D3DMATRIX mat_projection =
	{
		2.0f / (R - L),   0.0f,         0.0f,  0.0f,
		0.0f,         2.0f / (T - B),   0.0f,  0.0f,
		0.0f,         0.0f,         0.5f,  0.0f,
		(L + R) / (L - R),  (T + B) / (B - T),  0.5f,  1.0f,
	};
	d3d_device->SetTransform(D3DTS_WORLD, &mat_identity);
	d3d_device->SetTransform(D3DTS_VIEW, &mat_identity);
	d3d_device->SetTransform(D3DTS_PROJECTION, &mat_projection);

	func();

	d3d9_state_block->Apply();
	d3d9_state_block->Release();
}


LRESULT CALLBACK wnd_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {
	null_input::null_wnd_proc(message, w_param, l_param);

	return DefWindowProcA(hwnd, message, w_param, l_param);
}

ATOM my_register_class(HINSTANCE instance, LPCTSTR class_name) {
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

BOOL init_window(HINSTANCE instance, LPCTSTR class_name, LPCTSTR title) {
	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);

	window = CreateWindowEx(WS_EX_APPWINDOW, class_name, title, WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800/*screen_rect.left, screen_rect.top*/ /*screen_rect.right, screen_rect.bottom*/, NULL, NULL, instance, NULL);

	if (!window) return FALSE;
	return TRUE;
}

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	LPCTSTR lpzClass = L"nullgui";
	if (!my_register_class(hInstance, lpzClass)) return 0;
	if (!init_window(hInstance, lpzClass, L"null-gui")) return 0;

	if (!create_device_d3d(window)) {
		cleanup_device_d3d();
		UnregisterClass(lpzClass, hInstance);
		return 0;
	}

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	null_gui::pre_begin_gui(window);

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		d3d_device->SetRenderState(D3DRS_ZENABLE, false);
		d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		d3d_device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 100, 100, 0), 1.0f, 0);

		if (d3d_device->BeginScene() >= 0) {
			setup_render_states([=]() {
				null_gui::begin_gui();
				null_render::init(d3d_device);

				static bool debug_window = true;
				static bool settings_window = true;

				null_font::create_font("Tahoma", 14, &main_font, true);

				static null_render::null_draw_list first;
				static null_render::null_draw_list two;

				null_render::render();

				null_render::begin(); {
					static int test_int = 0;
					static color clr(255, 255, 255, 255);
					static float size_window = 150.f;
					static std::vector<bool> test_bools = {false, false, false, false};

					if(null_gui::begin_window("debug window [ window with debug information ]", vec2(100, 100), vec2(300, 300), &debug_window)) {
						null_gui::text(utils::format("active item name - '%s'", null_gui::deeps::active_name.c_str()));
						null_gui::text(utils::format("active window name - '%s'", null_gui::deeps::active_window_name.c_str()));
						null_gui::end_window();
					}

					if(null_gui::begin_window("settings window", vec2(100, 100), vec2(300, 500), &settings_window)) {
						null_gui::text("floats --------------------------------------------------------------------------------");
						null_gui::slider_float("window_title_size", &null_gui::gui_settings::window_title_size, 0, 100, "%.0f");
						null_gui::slider_float("item_spacing", &null_gui::gui_settings::item_spacing, 1, 20, "%.0f");
						null_gui::slider_float("text_spacing", &null_gui::gui_settings::text_spacing, 1, 20, "%.0f");
						null_gui::slider_float("checkbox_size", &null_gui::gui_settings::checkbox_size, 1, 50, "%.0f");
						null_gui::slider_float("combo_size", &null_gui::gui_settings::combo_size, 1, 50, "%.0f");
						null_gui::slider_float("slider_size", &null_gui::gui_settings::slider_size, 3, 20, "%.0f");
						null_gui::slider_float("colorpicker_size", &null_gui::gui_settings::colorpicker_size, 1, 200, "%.0f");
						null_gui::slider_float("colorpicker_thickness", &null_gui::gui_settings::colorpicker_thickness, 3, 20, "%.0f");
						null_gui::text("floats --------------------------------------------------------------------------------");
						null_gui::text("bools ---------------------------------------------------------------------------------");
						null_gui::checkbox("items_size_full_window", &null_gui::gui_settings::items_size_full_window);
						null_gui::checkbox("checkbox_hovered_with_text", &null_gui::gui_settings::checkbox_hovered_with_text);
						null_gui::checkbox("clamp_window_on_screen", &null_gui::gui_settings::clamp_window_on_screen);
						null_gui::checkbox("spacing_checkbox_size", &null_gui::gui_settings::spacing_checkbox_size);
						null_gui::checkbox("move_window_on_title_bar", &null_gui::gui_settings::move_window_on_title_bar);
						null_gui::text("bools ---------------------------------------------------------------------------------");
						null_gui::text("vectors --------------------------------------------------------------------------------");
						null_gui::slider_float("button_padding_x", &null_gui::gui_settings::button_padding.x, 0, 50, "%.0f");
						null_gui::slider_float("button_padding_y", &null_gui::gui_settings::button_padding.y, 0, 50, "%.0f");
						null_gui::slider_float("window_padding_x", &null_gui::gui_settings::window_padding.x, 0, 50, "%.0f");
						null_gui::slider_float("window_padding_y", &null_gui::gui_settings::window_padding.y, 0, 50, "%.0f");
						null_gui::text("vectors --------------------------------------------------------------------------------");
						null_gui::end_window();
					}
					if (null_gui::begin_window("nullptr 1", vec2(300, 300), vec2(300.f, 0.f), {null_gui::window_flags::set_size, null_gui::window_flags::auto_size }, nullptr)) {
						null_gui::text(utils::format("%d", test_int));
						null_gui::same_line();
						null_gui::deeps::push_var({ &null_gui::gui_settings::items_size_full_window, false }); {
							if (null_gui::button("-")) {
								test_int--;
							}
							null_gui::same_line();
							if (null_gui::button("+")) {
								test_int++;
							}
						} null_gui::deeps::pop_var();
						null_gui::slider_int("slider", &test_int, 0, 100);
						null_gui::combo("combo", &test_int, { "nullptr", "null-gui", "https://github.com/0suddenly0/null-gui", "1", "2", "3", "4", "suddenly" });
						null_gui::multicombo("multicombo", &test_bools, { "head", "body", "legs", "arms" });
						null_gui::checkbox("show debug window", &debug_window);
						null_gui::tooltip([]() {null_gui::text("test tooltip"); });
						null_gui::checkbox("show settings window", &settings_window);
						null_gui::colorpicker("color", &null_gui::gui_settings::main_color);
						null_gui::end_window();
					}

					null_gui::begin_window("test columns window", vec2(10, 10), vec2(400, 300), nullptr); {
						null_gui::begin_columns(3); {
							null_gui::button("test button##1");
							null_gui::next_column();
							null_gui::button("test button##2");
							null_gui::button("test button##3");
							null_gui::button("test button##4");
							null_gui::next_column();
							null_gui::button("test button##5");
						} null_gui::end_columns();
						null_gui::end_window();
					}
				}
				null_render::end();

				});

			if (GetAsyncKeyState(VK_END)) {
				ExitProcess(-1);
			}

			d3d_device->EndScene();
		}
		d3d_device->Present(NULL, NULL, NULL, NULL);
	}
	cleanup_device_d3d();

	DestroyWindow(window);
	UnregisterClass(lpzClass, hInstance);

	return 0;
}
