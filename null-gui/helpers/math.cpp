#include "math.h"

namespace math {
    vec2 line_closest_point(const vec2& a, const vec2& b, const vec2& p) {
        vec2 ap = p - a;
        vec2 ab_dir = b - a;
        float dot = ap.x * ab_dir.x + ap.y * ab_dir.y;
        if (dot < 0.0f) return a;
        float ab_len_sqr = ab_dir.x * ab_dir.x + ab_dir.y * ab_dir.y;
        if (dot > ab_len_sqr) return b;
        return a + ab_dir * dot / ab_len_sqr;
    }

    std::array<vec2, 3> calc_arrow(float scale) {
        float r = scale * 0.4f;
        return { vec2(+0.000f, +0.750f) * r, vec2(-0.866f, -0.750f) * r, vec2(+0.866f, -0.750f) * r };
    }

}