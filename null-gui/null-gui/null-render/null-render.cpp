#include "null-render.h"
#include "../null-gui.h"
#define pi 3.14159265359f

std::shared_mutex mutex;

null_render::null_draw_list* lower_draw_list_safe = new null_render::null_draw_list;
std::vector<null_render::null_draw_list*> draw_lists_safe;
null_render::null_draw_list* upper_draw_list_safe = new null_render::null_draw_list;

namespace null_font {
	font::font(std::string _name, int _size) {
		create_font(_name, _size, this);
	}

	vec2 font::text_size(std::string text) {
		RECT rect = RECT();
		data->DrawTextA(nullptr , text.c_str(), text.length(), &rect, DT_CALCRECT, D3DCOLOR_RGBA(0, 0, 0, 0));
		return vec2( rect.right - rect.left, rect.bottom - rect.top );
	}

	void font::resize(int new_size) {
		if (size == new_size) return;
		D3DXCreateFontA(null_render::device, new_size, 0, 100, 4, FALSE, DEFAULT_CHARSET, 1, DEFAULT_QUALITY, DEFAULT_PITCH, name.data(), &data);
		size = new_size;
	}

	vec2 text_size(std::string text, font* fnt) {
		font _fnt = !fnt ? pushed_fonts.size() <= 0 ? main_font : pushed_fonts.back() : *fnt;

		vec2 formated_size = _fnt.text_size("_" + text + "_");
		formated_size.x -= _fnt.text_size("_").x * 2;
		return formated_size;
	}

	void create_font(std::string name, int size, font* fnt, bool set_main) {
		fnt->size = size;
		fnt->name = name;

		if (!null_render::device) {
			auto result = std::find(external_create_fonts.begin(), external_create_fonts.end(), fnt);
			if (result == external_create_fonts.end() || external_create_fonts.size() <= 0)
				external_create_fonts.push_back(fnt);
			return;
		}

		if (!fnt->created) {
			if (D3DXCreateFontA(null_render::device, size, 0, 100, 4, FALSE, DEFAULT_CHARSET, 1, DEFAULT_QUALITY, DEFAULT_PITCH, name.data(), &fnt->data) >= 0)
				fnt->created = true;
		}

		if (set_main) main_font = *fnt;

		auto result = std::find(all_fonts.begin(), all_fonts.end(), fnt);
		if (result == all_fonts.end())
			all_fonts.push_back(fnt);
	}

	void create_font(std::string name, int size) {
		if (!main_font.created) D3DXCreateFontA(null_render::device, size, 0, 100, 4, FALSE, DEFAULT_CHARSET, 1, DEFAULT_QUALITY, DEFAULT_PITCH, name.data(), &main_font.data);
		main_font.created = true;
		main_font.size = size;
		main_font.name = name;
	}
}

namespace null_render {
	namespace primitive_render {
		void draw_rect(IDirect3DDevice9* device, vec2 start, vec2 end, color clr) {
			null_render::vertice verts[5] = {
				{ float(start.x), float(start.y), 0.01f, 0.01f, clr.get_d3d() },
				{ float(end.x), float(start.y), 0.01f, 0.01f, clr.get_d3d() },
				{ float(end.x), float(end.y), 0.01f, 0.01f, clr.get_d3d() },
				{ float(start.x), float(end.y), 0.01f, 0.01f, clr.get_d3d() },
				{ float(start.x), float(start.y), 0.01f, 0.01f, clr.get_d3d() }
			};

			device->SetTexture(0, nullptr);
			device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &verts, 20);
		}

		void draw_rect_filled(IDirect3DDevice9* device, vec2 start, vec2 end, color clr) {
			null_render::vertice verts[4] = {
				{ float(start.x), float(start.y), 0.01f, 0.01f, clr.get_d3d() },
				{ float(end.x), float(start.y), 0.01f, 0.01f, clr.get_d3d() },
				{ float(start.x), float(end.y), 0.01f, 0.01f, clr.get_d3d() },
				{ float(end.x), float(end.y), 0.01f, 0.01f, clr.get_d3d() }
			};

			device->SetTexture(0, nullptr);
			device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
		}
		
