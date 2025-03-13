#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

// ��
class sphere : public hittable {
public:
	// ���캯��
	sphere() {}
	// sphere(vec3 cen, double r) : center(cen), radius(r) {};
	sphere(vec3 cen, double r, shared_ptr<material> m)
		: center(cen), radius(r), mat_ptr(m) {};

	// �޶�t�ķ�Χ����û�б�Ҫ������ĳЩ����»�ܷ���
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

public:
	vec3 center;                  // ����
	double radius;                // �뾶
	shared_ptr<material> mat_ptr; // ����
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	// �ж��Ƿ��ཻ��t�Ƿ��н⣩
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	// t > 0���ཻ����һ���ж��Ƿ���Ⱦ������¼�ཻ����Ϣ
	if (discriminant > 0) {
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;      // t�Ľ�С�⣬������
		if (temp < t_max && temp > t_min) {
			rec.t = temp;          // ��¼�ཻ�����ߵ�tֵ
			rec.p = r.at(rec.t);   // ��¼�ཻ��λ��

			// ����ָ���ཻ�㼴Ϊ�ⷨ��
			// rec.normal = (rec.p - center) / radius;
			vec3 outward_normal = (rec.p - center) / radius;
			// ��¼����Ϊ���ӷ���
			rec.set_face_normal(r, outward_normal);

			rec.mat_ptr = mat_ptr; // ��¼����

			return true;
		}

		// ������ҲҪ���㣬͸�����ʵ�Ч����Ҫ 
		temp = (-half_b + root) / a;           // t�Ľϴ�⣬������
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.at(rec.t);

			// rec.normal = (rec.p - center) / radius;
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);

			rec.mat_ptr = mat_ptr; // ��¼����

			return true;
		}
	}
	return false;
}


#endif
