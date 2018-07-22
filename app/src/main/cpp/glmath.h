#pragma once

#include <math.h>

struct vec2 {
	float x, y;

	constexpr vec2() : x(0), y(0) {}
    constexpr vec2(float x, float y) : x(x), y(y) {}

	float magnitude() {
		return sqrtf(sqr_magnitide());
	}

	float sqr_magnitide() {
		return dot(*this);
	}

	vec2 normalize() {
		float l = magnitude();
		return l != 0 ? (*this * (1.0f / l)) : vec2();
	}

	float dot(const vec2& other) {
		return x * other.x + y * other.y;
	}

	const float& operator[] (int index) const {
		return (&x)[index];
	}

	float& operator[] (int index) {
		return (&x)[index];
	}

	vec2 operator-() const {
		return { -x, -y };
	}

	vec2 operator-(const vec2& other) const {
		return { x - other.x, y - other.y };
	}

    vec2 operator+(const vec2& other) const {
        return { x + other.x, y + other.y };
    }

    vec2& operator+=(const vec2& other) {
        return *this = operator+(other);
    }

	vec2 operator*(const float &other) const {
		return vec2(x * other, y * other);
	}

	friend vec2 operator*(const float &other, const vec2 &b) {
		return b * other;
	}

	vec2 operator/(const float &other) {
		return operator*(1.0f / other);
	}
};

struct ivec2 {
	int x, y;

	constexpr ivec2() : x(0), y(0) {}
    constexpr ivec2(int x, int y) : x(x), y(y) {}

	int dot(const ivec2& other) {
		return x * other.x + y * other.y;
	}

	const int& operator[] (int index) const {
		return (&x)[index];
	}

	int& operator[] (int index) {
		return (&x)[index];
	}

	ivec2 operator-() const {
		return { -x, -y };
	}

	ivec2 operator-(const ivec2& other) const {
		return { x - other.x, y - other.y };
	}

	ivec2 operator+(const ivec2& other) const {
		return { x + other.x, y + other.y };
	}

	ivec2 operator*(const int &other) const {
		return ivec2(x * other, y * other);
	}

	friend ivec2 operator*(const int &other, const ivec2 &b) {
		return b * other;
	}

	bool operator==(const ivec2& other) const {
		return (x == other.x) && (y == other.y);
	}

	bool operator!=(const ivec2& other) const {
		return (x != other.x) || (y != other.y);
	}
};

struct vec3 {
    float x, y, z;

    constexpr vec3() : x(0), y(0), z(0) {}
    constexpr vec3(const vec2& v, float z) : x(v.x), y(v.y), z(z) {}
    constexpr vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    float magnitude() {
        return sqrtf(sqr_magnitide());
    }

    float sqr_magnitide() {
        return dot(*this);
    }

    vec3 normalize() {
        float l = magnitude();
        return l != 0 ? (*this * (1.0f / l)) : vec3();
    }

    float dot(const vec3& other) {
        return x * other.x + y * other.y + z * other.z;
    }

    vec3 cross(const vec3& other) {
        return {
                y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x
        };
    }

    const float& operator[] (int index) const {
        return (&x)[index];
    }

    float& operator[] (int index) {
        return (&x)[index];
    }

    vec3 operator-() const {
        return vec3(-x, -y, -z);
    }

    vec3 operator-(const vec3& other) const {
        return vec3(x - other.x, y - other.y, z - other.z);
    }

    vec3 operator+(const vec3& other) const {
        return vec3(x + other.x, y + other.y, z + other.z);
    }

    vec3 operator*(const float &other) const {
        return vec3(x * other, y * other, z * other);
    }

    friend vec3 operator*(const float &other, const vec3 &b) {
        return b * other;
    }

    vec3 operator/(const float &other) {
        return operator*(1.0f / other);
    }
};

struct ivec3 {
    int x, y, z;

    constexpr ivec3() : x(0), y(0), z(0) {}
    constexpr ivec3(const ivec2& v, int z) : x(v.x), y(v.y), z(z) {}
    constexpr ivec3(int x, int y, int z) : x(x), y(y), z(z) {}

    int dot(const ivec3& other) {
        return x * other.x + y * other.y + z * other.z;
    }

