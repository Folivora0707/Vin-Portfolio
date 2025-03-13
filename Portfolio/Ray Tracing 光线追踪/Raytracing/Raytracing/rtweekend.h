#pragma once

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>   // ʹ��rand()�������
#include <limits>
#include <memory>

// ���ó����ͺ�������
// Usings

using std::shared_ptr;
using std::make_shared;

// Constants

const double infinity = std::numeric_limits<double>::infinity();   // double���������
const double pi = 3.1415926535897932385;                           // Բ����

// Utility Functions

// �Ƕ�ת��Ϊ����
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180;
}

inline double ffmin(double a, double b) { return a <= b ? a : b; } // ��Сֵ
inline double ffmax(double a, double b) { return a >= b ? a : b; } // �ϴ�ֵ

// ȡ[0,1)֮�������
inline double random_double() {
	// Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0);
}   // +1.0����ӱ�ȻС�ڷ�ĸ�������������Ȼ < 1

// ȡ[min,max)֮�������
inline double random_double(double min, double max) {
	// Returns a random real in [min,max).
	return min + (max - min)*random_double();
}

// �淶Խ������
inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

// Common Headers
// �������ʹ��Щͷ�ļ�Ҳ����ʹ����������

#include "ray.h"   // ������
#include "vec3.h"  // ��ά������

#endif
