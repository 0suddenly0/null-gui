#pragma once
#include <cmath>

class vec2 {
public:
    vec2() { x = y = 0.f; }

    template <typename t>
    vec2(t _x, t _y) : x(_x), y(_y) {}

    vec2 normalized() {
        vec2 res = *this;
        float l = sqrt(res.length());
        if(l != 0.0f) {
            res /= l;
        } else {
            res.x = res.y = 0.0f;
        }
        return res;
    }

    float dist_to(const vec2& src) {
        vec2 delta;

        delta.x = x - src.x;
        delta.y = y - src.y;

        return sqrt(delta.length());
    }

    float dot(const vec2& src) const { return (x * src.x + y * src.y); }

    float length() { return (float)(x * x + y * y); }

    float& operator[](int i) { return ((float*)this)[i]; }
    float operator[](int i) const { return ((float*)this)[i]; }

    bool operator==(const vec2& src) { return (x == src.x) && y == src.y; }
    bool operator==(float src) { return x == src && y == src; }

    bool operator!=(const vec2& src) { return (x != src.x) || y != src.y; }
    bool operator!=(float src) { return x != src || y != src; }

    bool operator<=(const vec2& src) { return x <= src.x && y <= src.y; }
    bool operator<=(float src) { return x <= src && y <= src; }

    bool operator>=(const vec2& src) { return x >= src.x && y >= src.y; }
    bool operator>=(float src) { return x >= src && y >= src; }

    bool operator<(const vec2& src) { return x < src.x&& y < src.y; }
    bool operator<(float src) { return x < src&& y < src; }

    bool operator>(const vec2& src) { return x > src.x && y > src.y; }
    bool operator>(float src) { return x > src && y > src; }

    vec2& operator*=(const vec2& src) { x *= src.x; y *= src.y; return *this; }
    vec2& operator*=(float src) { x *= src;  y *= src; return *this; }

    vec2& operator/=(const vec2& src) { x /= src.x; y /= src.y; return *this; }
    vec2& operator/=(float src) { x /= src; y /= src; return *this; }

    vec2& operator+=(const vec2& src) { x += src.x; y += src.y; return *this; }
    vec2& operator+=(float src) { x += src; y += src; return *this; }

    vec2& operator-=(const vec2& src) { x -= src.x; y -= src.y; return *this; }
    vec2& operator-=(float src) { x -= src; y -= src; return *this; }

    vec2& operator=(const vec2& src) { x = src.x; y = src.y; return *this; }
    vec2& operator=(float src) { x = src; y = src; return *this; }

    vec2 operator+(const vec2& src) const { return vec2(x + src.x, y + src.y); }
    vec2 operator+(const vec2& src) { return vec2(x + src.x, y + src.y); }
    vec2 operator+(const float src) { return vec2(x + src, y + src); }

    vec2 operator-(void) const { return vec2(-x, -y); }
    vec2 operator-(const vec2& src) const { return vec2(x - src.x, y - src.y); }
    vec2 operator-(const vec2& src) { return vec2(x - src.x, y - src.y); }
    vec2 operator-(float src) { return vec2(x - src, y - src); }

    vec2 operator*(const vec2& src) { return vec2(x * src.x, y * src.y); }
    vec2 operator*(float src) const { return vec2(x * src, y * src); }
    vec2 operator*(float src) { return vec2(x * src, y * src); }

    vec2 operator/(const vec2& src) { return vec2(x / src.x, y / src.y); }
    vec2 operator/(float src) { return vec2(x / src, y / src); }

    float x, y;
};