    ivec3 cross(const ivec3& other) {
        return {
            y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x
        };
    }

    const int& operator[] (int index) const {
        return (&x)[index];
    }

    int& operator[] (int index) {
        return (&x)[index];
    }

    ivec3 operator-() const {
        return ivec3(-x, -y, -z);
    }

    ivec3 operator-(const ivec3& other) const {
        return ivec3(x - other.x, y - other.y, z - other.z);
    }

    ivec3 operator+(const ivec3& other) const {
        return ivec3(x + other.x, y + other.y, z + other.z);
    }

    ivec3 operator*(const int &other) const {
        return ivec3(x * other, y * other, z * other);
    }

    friend ivec3 operator*(const int &other, const ivec3 &b) {
        return b * other;
    }
};

struct vec4 {
    float x, y, z, w;

    constexpr vec4() : x(0), y(0), z(0), w(0) {}
    constexpr vec4(const vec3& v, float w = 0) : x(v.x), y(v.y), z(v.z), w(w) {}
    constexpr vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    float magnitude() {
        return sqrtf(sqr_magnitide());
    }

    float sqr_magnitide() {
        return dot(*this);
    }

    vec4 normalize() {
        float l = magnitude();
        return l != 0 ? (*this * (1.0f / l)) : vec4();
    }

    float dot(const vec4& other) {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    const float& operator[] (int index) const {
        return (&x)[index];
    }

    float& operator[] (int index) {
        return (&x)[index];
    }

    vec4 operator-() const {
        return vec4(-x, -y, -z, -w);
    }

    vec4 operator-(const vec4& other) const {
        return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    vec4 operator+(const vec4& other) const {
        return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    vec4 operator*(const float &other) const {
        return vec4(x * other, y * other, z * other, w * other);
    }

    friend vec4 operator*(const float &other, const vec4 &b) {
        return b * other;
    }

    vec4 operator/(const float &other) const {
        return operator*(1.0f / other);
    }
};

struct ivec4 {
    int x, y, z, w;

    constexpr ivec4() : x(0), y(0), z(0), w(0) {}
    constexpr ivec4(const ivec3& v, int w = 0) : x(v.x), y(v.y), z(v.z), w(w) {}
    constexpr ivec4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}

    int dot(const ivec4& other) {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    const int& operator[] (int index) const {
        return (&x)[index];
    }

    int& operator[] (int index) {
        return (&x)[index];
    }

    ivec4 operator-() const {
        return ivec4(-x, -y, -z, -w);
    }

    ivec4 operator-(const ivec4& other) const {
        return ivec4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    ivec4 operator+(const ivec4& other) const {
        return ivec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    ivec4 operator*(const int &other) const {
        return ivec4(x * other, y * other, z * other, w * other);
    }

    friend vec4 operator*(const int &other, const vec4 &b) {
        return b * other;
    }
};

struct mat3x3 {
	vec3 x, y, z;

	constexpr mat3x3() : x(1, 0, 0), y(0, 1, 0), z(0, 0, 1) {}
    constexpr mat3x3(const vec3& x, const vec3& y, const vec3& z) : x(x), y(y), z(z) {}

	const vec3& operator[](int index) const {
		return (&x)[index];
	}

	vec3& operator[](int index) {
		return (&x)[index];
	}

	friend vec3 operator*(const vec3& v, const mat3x3& m) {
		return {
				v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2],
				v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2],
				v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m[2][2]
		};
	}

	friend vec3 operator*(const mat3x3& m, const vec3& v) {
		return {
				m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2],
				m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2],
				m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2]
		};
	}
};

struct mat4x4 {
	vec4 x, y, z, w;

	constexpr mat4x4() : x(1, 0, 0, 0), y(0, 1, 0, 0), z(0, 0, 1, 0), w(0, 0, 0, 1) {}
    constexpr mat4x4(const mat3x3 &m, vec4 w = vec4(0, 0, 0, 1)) : x(m.x, 0), y(m.y, 0), z(m.z, 0), w(w) {}
    constexpr mat4x4(const vec4& x, const vec4& y, const vec4& z, const vec4& w) : x(x), y(y), z(z), w(w) {}

	const vec4& operator[](int index) const {
		return (&x)[index];
	}

