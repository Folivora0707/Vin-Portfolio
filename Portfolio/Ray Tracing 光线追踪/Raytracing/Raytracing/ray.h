#pragma once

// 使用 #ifndef 与 #endif 防止一个源文件两次包含同一个头文件
#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// 光线类，主要是描述光线的首尾
class ray {
public:
	// 构造函数
	ray() {}
	ray(const vec3& origin, const vec3& direction)
		: orig(origin), dir(direction)
	{}

	vec3 origin() const { return orig; }     // 首坐标
	vec3 direction() const { return dir; }   // 尾坐标

	// 利用常数t，沿光线移动并返回坐标值
	// t可以视为光线射出后经历的时间
	vec3 at(double t) const {
		return orig + t * dir;
	}

public:
	vec3 orig;
	vec3 dir;
};

#endif
