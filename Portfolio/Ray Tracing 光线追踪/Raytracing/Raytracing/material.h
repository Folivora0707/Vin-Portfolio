#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

// Schlick近似
double schlick(double cosine, double ref_idx) {
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}

class material {
public:
	// 虚函数，方便重载
	// 参数依次为：入射光线、相交点、光线衰减、散射光线
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
	) const = 0;
};

// 兰伯特材质
class lambertian : public material {
public:
	// 构造函数参数为光能衰减
	lambertian(const vec3& a) : albedo(a) {}

	// 兰伯特材质的散射为理想漫反射
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

// 金属材质
class metal : public material {
public:
	// 构造函数
	// metal(const vec3& a) : albedo(a) {}
	// fuzz为毛糙程度
	metal(const vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	// 金属材质的散射为镜面反射
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
	) const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		// scattered = ray(rec.p, reflected);
		// 镜面反射加上随机偏移量创造绒毛金属效果
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;

		// 点积大于0，向量夹角为锐角
		// 散射光线与法线夹角不可能是钝角
		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	vec3 albedo; // 光能衰减
	double fuzz; // 毛糙程度
};

// 透明材质
class dielectric : public material {
public:
	// 构造函数
	dielectric(double ri) : ref_idx(ri) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
	) const {
		// 默认不衰减
		attenuation = vec3(1.0, 1.0, 1.0);
		double etai_over_etat;

		// 介质内部射出的光线相对折射率取倒数
		if (rec.front_face) {
			etai_over_etat = 1.0 / ref_idx;
		}
		else {
			etai_over_etat = ref_idx;
		}

		vec3 unit_direction = unit_vector(r_in.direction());                  // 单位化

		// 折射角的正弦值不可能大于1
		// 若由高折射率介质射入低折射率介质，不符合斯涅尔定律
		// 则认为应该发生反射而不是折射
		double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);      // 计算cos 
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);                 // 计算sin
		if (etai_over_etat * sin_theta > 1.0) {            // 由斯涅尔定律推导折射角正弦
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}

		// 应用Schlick近似，创造玻璃上随角度变化的反射效果
		double reflect_prob = schlick(cos_theta, etai_over_etat);
		// Schlick近似值决定发生镜面反射的概率
		if (random_double() < reflect_prob)
		{
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}

		vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat); // 计算折射
		scattered = ray(rec.p, refracted);
		return true;
	}

	double ref_idx; // 相对折射率
};

#endif