		void draw_rect_multicolor(IDirect3DDevice9* device, vec2 start, vec2 end, std::array<color, 2> top, std::array<color, 2> down) {
			null_render::vertice verts[4] = {
				{ start.x, start.y, 0.01f, 0.01f, top.at(0).get_d3d() },
				{ end.x, start.y, 0.01f, 0.01f, top.at(1).get_d3d() },
				{ start.x, end.y, 0.01f, 0.01f, down.at(0).get_d3d() },
				{ end.x, end.y, 0.01f, 0.01f, down.at(1).get_d3d() }
			};

			device->SetTexture(0, nullptr);
			device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
		}

		void clip(IDirect3DDevice9* device, vec2 start, vec2 end) {
			RECT clip_space{ start.x, start.y, end.x, end.y };
			device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
			device->SetScissorRect(&clip_space);
		}

		void draw_text(std::string text, vec2 pos, color clr, bool outline, std::array<bool, 2> centered, vec2 clip_space, null_font::font font) {
			vec2 draw_pos = pos;
			if (centered.at(0)) draw_pos.x -= font.text_size(text).x / 2;
			if (centered.at(1)) draw_pos.y -= font.text_size(text).y / 2;

			if (outline) {
				font.data->DrawTextA(NULL, text.c_str(), -1, new RECT{ (LONG)draw_pos.x + 1, (LONG)draw_pos.y, (LONG)clip_space.x, (LONG)clip_space.y }, 0, color(0.f, 0.f, 0.f, clr.a()).get_d3d());
				font.data->DrawTextA(NULL, text.c_str(), -1, new RECT{ (LONG)draw_pos.x - 1, (LONG)draw_pos.y, (LONG)clip_space.x, (LONG)clip_space.y }, 0, color(0.f, 0.f, 0.f, clr.a()).get_d3d());
				font.data->DrawTextA(NULL, text.c_str(), -1, new RECT{ (LONG)draw_pos.x, (LONG)draw_pos.y + 1, (LONG)clip_space.x, (LONG)clip_space.y }, 0, color(0.f, 0.f, 0.f, clr.a()).get_d3d());
				font.data->DrawTextA(NULL, text.c_str(), -1, new RECT{ (LONG)draw_pos.x, (LONG)draw_pos.y - 1, (LONG)clip_space.x, (LONG)clip_space.y }, 0, color(0.f, 0.f, 0.f, clr.a()).get_d3d());
			}

			font.data->DrawTextA(NULL, text.c_str(), -1, new RECT{ (LONG)draw_pos.x, (LONG)draw_pos.y, (LONG)clip_space.x, (LONG)clip_space.y }, 0, clr.get_d3d());
		}

