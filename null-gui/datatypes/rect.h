#pragma once

class vec2;

class rect {
public:
    rect() { min = max = 0.f; }

    template <typename t>
    rect(t min_x, t min_y, t max_x, t max_y) { min = vec2(min_x, min_y); max = vec2(max_x, max_y); }
    rect(vec2 _min, vec2 _max) { min = _min; max = _max; }

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

    bool operator<(const rect& src) { return min < src.min&& max < src.max; }
    bool operator<(const vec2& src) { return min < src&& max < src; }
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
    rect operator*(float src) const { return rect(min * src, max * src); }
    rect operator*(float src) { return rect(min * src, max * src); }

    rect operator/(const rect& src) { return rect(min / src.min, min / src.max); }
    rect operator/(const vec2& src) { return rect(min / src, max / src); }
    rect operator/(float src) { return rect(min / src, max / src); }

    vec2 min, max;
};