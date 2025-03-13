#pragma once       // 防止重定义

#ifndef VEC3_H
#define VEC3_H

#include <iostream>

// 三维向量类，可以用来定义颜色、位置、方向、偏移量等
class vec3 {
public:
	// 构造函数
	vec3() : e{ 0,0,0 } {}
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	// 使用三个double值描述
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	// 重载减号
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	
	// 重载中括号
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	// 重载加号
	vec3& operator+=(const vec3 &v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	// 重载乘号
	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	// 重载除号
	vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	// 与原点的欧氏距离，或者说向量的模
	double length() const {
		return sqrt(length_squared());
	}
	// 与原点的距离的平方，三值平方和
	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	// 将三维向量值转化为RGB值输出，三个值取值范围都是[0,1]
	void write_color(std::ostream &out, int samples_per_pixel) {
		/*
		// Divide the color total by the number of samples.
		// samples_per_pixel，采样次数，除法求得平均值，以求得抗锯齿效果
		auto scale = 1.0 / samples_per_pixel;
		auto r = scale * e[0];
		auto g = scale * e[1];
		auto b = scale * e[2];
		*/
		// Divide the color total by the number of samples and gamma-correct
        // for a gamma value of 2.0.
		// 简单伽马校正提亮场景，参数取1/2，就是开平方根
		auto scale = 1.0 / samples_per_pixel;
		auto r = sqrt(scale * e[0]);
		auto g = sqrt(scale * e[1]);
		auto b = sqrt(scale * e[2]);

		// Write the translated [0,255] value of each color component.
		out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
	}

	// 创建随机向量，坐标值均在[0,1)之间
	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}
	// 创建随机向量，坐标值均在[min,max)之间
	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

public:
	double e[3];
};

// 功能定义
// inline定义内联函数，避免频繁使用大量消耗栈空间
// 重载输出
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// 重载加法
inline vec3 operator+(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// 重载减法
inline vec3 operator-(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// 重载乘法
inline vec3 operator*(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}   // 向量 * 向量

inline vec3 operator*(double t, const vec3 &v) {
	return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}   // 常数 * 向量

inline vec3 operator*(const vec3 &v, double t) {
	return t * v;
}   // 向量 * 常数

// 重载除法
inline vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}

// 三维向量点乘结果，即数量积
inline double dot(const vec3 &u, const vec3 &v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

// 三维向量叉乘结果，即向量积
inline vec3 cross(const vec3 &u, const vec3 &v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// 三维向量的单位向量
inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

// 单位圆内取随机向量
vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1, 1);           // 边长为2的立方体域内取随机向量
		if (p.length_squared() >= 1) continue;  // 若随机向量的模 >= 1 则重新选取
		return p;
	}
}

// 朗伯分布漫反射（理想散射）
vec3 random_unit_vector() {
	// 单位圆表面取随机向量
	auto a = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	return vec3(r*cos(a), r*sin(a), z);
}
// 最初的漫反射方法，半球（法线）反射
vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	// 只选取与法线同向半球域内的向量
	if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

// 计算反射光线，v是入射光，n是法线
vec3 reflect(const vec3& v, const vec3& n) {
	// 数量积几何意义，v在n方向上的投影
	return v - 2 * dot(v, n)*n;
}

// 计算折射光线，uv是入射光线，n是法线，应用了斯涅尔定律
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	auto cos_theta = dot(-uv, n);          // 入射角的cos值
	// etai_over_etat是入射介质折射率与折射介质折射率的比值
	vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);        // 入射光的平行向量
	vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n; // 入射光的垂直向量
	return r_out_parallel + r_out_perp;    // 折射光线
}

// 单位圆内选取随机点
vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

#endif