#pragma once
#define NOMINMAX
#include <windows.h>
#include <array>

#include "../datatypes/color.h"
#include "../datatypes/vec2.h"

#define pi 3.14159265358979323846f
#define NORMALIZE2F_OVER_ZERO(VX,VY) do { float d2 = VX*VX + VY*VY; if (d2 > 0.0f) { float inv_len = 1.0f / sqrtf(d2); VX *= inv_len; VY *= inv_len; } } while (0)
#define FIXNORMAL2F(VX,VY) do { float d2 = VX*VX + VY*VY; if (d2 < 0.5f) d2 = 0.5f; float inv_lensq = 1.0f / d2; VX *= inv_lensq; VY *= inv_lensq; } while (0)

namespace math {
	template <typename t> static t min(t a, t b) { return a < b ? a : b; }
	template <typename t> static t max(t a, t b) { return a >= b ? a : b; }
	template <typename t> static t clamp(t val, t min, t max) { return val < min ? min : val > max ? max : val; }
	static float lerp(float a, float b, float t) { return a + t * (b - a); }

	static vec2 min(vec2 a, vec2 b) { return a < b ? a : b; }
	static vec2 max(vec2 a, vec2 b) { return a >= b ? a : b; }
	static vec2 clamp(vec2 val, vec2 min, vec2 max) { return val < min ? min : val > max ? max : val; }
	static float floor(float val) { return (float)(int)(val); }
	static vec2 floor(const vec2& val) { return vec2((float)(int)(val.x), (float)(int)(val.y)); }

	vec2 line_closest_point(const vec2& a, const vec2& b, const vec2& p);
	std::array<vec2, 3> calc_arrow(float scale);
}