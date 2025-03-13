#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

// 摄像机类
class camera {
public:
	/*
	// 构造初始相机
	camera() {
		lower_left_corner = vec3(-2.0, -1.0, -1.0);  
		horizontal = vec3(4.0, 0.0, 0.0);               
		vertical = vec3(0.0, 2.0, 0.0);              
		origin = vec3(0.0, 0.0, 0.0);                
	}
	*/
	// 构造相机
	camera(
		vec3 lookfrom, vec3 lookat, // 确定镜头朝向
		vec3 vup,         // 相机的正上
		double vfov,      // top to bottom, in degrees，视角度数
		double aspect,    // 视野宽高比
		double aperture,  // 光圈，镜头直径
		double focus_dist // 焦距
	) {
		// origin = vec3(0.0, 0.0, 0.0);
		origin = lookfrom;          // 相机位置灵活化
		lens_radius = aperture / 2; // 镜头半径

		// vec3 u, v, w;

		auto theta = degrees_to_radians(vfov);  // 度数转弧度
		auto half_height = tan(theta / 2);      // 垂直度量的一半
		auto half_width = aspect * half_height; // 水平度量的一半

		// lower_left_corner = vec3(-half_width, -half_height, -1.0);
		// horizontal = vec3(2 * half_width, 0.0, 0.0);
		// vertical = vec3(0.0, 2 * half_height, 0.0);

		w = unit_vector(lookfrom - lookat);     // 镜头正前指向
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

	// s是屏幕横坐标的权值，t是屏幕纵坐标的权值
	ray get_ray(double s, double t) {

		// return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
		
		vec3 rd = lens_radius * random_in_unit_disk();
		// 光源的x,y坐标加上一个随机偏移
		vec3 offset = u * rd.x() + v * rd.y();

		return ray(
			origin + offset,
			lower_left_corner + s * horizontal + t * vertical - origin - offset
		);
	}

public:
	vec3 origin;              // 光源位置
	vec3 lower_left_corner;   // 基准屏幕左下角，参照点
	vec3 horizontal;          // X轴，水平方向，屏幕宽的基准
	vec3 vertical;            // Y轴，竖直方向，屏幕高的基准
	vec3 u, v, w;
	double lens_radius;       // 镜头半径
};

#endif