	vec4& operator[](int index) {
		return (&x)[index];
	}

	static mat4x4 translate(float x, float y, float z) {
		return {
				{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 1, 0},
				{x, y, z, 1}
		};
	}

	static mat4x4 translate(const vec3 &position) {
		return {
				{ 1, 0, 0, 0},
				{ 0, 1, 0, 0},
				{ 0, 0, 1, 0},
				{position, 1}
		};
	}

	static mat4x4 rotate(float y, float p, float r) {
		float cy = cosf(y);
		float sy = sinf(y);

		float cp = cosf(p);
		float sp = sinf(p);

		float cr = cosf(r);
		float sr = sinf(r);

		mat4x4 rotateX = {
				{1,  0,   0, 0},
				{0, cy, -sy, 0},
				{0, sy,  cy, 0},
				{0,  0,   0, 1}
		};

		mat4x4 rotateY = {
				{ cp, 0, sp, 0},
				{  0, 1,  0, 0},
				{-sp, 0, cp, 0},
				{  0, 0,  0, 1}
		};

		mat4x4 rotateZ = {
				{cr, -sr, 0, 0},
				{sr,  cr, 0, 0},
				{ 0,   0, 1, 0},
				{ 0,   0, 0, 1}
		};

		return rotateZ * rotateY * rotateX;
	}

	static mat4x4 scale(float x, float y, float z) {
		return {
				{x, 0, 0, 0},
				{0, y, 0, 0},
				{0, 0, z, 0},
				{0, 0, 0, 1}
		};
	}

	static mat4x4 scale(const vec3 &scale) {
		return mat4x4::scale(scale.x, scale.y, scale.z);
	}

	friend vec4 operator*(const vec4& a, const mat4x4& b) {
		return {
				a[0] * b[0][0] + a[1] * b[0][1] + a[2] * b[0][2] + a[3] * b[0][3],
				a[0] * b[1][0] + a[1] * b[1][1] + a[2] * b[1][2] + a[3] * b[1][3],
				a[0] * b[2][0] + a[1] * b[2][1] + a[2] * b[2][2] + a[3] * b[2][3],
				a[0] * b[3][0] + a[1] * b[3][1] + a[2] * b[3][2] + a[3] * b[3][3]
		};
	}

	friend vec4 operator*(const mat4x4& a, const vec4& b) {
		return {
				a[0][0] * b[0] + a[1][0] * b[1] + a[2][0] * b[2] + a[3][0] * b[3],
				a[0][1] * b[0] + a[1][1] * b[1] + a[2][1] * b[2] + a[3][1] * b[3],
				a[0][2] * b[0] + a[1][2] * b[1] + a[2][2] * b[2] + a[3][2] * b[3],
				a[0][3] * b[0] + a[1][3] * b[1] + a[2][3] * b[2] + a[3][3] * b[3]
		};
	}

	friend mat4x4 operator*(const mat4x4& a, const mat4x4& b) {
		return { a * b[0], a * b[1], a * b[2], a * b[3] };
	}

	static mat4x4 lookat(const vec3& position, const vec3& target) {
		vec3 d = (target - position).normalize();

		return rotate(atan2(d.x, d.y), asin(d.y), 0) * mat4x4(
				vec4(1, 0, 0, 0),
				vec4(0, 1, 0, 0),
				vec4(0, 0, 1, 0),
				vec4(-position, 1)
		);
	}

	static mat4x4 orthographic(float size, float aspect, float near, float far) {
		float f = 2.0f / size;
		float t = f / aspect;
		float a = 2.0f * far * near / (near - far);
		float b = (near + far) / (near - far);

		return {
				{ t, 0, 0, 0 },
				{ 0, f, 0, 0 },
				{ 0, 0, a, b },
				{ 0, 0, 0, 1 }
		};
	}

	static mat4x4 perspective(float fov, float aspect, float near, float far) {
		float f = 1.0f / float(tan(fov * M_PI / 360.0));
		float t = f / aspect;
		float a = (near + far) / (near - far);
		float b = 2.0f * far / (near - far);

		return {
			{t, 0,  0, 0},
			{0, f,  0, 0},
			{0, 0,  a, b},
			{0, 0, -1, 0}
		};
	}
};