#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "ray.h"

class material;

// �ཻ��Ľṹ����
struct hit_record {
	vec3 p;       // �ཻ��λ��
	vec3 normal;  // ����

	// ����ָ�뼯
	shared_ptr<material> mat_ptr;

	double t;         // �ཻ����ߵ�tֵ
	bool front_face;  // �Ƿ�������

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		// ��˽��С��0�������뷨�߼н�Ϊ�۽�
		// ӵ����Щ���ߵ���ǡ���������ܿ����ģ�Ӧ����Ⱦ
		front_face = dot(r.direction(), outward_normal) < 0;

		// ���߸���Ϊ���ӷ���
		normal = front_face ? outward_normal : -outward_normal;
	}
};

// ����һ�������ײ�ĳ�����
class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif
