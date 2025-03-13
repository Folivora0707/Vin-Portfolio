#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

// 球
class sphere : public hittable {
public:
	// 构造函数
	sphere() {}
	// sphere(vec3 cen, double r) : center(cen), radius(r) {};
	sphere(vec3 cen, double r, shared_ptr<material> m)
		: center(cen), radius(r), mat_ptr(m) {};

	// 限定t的范围看似没有必要，但在某些情况下会很方便
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

public:
	vec3 center;                  // 球心
	double radius;                // 半径
	shared_ptr<material> mat_ptr; // 材质
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	// 判断是否相交（t是否有解）
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	// t > 0，相交，进一步判断是否渲染，并记录相交点信息
	if (discriminant > 0) {
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;      // t的较小解，向阳面
		if (temp < t_max && temp > t_min) {
			rec.t = temp;          // 记录相交处光线的t值
			rec.p = r.at(rec.t);   // 记录相交点位置

			// 球心指向相交点即为外法线
			// rec.normal = (rec.p - center) / radius;
			vec3 outward_normal = (rec.p - center) / radius;
			// 记录法线为可视方向
			rec.set_face_normal(r, outward_normal);

			rec.mat_ptr = mat_ptr; // 记录材质

			return true;
		}

		// 背光面也要计算，透明材质等效果需要 
		temp = (-half_b + root) / a;           // t的较大解，背光面
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.at(rec.t);

			// rec.normal = (rec.p - center) / radius;
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);

			rec.mat_ptr = mat_ptr; // 记录材质

			return true;
		}
	}
	return false;
}


#endif
