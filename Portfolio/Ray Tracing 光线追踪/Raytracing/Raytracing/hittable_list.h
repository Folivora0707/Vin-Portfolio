#pragma once

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>      // 使用shared指针
#include <vector>

using std::shared_ptr;
using std::make_shared;

// 场景清单
class hittable_list : public hittable {
public:
	// 构造函数
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }

	// 场景清零
	void clear() { objects.clear(); }
	// 添加场景
	void add(shared_ptr<hittable> object) { objects.push_back(object); }

	// 光线碰撞检测
	// const意味着函数体内不能对成员数据进行任何改动
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

public:
	// 指向hittable类型对象的指针的集合
	std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	// 遍历场景对象，任一对象存在与光线的碰撞则返回真
	// rec存储距离光源最近的相交点
	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

#endif
