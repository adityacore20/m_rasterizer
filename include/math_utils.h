#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>

// shear matrix * rotation matrix = composition matrix

static constexpr float PI = 3.14159265358979f;
static constexpr float deg_radian = PI / 180.0f;
static constexpr float radian_deg = 180.0f / PI;
static constexpr float EPS = 1e-6f; // to ensure backface culling

inline float clampf(float v, float low,
        float high) { // clamping helper used to prevent visual
                      // glitches and to manage memory crash
    return v < low ? low : v > high ? high : v;
}

// for calculating z depth and vertex attribute interpolation used for colour
// management ,smooth visuals
inline float lerpf(float a, float b, float t) { return (a + (b - a) * t); }

// Lighting Math, When calculating the brightness of a surface using a dot
// product
inline float saturate(float v) { return clampf(v, 0.0f, 1.0f); }

struct Vector2d {
    float x = 0;
    float y = 0;

    Vector2d() = default;

    Vector2d(float x, float y) : x(x), y(y) {};

    Vector2d operator+(const Vector2d &o) const { return {x + o.x, y + o.y}; }

    Vector2d operator-(const Vector2d &o) const { return {x - o.x, y - o.y}; }

    Vector2d operator*(float s) const { return {x * s, y * s}; }

    Vector2d operator/(float s) const { return {x / s, y / s}; }

    Vector2d &operator+=(const Vector2d &o) {
        x = x + o.x;
        y = y + o.y;
        return *this;
    }

    float dot(const Vector2d &o) const { return x * o.x + y * o.y; }

    float length() const { return std::sqrt(x * x + y * y); }

    Vector2d normalized() const {
        float l = this->length();
        return l > EPS ? Vector2d{x / l, y / l}
        : Vector2d{}; // as in graphics the camera direction is in
                      // normalized way like the unit vector
    }

    static Vector2d lerp(const Vector2d &a, const Vector2d &b, float t) {
        return {lerpf(a.x, b.x, t), lerpf(a.y, b.y, t)};
    }
};

inline Vector2d operator*(float s, const Vector2d &v) { return v * s; }

struct Vector3d {
    float x = 0;
    float y = 0;
    float z = 0;

    Vector3d()=default;

    Vector3d(float x, float y, float z) : x(x), y(y), z(z) {}

    explicit Vector3d(float v) : x(v), y(v), z(v) {}

    Vector3d operator+(const Vector3d &o) const {
        return {x + o.x, y + o.y, z + o.z};
    }

    Vector3d operator-(const Vector3d &o) const {
        return {x - o.x, y - o.y, z - o.z};
    }

    Vector3d operator*(float s) const { return {x * s, y * s, z * s}; }

    Vector3d operator*(const Vector3d &o) const {
        return {x * o.x, y * o.y, z * o.z};
    }

    Vector3d operator/(float s) const { return {x / s, y / s, z / s}; }

    Vector3d operator-() const { return {-x, -y, -z}; }

    Vector3d &operator+=(const Vector3d &o) {
        x = x + o.x;
        y = y + o.y;
        z = z + o.z;
        return *this;
    }

    Vector3d &operator-=(const Vector3d &o) {
        x = x - o.x;
        y = y - o.y;
        z = z - o.z;
        return *this;
    }

    Vector3d &operator*=(const Vector3d &o) {
        x = x * o.x;
        y = y * o.y;
        z = z * o.z;
        return *this;
    }

    float dot(const Vector3d &o) const { return x * o.x + y * o.y + z * o.z; }

    Vector3d cross(const Vector3d &o) const {
        return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x};
    }

    float length() const { return std::sqrt(x * x + y * y + z * z); }

    float lengthSq() const { return x * x + y * y + z * z; }

    Vector3d normalized() const {
        float l = this->length();
        return l > EPS ? Vector3d{x / l, y / l, z / l} : Vector3d{0.0f, 0.0f, 0.0f};
    }

    static Vector3d reflect(const Vector3d &v, const Vector3d &n) {
        return v - n * (2.0f * v.dot(n));
    } // it is basically a reflection formula of physics basically when a light
      // bounces off from a solid surface
    static Vector3d lerp(const Vector3d &a, const Vector3d &b, float t) {
        return {lerpf(a.x, b.x, t), lerpf(a.y, b.y, t), lerpf(a.z, b.z, t)};
    }

    static Vector3d clamp(const Vector3d &v, float low, float high) {
        return {clampf(v.x, low, high), clampf(v.y, low, high),
            clampf(v.z, low, high)};
    }

    std::string str() const {
        std::ostringstream s;
        s << "(" << x << "," << y << "," << z << ")";
        return s.str();
    }
};

