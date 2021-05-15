#pragma once
#define NOMINMAX
#include <windows.h>

#include "color.h"
#include "vectors.h"

#define pi 3.14159265358979323846f
#define NORMALIZE2F_OVER_ZERO(VX,VY) do { float d2 = VX*VX + VY*VY; if (d2 > 0.0f) { float inv_len = 1.0f / sqrtf(d2); VX *= inv_len; VY *= inv_len; } } while (0)
#define FIXNORMAL2F(VX,VY) do { float d2 = VX*VX + VY*VY; if (d2 < 0.5f) d2 = 0.5f; float inv_lensq = 1.0f / d2; VX *= inv_lensq; VY *= inv_lensq; } while (0)

namespace math {
	template <typename T> T min(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }
	template <typename T> T max(T lhs, T rhs) { return lhs >= rhs ? lhs : rhs; }	
	template <typename T> T clamp(T v, T mn, T mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }
	float lerp(float a, float b, float t) { return a + t * (b - a); }

	vec2 min(vec2 lhs, vec2 rhs) { return vec2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
	vec2 max(vec2 lhs, vec2 rhs) { return vec2(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
	vec2 clamp(const vec2& v, const vec2& mn, vec2 mx) { return vec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y); }
	float floor(float f) { return (float)(int)(f); }
	vec2 floor(const vec2& v) { return vec2((float)(int)(v.x), (float)(int)(v.y)); }
	vec2 mul(const vec2& lhs, const vec2& rhs) { return vec2(lhs.x * rhs.x, lhs.y * rhs.y); }

	vec2 line_closest_point(const vec2& a, const vec2& b, const vec2& p);
}