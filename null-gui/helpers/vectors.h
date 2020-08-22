#pragma once
#include <cmath>
#define deg(a) a * 57.295779513082f

class vec2 {
public:
    vec2() {
        x = 0.f;
        y = 0.f;
    }

    template <typename T>
    vec2(T X, T Y) {
        x = X;
        y = Y;
    }

    float& operator[](int i) {
        return ((float*)this)[i];
    }

    float operator[](int i) const {
        return ((float*)this)[i];
    }

    bool operator==(const vec2& src) {
        return (src.x == x) && (src.y == y);
    }

    bool operator!=(const vec2& src) {
        return (src.x != x) || (src.y != y);
    }

    bool operator!=(float fl) {
        return (fl != x) || (fl != y);
    }

    bool operator<=(const vec2& src) {
        return (src.x <= x) && (src.y <= y);
    }

    bool operator>=(const vec2& src) {
        return (src.x >= x) && (src.y >= y);
    }

    bool operator<(const vec2& src) {
        return (src.x < x) && (src.y < y);
    }

    bool operator>(const vec2& src) {
        return (src.x > x) && (src.y > y);
    }

    vec2& operator=(float fl) {
        x = fl;
        y = fl;
        return *this;
    }

    vec2& operator=(const vec2& v) {
        x = v.x;
        y = v.y;
        return *this;
    }

    vec2& operator*=(const vec2& v) {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    vec2& operator*=(float fl) {
        x *= fl;
        y *= fl;
        return *this;
    }

    vec2& operator/=(const vec2& v) {
        x /= v.x;
        y /= v.y;
        return *this;
    }

    vec2& operator/=(float fl) {
        x /= fl;
        y /= fl;
        return *this;
    }

    vec2& operator+=(const vec2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    vec2& operator+=(float fl) {
        x += fl;
        y += fl;
        return *this;
    }

    vec2& operator-=(const vec2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    vec2& operator-=(float fl) {
        x -= fl;
        y -= fl;
        return *this;
    }

    vec2 operator-(void) const {
        return vec2(-x, -y);
    }

    vec2 operator+(const vec2& v) {
        return vec2(x + v.x, y + v.y);
    }

    vec2 operator+(const vec2& v) const {
        return vec2(x + v.x, y + v.y);
    }

    vec2 operator+(const float v) {
        return vec2(x + v, y + v);
    }

    vec2 operator-(const float v) {
        return vec2(x - v, y - v);
    }

    vec2 operator-(const vec2& v) {
        return vec2(x - v.x, y - v.y);
    }

    vec2 operator-(const vec2& v) const {
        return vec2(x - v.x, y - v.y);
    }

    vec2 operator*(float fl) {
        return vec2(x * fl, y * fl);
    }

    vec2 operator*(const vec2& v) {
        return vec2(x * v.x, y * v.y);
    }

    vec2 operator/(float fl) {
        return vec2(x / fl, y / fl);
    }

    vec2 operator/(const vec2& v) {
        return vec2(x / v.x, y / v.y);
    }

    float dot(const vec2& vOther) const {
        return (x * vOther.x + y * vOther.y);
    }

    vec2 normalized() {
        vec2 res = *this;
        float l = res.length();
        if (l != 0.0f) {
            res /= l;
        }
        else {
            res.x = res.y = 0.0f;
        }
        return res;
    }

    float dist_to(const vec2& vOther) {
        vec2 delta;

        delta.x = x - vOther.x;
        delta.y = y - vOther.y;

        return delta.length();
    }

    float length() {
        return (float)sqrt(x * x + y * y);
    }

    float length_sqr(void) {
        return (float)(x * x + y * y);
    }

    float x, y;
};

class rect {
public:
    rect() {
        min.x = 0.f;
        min.y = 0.f;
        max.x = 0.f;
        max.y = 0.f;
    }

    rect(vec2 _min, vec2 _max) {
        min.x = _min.x;
        min.y = _min.y;
        max.x = _max.x;
        max.y = _max.y;
    }

    float& operator[](int i) {
        return ((float*)this)[i];
    }

    float operator[](int i) const {
        return ((float*)this)[i];
    }

    bool operator==(const rect& src) {
        return (src.min.x == min.x) && (src.min.y == min.y) && (src.max.x == max.x) && (src.max.y == max.y);
    }

    bool operator!=(const rect& src) {
        return (src.min.x != min.x) || (src.min.y != min.y) || (src.max.x != max.x) || (src.max.y != max.y);
    }

    rect& operator=(float fl) {
        min.x = fl;
        min.y = fl;
        max.x = fl;
        max.y = fl;
        return *this;
    }

    rect& operator=(const rect& v) {
        min.x = v.min.x;
        min.y = v.min.y;
        max.x = v.max.x;
        max.y = v.max.y;
        return *this;
    }

    rect& operator*=(const rect& v) {
        min.x *= v.min.x;
        min.y *= v.min.y;
        max.x *= v.max.x;
        max.y *= v.max.y;
        return *this;
    }

    rect& operator*=(float fl) {
        min.x *= fl;
        min.y *= fl;
        max.x *= fl;
        max.y *= fl;
        return *this;
    }

    rect& operator/=(const rect& v) {
        min.x /= v.min.x;
        min.y /= v.min.y;
        max.x /= v.max.x;
        max.y /= v.max.y;
        return *this;
    }

    rect& operator/=(float fl) {
        min.x /= fl;
        min.y /= fl;
        max.x /= fl;
        max.y /= fl;
        return *this;
    }

    rect& operator+=(const rect& v) {
        min.x += v.min.x;
        min.y += v.min.y;
        max.x += v.max.x;
        max.y += v.max.y;
        return *this;
    }

    rect& operator+=(float fl) {
        min.x += fl;
        min.y += fl;
        max.x += fl;
        max.y += fl;
        return *this;
    }

    rect& operator-=(const rect& v) {
        min.x -= v.min.x;
        min.y -= v.min.y;
        max.x -= v.max.x;
        max.y -= v.max.y;
        return *this;
    }

    rect& operator-=(float fl) {
        min.x -= fl;
        min.y -= fl;
        max.x -= fl;
        max.y -= fl;
        return *this;
    }

    rect operator-(void) const {
        return rect(-min, -max);
    }

    rect operator+(const rect& v) {
        return rect(min + v.min, max + v.min);
    }

    rect operator+(const rect& v) const {
        return rect(min + v.min, max + v.min);
    }

    rect operator+(const float v) {
        return rect(min + v, max + v);
    }

    rect operator-(const float v) {
        return rect(min - v, max - v);
    }

    rect operator-(const rect& v) {
        return rect(min - v.min, max - v.max);
    }

    rect operator-(const rect& v) const {
        return rect(min - v.min, max - v.max);
    }

    rect operator*(float fl) {
        return rect(min * fl, max * fl);
    }

    rect operator*(const rect& v) {
        return rect(min * v.min, max * v.max);
    }

    rect operator/(float fl) {
        return rect(min / fl, max / fl);
    }

    rect operator/(const rect& v) {
        return rect(min / v.min, min / v.min);
    }

    vec2 min, max;
};
