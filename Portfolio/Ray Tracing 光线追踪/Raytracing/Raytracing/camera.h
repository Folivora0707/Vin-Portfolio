#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

// �������
class camera {
public:
	/*
	// �����ʼ���
	camera() {
		lower_left_corner = vec3(-2.0, -1.0, -1.0);  
		horizontal = vec3(4.0, 0.0, 0.0);               
		vertical = vec3(0.0, 2.0, 0.0);              
		origin = vec3(0.0, 0.0, 0.0);                
	}
	*/
	// �������
	camera(
		vec3 lookfrom, vec3 lookat, // ȷ����ͷ����
		vec3 vup,         // ���������
		double vfov,      // top to bottom, in degrees���ӽǶ���
		double aspect,    // ��Ұ��߱�
		double aperture,  // ��Ȧ����ͷֱ��
		double focus_dist // ����
	) {
		// origin = vec3(0.0, 0.0, 0.0);
		origin = lookfrom;          // ���λ����
		lens_radius = aperture / 2; // ��ͷ�뾶

		// vec3 u, v, w;

		auto theta = degrees_to_radians(vfov);  // ����ת����
		auto half_height = tan(theta / 2);      // ��ֱ������һ��
		auto half_width = aspect * half_height; // ˮƽ������һ��

		// lower_left_corner = vec3(-half_width, -half_height, -1.0);
		// horizontal = vec3(2 * half_width, 0.0, 0.0);
		// vertical = vec3(0.0, 2 * half_height, 0.0);

		w = unit_vector(lookfrom - lookat);     // ��ͷ��ǰָ��
		u = unit_vector(cross(vup, w));         
		v = cross(w, u);

		lower_left_corner = origin
			- half_width * focus_dist * u
			- half_height * focus_dist * v
			- focus_dist * w;

		horizontal = 2 * half_width * focus_dist * u;
		vertical = 2 * half_height * focus_dist * v;

		/*
		lower_left_corner = origin - half_width * u - half_height * v - w;

		horizontal = 2 * half_width*u;
		vertical = 2 * half_height*v;
		*/
	}

	// s����Ļ�������Ȩֵ��t����Ļ�������Ȩֵ
	ray get_ray(double s, double t) {

		// return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
		
		vec3 rd = lens_radius * random_in_unit_disk();
		// ��Դ��x,y�������һ�����ƫ��
		vec3 offset = u * rd.x() + v * rd.y();

		return ray(
			origin + offset,
			lower_left_corner + s * horizontal + t * vertical - origin - offset
		);
	}

public:
	vec3 origin;              // ��Դλ��
	vec3 lower_left_corner;   // ��׼��Ļ���½ǣ����յ�
	vec3 horizontal;          // X�ᣬˮƽ������Ļ��Ļ�׼
	vec3 vertical;            // Y�ᣬ��ֱ������Ļ�ߵĻ�׼
	vec3 u, v, w;
	double lens_radius;       // ��ͷ�뾶
};

#endif