#pragma once
#include <d3d9.h>
#include <algorithm>

#define COL32_R_SHIFT    0
#define COL32_G_SHIFT    8
#define COL32_B_SHIFT    16
#define COL32_A_SHIFT    24
#define COL32_A_MASK     0xFF000000
#define COL32(R,G,B,A)    (((unsigned int)(A)<<COL32_A_SHIFT) | ((unsigned int)(B)<<COL32_B_SHIFT) | ((unsigned int)(G)<<COL32_G_SHIFT) | ((unsigned int)(R)<<COL32_R_SHIFT))

class color {
public:
    float& r() { return source[0]; }
    float& g() { return source[1]; }
    float& b() { return source[2]; }
    float& a() { return source[3]; }

    color() { inalizate(255, 255, 255, 255); }
	color(int r, int g, int b, int a = 255) { inalizate(r, g, b, a); }
	color(float r, float g, float b, float a = 1.f) { inalizate(r, g, b, a); }
	color(color clr, int a) { clr.convert_to_int(); inalizate((int)clr.r(), (int)clr.g(), (int)clr.b(), a); }
	color(color clr, float a) { inalizate(clr.r(), clr.g(), clr.b(), a); }

    void convert_to_int() { source[0] *= 255.f; source[1] *= 255.f; source[2] *= 255.f; source[3] *= 255.f; }
    void convert_to_float() { source[0] /= 255.f; source[1] /= 255.f; source[2] /= 255.f; source[3] /= 255.f; }

	color get_convert_to_int() { return color(source[0] * 255.f, source[1] * 255.f, source[2] * 255.f, source[3] * 255.f); }
	color get_convert_to_float() { return color(source[0] / 255.f, source[1] / 255.f, source[2] / 255.f, source[3] / 255.f); }

	color rgb_to_hsv() {
		color out;
		out.a() = a();
		double min, max, delta;

		min = r() < g() ? r() : g();
		min = min < b() ? min : b();

		max = r() > g() ? r() : g();
		max = max > b() ? max : b();

		out.b() = max;
		delta = max - min;
		if (delta < 0.00001) {
			out.g() = 0;
			out.r() = 0;
			return out;
		}

		if (max > 0.0) {
			out.g() = (delta / max);
		} else {
			out.g() = 0.0;
			out.r() = NAN;
			return out;
		}

		if (r() >= max) out.r() = (g() - b()) / delta;
		else {
			if (g() >= max) out.r() = 2.0 + (b() - r()) / delta;
			else out.r() = 4.0 + (r() - g()) / delta;
		}

		out.r() *= 60.0;
		if (out.r() < 0.0) out.r() += 360.0;

		return out;
	}

	color hsv_to_rgb() {
		double hh, p, q, t, ff;
		long i;
		color out;
		out.a() = a();

		if (g() <= 0.0) {
			out = color(b(), b(), b(), a());
			return out;
		}

		hh = r();
		if (hh >= 360.0) hh = 0.0;
		hh /= 60.0;
		i = (long)hh;
		ff = hh - i;
		p = b() * (1.0 - g());
		q = b() * (1.0 - (g() * ff));
		t = b() * (1.0 - (g() * (1.0 - ff)));

		switch (i) {
		case 0: out.r() = b(); out.g() = t; out.b() = p; break;
		case 1: out.r() = q; out.g() = b(); out.b() = p; break;
		case 2: out.r() = p; out.g() = b(); out.b() = t; break;
		case 3: out.r() = p; out.g() = q; out.b() = b(); break;
		case 4: out.r() = t; out.g() = p; out.b() = b(); break;
		case 5:
		default: out.r() = b(); out.g() = p; out.b() = q; break;
		}

		return out;
	}

	unsigned int to_unsigned() {
		color clr = get_convert_to_int();
		return COL32((int)clr.source[0], (int)clr.source[1], (int)clr.source[2], (int)clr.source[3]);
	}

	color operator+(color _color) {
		return color(r() + _color.r(), g() + _color.g(), b() + _color.b(), a() + _color.a());
	}

	color operator-(color& _color) {
		return color(r() - _color.r(), g() - _color.g(), b() - _color.b(), a() - _color.a());
	}

	color operator*(float& value) {
		return color(r() * value, g() * value, b() * value, a() * value);
	}

    void operator=(const color& _color) {
		inalizate(_color.source[0], _color.source[1], _color.source[2], _color.source[3]);
	}

    color& operator=(const float* _color) {
		inalizate(_color[0], _color[1], _color[2], _color[3]); return *this;
	}

    color& operator=(const int* _color) {
		inalizate(_color[0], _color[1], _color[2], _color[3]); return *this;
	}

    bool operator==(const color& rhs) const {
		return rhs.source[0] == source[0] && rhs.source[1] == source[1] && rhs.source[2] == source[2] && rhs.source[3] == source[3];
	}

    bool operator!=(const color& rhs) const {
		return !(operator==(rhs));
	}

    int get_raw() const { return *((int*)this); }
    void set_raw(int color32) { *((int*)this) = color32; }

    void inalizate(float r, float g, float b, float a) { source[0] = r; source[1] = g; source[2] = b; source[3] = a; }
    void inalizate(int r, int g, int b, int a) { source[0] = r / 255.f; source[1] = g / 255.f; source[2] = b / 255.f; source[3] = a / 255.f; }

    float source[4];
};