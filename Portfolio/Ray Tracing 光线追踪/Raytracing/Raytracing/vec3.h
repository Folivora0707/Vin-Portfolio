#pragma once       // ��ֹ�ض���

#ifndef VEC3_H
#define VEC3_H

#include <iostream>

// ��ά�����࣬��������������ɫ��λ�á�����ƫ������
class vec3 {
public:
	// ���캯��
	vec3() : e{ 0,0,0 } {}
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	// ʹ������doubleֵ����
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	// ���ؼ���
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	
	// ����������
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	// ���ؼӺ�
	vec3& operator+=(const vec3 &v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	// ���س˺�
	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	// ���س���
	vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	// ��ԭ���ŷ�Ͼ��룬����˵������ģ
	double length() const {
		return sqrt(length_squared());
	}
	// ��ԭ��ľ����ƽ������ֵƽ����
	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	// ����ά����ֵת��ΪRGBֵ���������ֵȡֵ��Χ����[0,1]
	void write_color(std::ostream &out, int samples_per_pixel) {
		/*
		// Divide the color total by the number of samples.
		// samples_per_pixel�������������������ƽ��ֵ������ÿ����Ч��
		auto scale = 1.0 / samples_per_pixel;
		auto r = scale * e[0];
		auto g = scale * e[1];
		auto b = scale * e[2];
		*/
		// Divide the color total by the number of samples and gamma-correct
        // for a gamma value of 2.0.
		// ��٤��У����������������ȡ1/2�����ǿ�ƽ����
		auto scale = 1.0 / samples_per_pixel;
		auto r = sqrt(scale * e[0]);
		auto g = sqrt(scale * e[1]);
		auto b = sqrt(scale * e[2]);

		// Write the translated [0,255] value of each color component.
		out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
	}

	// �����������������ֵ����[0,1)֮��
	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}
	// �����������������ֵ����[min,max)֮��
	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

public:
	double e[3];
};

// ���ܶ���
// inline������������������Ƶ��ʹ�ô�������ջ�ռ�
// �������
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// ���ؼӷ�
inline vec3 operator+(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// ���ؼ���
inline vec3 operator-(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// ���س˷�
inline vec3 operator*(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}   // ���� * ����

inline vec3 operator*(double t, const vec3 &v) {
	return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}   // ���� * ����

inline vec3 operator*(const vec3 &v, double t) {
	return t * v;
}   // ���� * ����

// ���س���
inline vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}

// ��ά������˽������������
inline double dot(const vec3 &u, const vec3 &v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

// ��ά������˽������������
inline vec3 cross(const vec3 &u, const vec3 &v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// ��ά�����ĵ�λ����
inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

// ��λԲ��ȡ�������
vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1, 1);           // �߳�Ϊ2������������ȡ�������
		if (p.length_squared() >= 1) continue;  // �����������ģ >= 1 ������ѡȡ
		return p;
	}
}

// �ʲ��ֲ������䣨����ɢ�䣩
vec3 random_unit_vector() {
	// ��λԲ����ȡ�������
	auto a = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	return vec3(r*cos(a), r*sin(a), z);
}
// ����������䷽�������򣨷��ߣ�����
vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	// ֻѡȡ�뷨��ͬ��������ڵ�����
	if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

// ���㷴����ߣ�v������⣬n�Ƿ���
vec3 reflect(const vec3& v, const vec3& n) {
	// �������������壬v��n�����ϵ�ͶӰ
	return v - 2 * dot(v, n)*n;
}

// ����������ߣ�uv��������ߣ�n�Ƿ��ߣ�Ӧ����˹��������
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	auto cos_theta = dot(-uv, n);          // ����ǵ�cosֵ
	// etai_over_etat�����������������������������ʵı�ֵ
	vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);        // ������ƽ������
	vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n; // �����Ĵ�ֱ����
	return r_out_parallel + r_out_perp;    // �������
}

// ��λԲ��ѡȡ�����
vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

#endif