		void draw_line(IDirect3DDevice9* device, vec2 start, vec2 end, color clr) {
			null_render::vertice verts[2] = {
				{ start.x, start.y, 0.01f, 0.01f, clr.get_d3d() },
				{ end.x, end.y, 0.01f, 0.01f, clr.get_d3d() }
			};

			device->SetTexture(0, nullptr);
			device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &verts, 20);
		}

		void draw_circle(IDirect3DDevice9* device, vec2 pos, color clr, float radius, bool filled) {
			std::vector<null_render::vertice> verticles = {
				{ pos.x, pos.y + radius, 0.01f, 0.01f, (int)clr.get_d3d() },
				{ pos.x, pos.y - radius, 0.01f, 0.01f, (int)clr.get_d3d() },
				{ pos.x + radius, pos.y, 0.01f, 0.01f, (int)clr.get_d3d() },
				{ pos.x - radius, pos.y, 0.01f, 0.01f, (int)clr.get_d3d() },
			};

			int f = 1 - radius;
			int ddF_x = 0;
			int ddF_y = -2 * radius;
			int x = 0;
			int y = radius;

			while (x < y) {
				if (f >= 0) {
					y--;
					ddF_y += 2;
					f += ddF_y;
				}
				x++;
				ddF_x += 2;
				f += ddF_x + 1;

				verticles.push_back({ pos.x + x, pos.y + y, 0.01f, 0.01f, (int)clr.get_d3d() });
				verticles.push_back({ pos.x - x, pos.y + y, 0.01f, 0.01f, (int)clr.get_d3d() });
				verticles.push_back({ pos.x + x, pos.y - y, 0.01f, 0.01f, (int)clr.get_d3d() });
				verticles.push_back({ pos.x - x, pos.y - y, 0.01f, 0.01f, (int)clr.get_d3d() });
				verticles.push_back({ pos.x + y, pos.y + x, 0.01f, 0.01f, (int)clr.get_d3d() });
				verticles.push_back({ pos.x - y, pos.y + x, 0.01f, 0.01f, (int)clr.get_d3d() });
				verticles.push_back({ pos.x + y, pos.y - x, 0.01f, 0.01f, (int)clr.get_d3d() });
				verticles.push_back({ pos.x - y, pos.y - x, 0.01f, 0.01f, (int)clr.get_d3d() });
			}

			device->DrawPrimitiveUP(filled ? D3DPT_LINESTRIP : D3DPT_POINTLIST, verticles.size() - 1, &verticles[0], sizeof(null_render::vertice));
		}
	}

	void null_draw_list::add_text(std::string text, vec2 pos, color clr, null_font::font font, bool outline, std::array<bool, 2> centered) {
		draw_call call;
		call.call_type = draw_call_type::text;
		call.call_text = draw_calls::call_text{ pos, clr, outline, centered, text, !font.data ? null_font::pushed_fonts.size() <= 0 ? null_font::main_font : null_font::pushed_fonts.back() : font };
		calls.push_back(call);
	}

	void null_draw_list::add_text(std::string text, vec2 pos, color clr, bool outline, std::array<bool, 2> centered) {
		draw_call call;
		call.call_type = draw_call_type::text;
		call.call_text = draw_calls::call_text{ pos, clr, outline, centered, text, null_font::pushed_fonts.size() <= 0 ? null_font::main_font : null_font::pushed_fonts.back() };
		calls.push_back(call);
	}

	void null_draw_list::push_clip(vec2 start, vec2 end) {
		if (clips.size() > 0) {
			draw_calls::call_clip last_clip = clips[clips.size() - 1];
			start.x = null_math::clamp(start.x, last_clip.start.x, last_clip.end.x);
			start.y = null_math::clamp(start.y, last_clip.start.y, last_clip.end.y);

			end.x = null_math::clamp(end.x, last_clip.start.x, last_clip.end.x);
			end.y = null_math::clamp(end.y, last_clip.start.y, last_clip.end.y);
		}

		clips.push_back({ start, end });

		draw_call call;
		call.call_type = draw_call_type::clip;
		call.call_clip = draw_calls::call_clip{ start, end };
		calls.push_back(call);
	}

	rect null_draw_list::get_clip() {
		if (clips.size() <= 0) return rect(vec2(-9999, -9999), vec2(9999, 9999));
		return rect(clips[clips.size() - 1].start, clips[clips.size() - 1].end);
	}

	void null_draw_list::pop_clip() {
		if(!clips.empty()) clips.pop_back();

		draw_call call;
		call.call_type = draw_call_type::clip;
		call.call_clip = clips.size() <= 0 ? draw_calls::call_clip{ vec2(-9999, -9999), vec2(9999, 9999) } : draw_calls::call_clip{ clips[clips.size() - 1].start, clips[clips.size() - 1].end };
		calls.push_back(call);
	}

	void null_draw_list::add_rect(vec2 start, vec2 end, color clr, bool filled) {
		draw_call call;
		call.call_type = draw_call_type::rect;
		call.call_rect = draw_calls::call_rect{ start, end, clr, filled };
		calls.push_back(call);
	}

	void null_draw_list::add_rect_multicolor(vec2 start, vec2 end, std::array<color, 2> top, std::array<color, 2> down) {
		draw_call call;
		call.call_type = draw_call_type::rect_multicolor;
		call.call_rect_multicolor = draw_calls::call_rect_multicolor{ start, end, top, down };
		calls.push_back(call);
	}

	void null_draw_list::add_line(vec2 start, vec2 end, color clr) {
		draw_call call;
		call.call_type = draw_call_type::line;
		call.call_line = draw_calls::call_line{ start, end, clr };
		calls.push_back(call);
	}

	void null_draw_list::add_circle(vec2 pos, color clr, float radius, bool filled) {
		draw_call call;
		call.call_type = draw_call_type::circle;
		call.call_circle = draw_calls::call_circle{ pos, clr, radius, filled };
		calls.push_back(call);
	}

	void null_draw_list::draw() {
		for (draw_call& call : calls) {
			switch (call.call_type) {
			case draw_call_type::text: call.call_text.draw(clips); break;
			case draw_call_type::clip: call.call_clip.clip(device); break;
			case draw_call_type::rect: call.call_rect.draw(device); break;
			case draw_call_type::line: call.call_line.draw(device); break;
			case draw_call_type::circle: call.call_circle.draw(device); break;
			case draw_call_type::rect_multicolor: call.call_rect_multicolor.draw(device); break;
			}
		}
	}

	void null_draw_list::clear() {
		calls.clear();
		clips.clear();
	}

	void null_draw_list::swap(null_draw_list* draw_list) {
		calls.swap(draw_list->calls);
		clips.swap(draw_list->clips);
	}

	void init(IDirect3DDevice9* _device, D3DPRESENT_PARAMETERS* _d3dp) {
		device = _device;
		d3dp = _d3dp;
	}

	void begin_render_states() {
		if (device->CreateStateBlock(D3DSBT_ALL, &d3d9_state_block) < 0)
			return;

		vec2 display_size = null_gui::deeps::display_size;

		device->SetPixelShader(NULL);
		device->SetVertexShader(NULL);
		device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		device->SetRenderState(D3DRS_LIGHTING, false);
		device->SetRenderState(D3DRS_FOGENABLE, false);
		device->SetRenderState(D3DRS_ZENABLE, false);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
		device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

		D3DVIEWPORT9 vp;
		vp.X = vp.Y = 0;
		vp.Width = (DWORD)display_size.x;
		vp.Height = (DWORD)display_size.y;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;
		device->SetViewport(&vp);

		float L = 0.5f, R = display_size.x + 0.5f, T = 0.5f, B = display_size.y + 0.5f;
		D3DMATRIX mat_identity = { { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f } };
		D3DMATRIX mat_projection = {
			2.0f / (R - L),   0.0f,         0.0f,  0.0f,
			0.0f,         2.0f / (T - B),   0.0f,  0.0f,
			0.0f,         0.0f,         0.5f,  0.0f,
			(L + R) / (L - R),  (T + B) / (B - T),  0.5f,  1.0f,
		};
		device->SetTransform(D3DTS_WORLD, &mat_identity);
		device->SetTransform(D3DTS_VIEW, &mat_identity);
		device->SetTransform(D3DTS_PROJECTION, &mat_projection);
	}

	void end_render_states() {
		d3d9_state_block->Apply();
		d3d9_state_block->Release();
	}

	void reset_device_d3d() {
		clear_device_objects();

		HRESULT hr = device->Reset(d3dp);
		if (hr != D3DERR_INVALIDCALL)
			create_device_objects();
	}

	void clear_device_objects() {
		for (null_font::font* fnt : null_font::all_fonts) fnt->data->OnLostDevice();
	}

	void create_device_objects() {
		for (null_font::font* fnt : null_font::all_fonts) fnt->data->OnResetDevice();
	}

	void begin() {
		for (int i = 0; i < null_font::external_create_fonts.size(); i++) {
			null_font::font* fnt = null_font::external_create_fonts[i];
			null_font::create_font(fnt->name, fnt->size, fnt);
			null_font::external_create_fonts.erase(null_font::external_create_fonts.begin() + i);
		}

		null_font::pushed_fonts.clear();
		lower_draw_list->clear();
		for (null_draw_list* list : draw_lists) { list->clear(); }
		draw_lists.clear();
		upper_draw_list->clear();
	}

	void end() {
		std::unique_lock<std::shared_mutex> lock(mutex);

		for (null_gui::window* wnd : null_gui::deeps::windows)
			if(!wnd->have_flag(null_gui::window_flags::group)) add_draw_list(wnd->draw_list);

		lower_draw_list_safe->swap(lower_draw_list);
		draw_lists_safe.swap(draw_lists);
		upper_draw_list_safe->swap(upper_draw_list);
	}

	void render() {
		std::unique_lock<std::shared_mutex> lock(mutex);

		lower_draw_list_safe->draw();
		for (null_draw_list* list : draw_lists_safe) { list->draw(); }
		upper_draw_list_safe->draw();
	}
}