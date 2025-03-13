#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "ray.h"

class material;

// 相交点的结构定义
struct hit_record {
	vec3 p;       // 相交点位置
	vec3 normal;  // 法线

	// 材质指针集
	shared_ptr<material> mat_ptr;

	double t;         // 相交点光线的t值
	bool front_face;  // 是否是正面

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		// 点乘结果小于0，光线与法线夹角为钝角
		// 拥有这些法线的面恰好是我们能看到的，应该渲染
		front_face = dot(r.direction(), outward_normal) < 0;

		// 法线更新为可视方向
		normal = front_face ? outward_normal : -outward_normal;
	}
};

// 定义一个检测碰撞的抽象类
class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif
