#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

// Schlick����
double schlick(double cosine, double ref_idx) {
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}

class material {
public:
	// �麯������������
	// ��������Ϊ��������ߡ��ཻ�㡢����˥����ɢ�����
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
	) const = 0;
};

// �����ز���
class lambertian : public material {
public:
	// ���캯������Ϊ����˥��
	lambertian(const vec3& a) : albedo(a) {}

	// �����ز��ʵ�ɢ��Ϊ����������
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
	) const {
		vec3 scatter_direction = rec.normal + random_unit_vector();
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

public:
	vec3 albedo;
};

// ��������
class metal : public material {
public:
	// ���캯��
	// metal(const vec3& a) : albedo(a) {}
	// fuzzΪë�ڳ̶�
	metal(const vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	// �������ʵ�ɢ��Ϊ���淴��
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
	) const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		// scattered = ray(rec.p, reflected);
		// ���淴��������ƫ����������ë����Ч��
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;

		// �������0�������н�Ϊ���
		// ɢ������뷨�߼нǲ������Ƕ۽�
		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	vec3 albedo; // ����˥��
	double fuzz; // ë�ڳ̶�
};

// ͸������
class dielectric : public material {
public:
	// ���캯��
	dielectric(double ri) : ref_idx(ri) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
	) const {
		// Ĭ�ϲ�˥��
		attenuation = vec3(1.0, 1.0, 1.0);
		double etai_over_etat;

		// �����ڲ�����Ĺ������������ȡ����
		if (rec.front_face) {
			etai_over_etat = 1.0 / ref_idx;
		}
		else {
			etai_over_etat = ref_idx;
		}

		vec3 unit_direction = unit_vector(r_in.direction());                  // ��λ��

		// ����ǵ�����ֵ�����ܴ���1
		// ���ɸ������ʽ�������������ʽ��ʣ�������˹��������
		// ����ΪӦ�÷����������������
		double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);      // ����cos 
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);                 // ����sin
		if (etai_over_etat * sin_theta > 1.0) {            // ��˹���������Ƶ����������
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}

		// Ӧ��Schlick���ƣ����첣������Ƕȱ仯�ķ���Ч��
		double reflect_prob = schlick(cos_theta, etai_over_etat);
		// Schlick����ֵ�����������淴��ĸ���
		if (random_double() < reflect_prob)
		{
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}

		vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat); // ��������
		scattered = ray(rec.p, refracted);
		return true;
	}

	double ref_idx; // ���������
};

#endif