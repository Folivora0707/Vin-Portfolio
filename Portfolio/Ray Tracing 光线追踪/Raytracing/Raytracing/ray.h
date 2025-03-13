#pragma once

// ʹ�� #ifndef �� #endif ��ֹһ��Դ�ļ����ΰ���ͬһ��ͷ�ļ�
#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// �����࣬��Ҫ���������ߵ���β
class ray {
public:
	// ���캯��
	ray() {}
	ray(const vec3& origin, const vec3& direction)
		: orig(origin), dir(direction)
	{}

	vec3 origin() const { return orig; }     // ������
	vec3 direction() const { return dir; }   // β����

	// ���ó���t���ع����ƶ�����������ֵ
	// t������Ϊ�������������ʱ��
	vec3 at(double t) const {
		return orig + t * dir;
	}

public:
	vec3 orig;
	vec3 dir;
};

#endif
