#pragma once
#include <cmath>
#define deg(a) a * 57.295779513082f

class vec2 {
public:
    vec2() { x = y = 0.f; }

    template <typename T>
    vec2(T _x, T _y) {
        x = _x;
        y = _y;
    }

    vec2 normalized() {
        vec2 res = *this;
        float l = res.length_sqrt();
        if (l != 0.0f) {
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

        return delta.length_sqrt();
    }

    float dot(const vec2& src) const { return (x * src.x + y * src.y); }

    float length() { return (float)(x * x + y * y); }
    float length_sqrt(void) { return (float)sqrt(x * x + y * y); }

    float& operator[](int i) { return ((float*)this)[i];  }
    float operator[](int i) const { return ((float*)this)[i]; }

    bool operator==(const vec2& src) { return (x == src.x) && y == src.y; }
    bool operator==(float src) { return x == src && y == src; }

    bool operator!=(const vec2& src) { return (x != src.x) || y != src.y; }
    bool operator!=(float src) { return x != src || y != src; }

    bool operator<=(const vec2& src) { return x <= src.x && y <= src.y; }
    bool operator<=(float src) { return x <= src && y <= src; }

    bool operator>=(const vec2& src) { return x >= src.x && y >= src.y; }
    bool operator>=(float src) { return x >= src && y >= src; }

    bool operator<(const vec2& src) {  return x < src.x && y < src.y; }
    bool operator<(float src) { return x < src && y < src; }

    bool operator>(const vec2& src) {  return x > src.x && y > src.y; }
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

    vec2 operator*(const vec2& src) {  return vec2(x * src.x, y * src.y); }
    vec2 operator*(float src) { return vec2(x * src, y * src); }

    vec2 operator/(const vec2& src) { return vec2(x / src.x, y / src.y); }
    vec2 operator/(float src) { return vec2(x / src, y / src); }

    float x, y;
};

class rect {
public:
    rect() { min = max = 0.f; }

    template <typename T>
    rect(T min_x, T min_y, T max_x, T max_y) {
        min = vec2(min_x, min_y);
        max = vec2(max_x, max_y);
    }

    rect(vec2 _min, vec2 _max) {
        min = _min;
        max = _max;
    }

    vec2 size() { return max - min; }
    vec2 centre() { return min + size() / 2; }

    float operator[](int i) const { return ((float*)this)[i]; }
    float& operator[](int i) { return ((float*)this)[i]; }

    bool operator==(const rect& src) { return min == src.min && max == src.max; }
    bool operator==(const vec2& src) { return min == src && max == src; }
    bool operator==(float src) { return min == src && max == src; }

    bool operator!=(const rect& src) { return min != src.min || max != src.max; }
    bool operator!=(const vec2& src) { return min != src || max != src; }
    bool operator!=(float src) { return min != src || max != src; }

    bool operator<=(const rect& src) { return min <= src.min && max <= src.max; }
    bool operator<=(const vec2& src) { return min <= src && max <= src; }
    bool operator<=(float src) { return min <= src && max <= src; }

    bool operator>=(const rect& src) { return min >= src.min && max >= src.max; }
    bool operator>=(const vec2& src) { return min >= src && max >= src; }
    bool operator>=(float src) { return min >= src && max >= src; }

    bool operator<(const rect& src) { return min < src.min && max < src.max; }
    bool operator<(const vec2& src) { return min < src && max < src; }
    bool operator<(float src) { return min < src&& max < src; }

    bool operator>(const rect& src) { return min > src.min && max > src.max; }
    bool operator>(const vec2& src) { return min > src && max > src; }
    bool operator>(float src) { return min > src && max > src; }

    rect& operator*=(const rect& src) { min *= src.min; max *= src.max; return *this; }
    rect& operator*=(const vec2& src) { min *= src; max *= src; return *this; }
    rect& operator*=(float src) { min *= src; max *= src; return *this; }

    rect& operator/=(const rect& src) { min /= src.min; max /= src.max; return *this; }
    rect& operator/=(const vec2& src) { min /= src; max /= src; return *this; }
    rect& operator/=(float src) { min /= src; max /= src; return *this; }

    rect& operator+=(const rect& src) { min += src.min; max += src.max; return *this; }
    rect& operator+=(const vec2& src) { min += src; max += src; return *this; }
    rect& operator+=(float src) { min += src; max += src; return *this; }

    rect& operator-=(const rect& src) { min -= src.min; max -= src.max; return *this; }
    rect& operator-=(const vec2& src) { min -= src; max -= src; return *this; }
    rect& operator-=(float src) { min -= src; max -= src; return *this; }

    rect& operator=(const rect& src) { min = src.min; max = src.max; return *this; }
    rect& operator=(const vec2& src) { min = max = src; return *this; }
    rect& operator=(float src) { min = max = src; return *this; }

    rect operator+(const rect& src) { return rect(min + src.min, max + src.max); }
    rect operator+(const vec2 src) { return rect(min + src, max + src); }
    rect operator+(float src) { return rect(min + src, max + src); }

    rect operator-(void) const { return rect(-min, -max); }
    rect operator-(const rect& src) { return rect(min - src.min, max - src.max); }
    rect operator-(const vec2& src) const { return rect(min - src, max - src); }
    rect operator-(float src) { return rect(min - src, max - src); }

    rect operator*(const rect& src) { return rect(min * src.min, max * src.max); }
    rect operator*(const vec2& src) { return rect(min * src, max * src); }
    rect operator*(float src) { return rect(min * src, max * src); }

    rect operator/(const rect& src) { return rect(min / src.min, min / src.max); }
    rect operator/(const vec2& src) { return rect(min / src, max / src); }
    rect operator/(float src) { return rect(min / src, max / src); }

    vec2 min, max;
};