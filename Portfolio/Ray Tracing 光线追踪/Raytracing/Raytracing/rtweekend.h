#pragma once

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>   // 使用rand()随机函数
#include <limits>
#include <memory>

// 常用常量和函数定义
// Usings

using std::shared_ptr;
using std::make_shared;

// Constants

const double infinity = std::numeric_limits<double>::infinity();   // double类型无穷大
const double pi = 3.1415926535897932385;                           // 圆周率

// Utility Functions

// 角度转换为弧度
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180;
}

inline double ffmin(double a, double b) { return a <= b ? a : b; } // 较小值
inline double ffmax(double a, double b) { return a >= b ? a : b; } // 较大值

// 取[0,1)之间随机数
inline double random_double() {
	// Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0);
}   // +1.0后分子必然小于分母，这样随机数必然 < 1

// 取[min,max)之间随机数
inline double random_double(double min, double max) {
	// Returns a random real in [min,max).
	return min + (max - min)*random_double();
}

// 规范越界数据
inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

// Common Headers
// 放在最后，使这些头文件也可以使用上述方法

#include "ray.h"   // 光线类
#include "vec3.h"  // 三维向量类

#endif