inline Vector3d operator*(float s, const Vector3d &v) { return v * s; }

struct Vector4d {
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1;

    Vector4d() = default;

    Vector4d(float x, float y, float z, float w = 1) : x(x), y(y), z(z), w(w) {}

    Vector4d(const Vector3d &v, float w = 1) : x(v.x), y(v.y), z(v.z), w(w) {}

    Vector3d xyz() const { return {x, y, z}; }

    Vector2d xy() const { return {x, y}; }

    Vector3d pdiv() const {
        if (std::abs(w) < EPS)
            return {x, y, z};
        else {
            return {x / w, y / w, z / w};
        }
    }

    Vector4d operator+(const Vector4d &o) const {
        return {x + o.x, y + o.y, z + o.z, w + o.w};
    }

    Vector4d operator-(const Vector4d &o) const {
        return {x - o.x, y - o.y, z - o.z, w - o.w};
    }

    Vector4d operator*(float s) const { return {x * s, y * s, z * s, w * s}; }

    Vector4d &operator+=(const Vector4d &o) {
        x = x + o.x;
        y = y + o.y;
        z = z + o.z;
        w = w + o.w;
        return *this;
    }

    Vector4d &operator*=(float s) {
        x = x * s;
        y = y * s;
        z = z * s;
        w = w * s;
        return *this;
    }

    // linear interpolation
    static Vector4d lerp(const Vector4d &a, const Vector4d &b, float t) {
        return {lerpf(a.x, b.x, t), lerpf(a.y, b.y, t), lerpf(a.z, b.z, t),
            lerpf(a.w, b.w, t)};
    }
};

struct Mat4 {
    float m[4][4] = {};

    static Mat4 identity() {
        Mat4 r;
        r.m[0][0] = r.m[1][1] = r.m[2][2] = r.m[3][3] = 1;
        return r;
    }

    static Mat4 translate(float tx, float ty, float tz) {
        Mat4 r = identity();
        r.m[0][3] = tx;
        r.m[1][3] = ty;
        r.m[2][3] = tz;
        return r;
    }

    static Mat4 translate(const Vector3d &t) { return translate(t.x, t.y, t.z); }
    static Mat4 scale(float sx, float sy, float sz) {
        Mat4 r = identity();
        r.m[0][0] = sx;
        r.m[1][1] = sy;
        r.m[2][2] = sz;
        return r;
    }

    static Mat4 scale(float s) { return scale(s, s, s); }
    static Mat4 scale(const Vector3d &s) { return scale(s.x, s.y, s.z); }
    static Mat4 rotationX(float a) {
        Mat4 r = identity();
        float c = cosf(a), s = sinf(a);
        r.m[1][1] = c;
        r.m[1][2] = -s;
        r.m[2][1] = s;
        r.m[2][2] = c;
        return r;
    }

    static Mat4 rotationY(float a) {
        Mat4 r = identity();
        float c = cosf(a), s = sinf(a);
        r.m[0][0] = c;
        r.m[0][2] = s;
        r.m[2][0] = -s;
        r.m[2][2] = c;
        return r;
    }

    static Mat4 rotationZ(float a) {
        Mat4 r = identity();
        float c = cosf(a), s = sinf(a);
        r.m[0][0] = c;
        r.m[0][1] = -s;
        r.m[1][0] = s;
        r.m[1][1] = c;
        return r;
    }

    static Mat4 rotationAxis(const Vector3d &ax, float a) {
        Vector3d v = ax.normalized();
        float c = cosf(a);
        float s = sinf(a);
        float t = 1 - c;
        Mat4 r = identity();
        r.m[0][0] = t * v.x * v.x + c;
        r.m[0][1] = t * v.x * v.y - s * v.z;
        r.m[0][2] = t * v.x * v.z + s * v.y;
        r.m[1][0] = t * v.x * v.y + s * v.z;
        r.m[1][1] = t * v.y * v.y + c;
        r.m[1][2] = t * v.y * v.z - s * v.x;
        r.m[2][0] = t * v.x * v.z - s * v.y;
        r.m[2][1] = t * v.y * v.z + s * v.x;
        r.m[2][2] = t * v.z * v.z + c;
        return r;
    }

