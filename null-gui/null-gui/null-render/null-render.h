#pragma once
#include <string>
#include <vector>
#include <array>
#include <array>
#include <assert.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <mutex>
#include <shared_mutex>
#include "../../helpers/color.h"
#include "../../helpers/vectors.h"

namespace null_font {
	typedef LPD3DXFONT null_font_data;

	class font {
	public:
		font() { created = false; }	
		vec2 text_size(std::string text);
		void resize(int new_size);
		std::string name;
		int size;
		bool created;
		null_font_data data;
	};

	font main_font;
	std::vector<font> pushed_fonts;

	vec2 text_size(std::string text, font* fnt = nullptr);
	void push_font(font _font) { pushed_fonts.push_back(_font); }
	void pop_font() { pushed_fonts.pop_back(); }
	void create_font(std::string name, int size, font* fnt, bool set_main = false);
}

namespace null_render {
	namespace primitive_render {
		void draw_rect(IDirect3DDevice9* device, vec2 start, vec2 end, color clr);
		void draw_rect_filled(IDirect3DDevice9* device, vec2 start, vec2 end, color clr);
		void draw_rect_multicolor(IDirect3DDevice9* device, vec2 start, vec2 end, std::array<color, 2> left, std::array<color, 2> right);
		void clip(IDirect3DDevice9* device, vec2 start, vec2 end);
		void draw_text(std::string text, vec2 pos, color clr, bool outline, std::array<bool, 2> centered, vec2 clip_space, null_font::font font);
		void draw_line(IDirect3DDevice9* device, vec2 start, vec2 end, color clr);
		void draw_circle(IDirect3DDevice9* device, vec2 pos, color clr, bool filled);
	}

	struct vertice {
		float x, y, z, rhw;
		int c;
	};

	enum class draw_call_type {
		none,
		text,
		clip,
		rect,
		rect_multicolor,
		line,
		circle,
		poly
	};

	namespace draw_calls {
		class call_clip {
		public:
			void clip(IDirect3DDevice9* device);

			vec2 start;
			vec2 end;
		};

		class call_text {
		public:
			void draw(std::vector<call_clip> clips);

			vec2 pos;
			color clr;
			bool outline;
			std::array<bool, 2> centered;
			std::string text;
			null_font::font font;
		};

		class call_rect {
		public:
			void draw(IDirect3DDevice9* device);

			vec2 start;
			vec2 end;
			color clr;
			bool filled = true;
		};

		class call_rect_multicolor {
		public:
			void draw(IDirect3DDevice9* device);

			vec2 start;
			vec2 end;
			std::array<color, 2> top; /* 1 - left color : 2 - right color */
			std::array<color, 2> down; /* 1 - left color : 2 - right color */
		};

		class call_line {
		public:
			void draw(IDirect3DDevice9* device);

			vec2 start;
			vec2 end;
			color clr;
		};

		class call_circle {
		public:
			void draw(IDirect3DDevice9* device);

			vec2 pos;
			color clr;
			float radius;
			bool filled = true;
		};

		class call_poly {
		public:
			void draw(IDirect3DDevice9* device);

			std::vector<vec2> points;
			color clr;
			bool filled = true;
		};
	}

	class draw_call {
	public:
		draw_call() {}

		draw_call_type call_type = draw_call_type::none;

		draw_calls::call_text            call_text;
		draw_calls::call_clip            call_clip;
		draw_calls::call_rect            call_rect;
		draw_calls::call_line            call_line;
		draw_calls::call_circle          call_circle;
		draw_calls::call_poly            call_poly;
		draw_calls::call_rect_multicolor call_rect_multicolor;
	};

	class null_draw_list {
	public:
		std::vector<draw_calls::call_clip> clips;
		std::vector<draw_call> calls;

		template<typename T>
		void add_text(std::string text, T x, T y, color clr, null_font::font font, bool outline = true, std::array<bool, 2> centered = { false, false }) {
			add_text(text, vec2(x, y), clr, font, outline, centered);
		}
		void add_text(std::string text, vec2 pos, color clr, null_font::font font, bool outline = true, std::array<bool, 2> centered = { false, false });

		template<typename T>
		void add_text(std::string text, T x, T y, color clr, bool outline = true, std::array<bool, 2> centered = { false, false }) {
			add_text(text, vec2(x, y), clr, outline, centered);
		}
		void add_text(std::string text, vec2 pos, color clr, bool outline = true, std::array<bool, 2> centered = { false, false });

		template<typename T>
		void push_clip(T x, T y, T x1, T y1) {
			push_clip(vec2(x, y), vec2(x1, y1));
		}
		void push_clip(vec2 start, vec2 end);
		void pop_clip();

		template<typename T>
		void add_rect(T x, T y, T x1, T y1, color clr, bool filled = true) {
			add_rect(vec2(x, y), vec2(x1, y1), clr, filled);
		}
		void add_rect(vec2 start, vec2 end, color clr, bool filled = true);

		template<typename T>
		void add_rect_multicolor(T x, T y, T x1, T y1, std::array<color, 2> top, std::array<color, 2> down) {
			add_rect_multicolor(vec2(x, y), vec2(x1, y1), top, down);
		}
		void add_rect_multicolor(vec2 start, vec2 end, std::array<color, 2> top, std::array<color, 2> down);

		template<typename T>
		void add_line(T x, T y, T x1, T y1, color clr) {
			add_line(vec2(x, y), vec2(x1, y1), clr);
		}
		void add_line(vec2 start, vec2 end, color clr);

		template<typename T>
		void add_circle(T x, T y, color clr, float radius, bool filled) {
			add_circle(vec2(x, y), clr, radius, filled);
		}
		void add_circle(vec2 pos, color clr, float radius, bool filled);

		void draw();
		void clear();
		void swap(null_draw_list draw_list);
	};

	IDirect3DDevice9* device;

	null_draw_list lower_draw_list;
	std::vector<null_draw_list*> draw_lists;
	null_draw_list upper_draw_list;

	void init(IDirect3DDevice9* _device);

	void begin();
	void end();

	void render();

	void add_draw_list(null_draw_list* list);
}