    static Mat4 lookAt(const Vector3d &eye, const Vector3d &target,
            const Vector3d &up) {
        Vector3d f = (target - eye).normalized(), r = f.cross(up).normalized(),
        u = r.cross(f);
        Mat4 m = identity();
        m.m[0][0] = r.x;
        m.m[0][1] = r.y;
        m.m[0][2] = r.z;
        m.m[0][3] = -r.dot(eye);
        m.m[1][0] = u.x;
        m.m[1][1] = u.y;
        m.m[1][2] = u.z;
        m.m[1][3] = -u.dot(eye);
        m.m[2][0] = -f.x;
        m.m[2][1] = -f.y;
        m.m[2][2] = -f.z;
        m.m[2][3] = f.dot(eye);
        return m;
    }

    static Mat4 perspective(float fovy, float aspect, float near, float far) {
        float t = tanf(fovy * 0.5f);
        Mat4 r;
        r.m[0][0] = 1 / (aspect * t);
        r.m[1][1] = 1 / t;
        r.m[2][2] = -(far + near) / (far - near);
        r.m[2][3] = -(2 * far * near) / (far - near);
        r.m[3][2] = -1;
        return r;
    }

    static Mat4 ortho(float l, float r, float b, float t, float n, float f) {
        Mat4 m = identity();
        m.m[0][0] = 2 / (r - 1);
        m.m[1][1] = 2 / (t - b);
        m.m[2][2] = -2 / (f - n);
        m.m[0][3] = -(r + 1) / (r - 1);
        m.m[1][3] = -(t + b) / (t - b);
        m.m[2][3] = -(f + n) / (f - n);
        return m;
    }

    Mat4 operator*(const Mat4 &o) const {
        Mat4 r;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                for (int k = 0; k < 4; k++)
                    r.m[i][j] += m[i][k] * o.m[k][j];
        return r;
    }

    Vector4d operator*(const Vector4d &v) const {
        return {m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
            m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w};
    }

    Vector3d mulDir(const Vector3d &v) const {
        return ((*this) * Vector4d(v, 0)).xyz();
    }

    Vector3d mulPt(const Vector3d &v) const {
        Vector4d r = (*this) * Vector4d(v, 1);
        return std::abs(r.w) > EPS ? r.xyz() * (1 / r.w) : r.xyz();
    }

    Mat4 transposed() const {
        Mat4 r;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                r.m[i][j] = m[j][i];
        return r;
    }
};

struct Color {
    float r = 0;
    float b = 0;
    float g = 0;
    float a = 1;

    Color() = default;
    Color(float r, float b, float g, float a = 1) : r(r), b(b), g(g), a(a) {}

    static Color white() { return {1, 1, 1}; }
    static Color black() { return {0, 0, 0}; }
    static Color red() { return {1, 0, 0}; }
    static Color green() { return {0, 1, 0}; }
    static Color blue() { return {0, 0, 1}; }
    static Color yellow() { return {1, 1, 0}; }  // Red + Green
    static Color cyan() { return {0, 1, 1}; }    // Green + Blue
    static Color magenta() { return {1, 0, 1}; } // Red + Blue
    static Color orange() { return {1, 0.5f, 0}; }
    static Color grey(float v = 0.5f) { return {v, v, v}; }

    Color operator+(const Color &o) const {
        return {r + o.r, g + o.g, b + o.b, a};
    }

    Color operator*(float s) const { return {r * s, g * s, b * s, a * s}; }

    Color operator*(const Color &o) const {
        return {r * o.r, g * o.g, b * o.b, a * o.a};
    }

    Color &operator+=(const Color &o) {
        r = r + o.r;
        g = g + o.g;
        b = b + o.b;
        return *this;
    }

    Color clamped() const {
        return {clampf(r, 0, 1), clampf(g, 0, 1), clampf(b, 0, 1), clampf(a, 0, 1)};
    }

    static Color lerp(const Color &a, const Color &b, float t) {
        return {lerpf(a.r, b.r, t), lerpf(a.g, b.g, t), lerpf(a.b, b.b, t),
            lerpf(a.a, b.a, t)};
    }

    uint32_t pack() const {
        Color c = clamped();
        return ((uint8_t)(c.a * 255) << 24) | ((uint8_t)(c.r * 255) << 16) |
            ((uint8_t)(c.g * 255) << 8) | (uint8_t)(c.b * 255);
    }

    static Color fromVector3d(const Vector3d &v) { return {v.x, v.y, v.z}; }

    Vector3d toVector3d() const { return {r, g, b}; }
};

inline Color operator*(float s, const Color &c) { return c * s; }
