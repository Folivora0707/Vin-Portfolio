#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include <iostream>

/*
// 检测光线与球之间的碰撞
// 写到sphere类中简化代码
double hit_sphere(const vec3& center, double radius, const ray& r) {
	/*
	// 由球面的直角坐标系表达式推导出有关参数t（ray类的at函数）的一元二次方程
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - radius * radius;
	// 一元二次方程是否有解
	auto discriminant = b * b - 4 * a * c;
	// return (discriminant > 0);
	if (discriminant < 0) {
		return -1.0;
	}   // 无碰撞返回负值，不渲染
	else {
		return (-b - sqrt(discriminant)) / (2.0*a);
	}   // 有碰撞则返回t的较小解
	// *
	// 消去常数，简化表达式
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	if (discriminant < 0) {
		return -1.0;
	}
	else {
		// 求t的较小解（两个解的情况下，较大解属于背光面，不渲染）
		return (-half_b - sqrt(discriminant)) / a;
	}
}
*/

/*
vec3 ray_color(const ray& r, const hittable& world) {
	/*
	// 如果光线与球相遇，返回红色
	// 球中心(0,0,-1)，半径为0.5
	if (hit_sphere(vec3(0, 0, -1), 0.5, r))    
		return vec3(1, 0, 0);
	*
	/*
	auto t = hit_sphere(vec3(0, 0, -1), 0.5, r);
	
	// t为正则表示光线与球的交点位于可视域
	if (t > 0.0) {
		// 计算法线向量N，并以此为依据着色
		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
		return 0.5*vec3(N.x() + 1, N.y() + 1, N.z() + 1);
	}
	*
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {
		// 原点出发发射光线到无穷远，以相交点面法线为依据着色
		// return 0.5 * (rec.normal + vec3(1, 1, 1));

		// 由相交点出发随机发射光线，光线终点在单位圆域内
		// 注意若反射光线一直能碰到物体就会无限递归
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5 * ray_color(ray(rec.p, target - rec.p), world);
	}   

	vec3 unit_direction = unit_vector(r.direction());  // 单位化向量
	// 原Y轴坐标属于[-1,1]，单位化后必然缩小，+1 后 *0.5结果必然属于[0,1]
	// 以此为权重，可以创造上下渐变的图片
	auto t = 0.5*(unit_direction.y() + 1.0);
	
	// 背景颜色，简单的蓝白渐变天空
	return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}
*/
// 着色
vec3 ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	// 添加反射次数防止无限递归
	// 超过递归次数，视为光能衰减至0，返回黑色，即阴影部分
	if (depth <= 0)
		return vec3(0, 0, 0);

	// if (world.hit(r, 0, infinity, rec)) {
	// 忽略接近0的浮点值
	if (world.hit(r, 0.001, infinity, rec)) {

		// vec3 target = rec.p + rec.normal + random_in_unit_sphere(); // 球域反射
		// vec3 target = rec.p + random_in_hemisphere(rec.normal);     // 半球反射
		// vec3 target = rec.p + rec.normal + random_unit_vector();    // 理想散射
		// 每次递归 * 0.5 ，即能量衰减一半
		// return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);

		ray scattered;        // 散射
		vec3 attenuation;     // 衰减，材质创建时赋值，RGB衰减量可不同

		// 正常散射则递归
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return vec3(0, 0, 0); // 阴影
	}   

	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

// 场景搭建
hittable_list random_scene() {
	hittable_list world;

	world.add(make_shared<sphere>(
		vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = vec3::random() * vec3::random();
					world.add(
						make_shared<sphere>(center, 0.2, make_shared<lambertian>(albedo)));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = vec3::random(.5, 1);
					auto fuzz = random_double(0, .5);
					world.add(
						make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
				}
				else {
					// glass
					world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
				}
			}
		}
	}

	world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

	world.add(
		make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));

	world.add(
		make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

	return world;
}

int main() {
	const int image_width = 200;       // 宽（px）
	const int image_height = 100;      // 高（px）
	const int samples_per_pixel = 100; // 采样次数
	const int max_depth = 50;          // 递归深度
	// 宽高比
	const auto aspect_ratio = double(image_width) / image_height;          
	
	// PPM图片格式前三行
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	/*
	// 设置一个位于原点的摄像机，4×2取景屏Z轴坐标-1
	// 右为X轴，上为Y轴，后为Z轴，前为屏幕，符合右手坐标系法则
	// 为了计算方便，并没有使用单位向量
	vec3 lower_left_corner(-2.0, -1.0, -1.0);  // 屏幕左下角
	vec3 horizontal(4.0, 0.0, 0.0);            // X轴，水平方向
	vec3 vertical(0.0, 2.0, 0.0);              // Y轴，竖直方向
	vec3 origin(0.0, 0.0, 0.0);                // 原点
	*/
	/*

	hittable_list world;

	world.add(make_shared<sphere>(
		// 小球，红加蓝整个紫色
		vec3(0, 0, -1), 0.5, make_shared<lambertian>(vec3(0.8, 0.2, 0.8))));
		// 全折射小球
		// vec3(0, 0, -1), 0.5, make_shared<dielectric>(1.5/1.0)));

	world.add(make_shared<sphere>(
		// 大球（地面）绿色衰减少，故偏绿
		vec3(0, -100.5, -1), 100, make_shared<lambertian>(vec3(0.3, 0.7, 0.3))));

	world.add(make_shared<sphere>(vec3(1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.6, 0.2), 0.3)));  // 右球偏黄，粗糙指数0.3
	// world.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.8, 0.8), 1.0))); // 左球偏白，粗糙指数1.0
	
	// 空气折射率取1.0，玻璃折射率（1.3-1.7）取1.5
	world.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<dielectric>(1.5/1.0)));
	// 相同位置创建一个小球，表面反向，创造空心玻璃球效果
	world.add(make_shared<sphere>(vec3(-1, 0, -1), -0.45, make_shared<dielectric>(1.5)));  

	camera cam;                     // 创建初始相机
	*/
	/*
	camera cam(90, double(image_width) / image_height); // 广角相机
	auto R = cos(pi / 4);
	hittable_list world;
	world.add(make_shared<sphere>(vec3(-R, 0, -1), R, make_shared<lambertian>(vec3(0, 1, 0))));
	world.add(make_shared<sphere>(vec3(R, 0, -1), R, make_shared<lambertian>(vec3(1, 0, 1))));
	*/
	/*
	// 一个世界，四个球
	hittable_list world;
	world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5, make_shared<lambertian>(vec3(0.8, 0.2, 0.8))));
	world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100, make_shared<lambertian>(vec3(0.3, 0.7, 0.3))));
	world.add(make_shared<sphere>(vec3(1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.6, 0.2), 0.3)));
	world.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<dielectric>(1.5)));
	*/
	/*
	vec3 vup(0, 1, 0);          // 固定相机的上方向
	// 位置，朝向，View up，视角，宽高比
	// camera cam(vec3(-2, 2, 1), vec3(0, 0, -1), vup, 90, aspect_ratio); // 远眺视角
	// 从摄像机位置（x,y,z）看向紫球球心（0,0,-1）
	// camera cam(vec3(-1, 2, 0), vec3(0, 0, -1), vup, 90, aspect_ratio); // 拉近
	camera cam(vec3(-2, 2, 1), vec3(0, 0, -1), vup, 30, aspect_ratio); //视角调小，近观视野
	*/
	// hittable_list world;            // 创建世界
	// world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));       // 创建个小球
	// world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));  // 创建个大球作为地面
	/*
	// 具有景深的灵活相机
	vec3 lookfrom(3, 3, 2);
	vec3 lookat(0, 0, -1);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = (lookfrom - lookat).length(); // 焦距
	auto aperture = 2.0;                               // 光圈
	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
	*/

	// 创建世界
	auto world = random_scene();

	// 设置相机
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	// 渲染
	for (int j = image_height - 1; j >= 0; --j) {
		// 使用error输出流指示进度
		std::cerr << "\r未渲染行数：" << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			/* R值递增，G值递减，B值固定的渐变图片
			// 未定义vec3类之前
			auto r = double(i) / image_width;
			auto g = double(j) / image_height;
			auto b = 0.2;
			// static_cast<int> 强制转换为int
			int ir = static_cast<int>(255.999 * r); 
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);
			// PPM图片可以 R G B 三个值占据一行，也可以一个值占据一行
			std::cout << ir << ' ' << ig << ' ' << ib << '\n';

			// 使用vec3类中定义的方法直接输出像素值
			vec3 color(double(i) / image_width, double(j) / image_height, 0.2);
			color.write_color(std::cout);
			*
			auto u = double(i) / image_width;
			auto v = double(j) / image_height;

			// 由原点向屏幕发射光线
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			vec3 color = ray_color(r, world);   // 着色
			color.write_color(std::cout);
			*/
			vec3 color(0, 0, 0);    // 初始化为黑色 
			// 利用随机数多次采样
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + random_double()) / image_width;
				auto v = (j + random_double()) / image_height;
				ray r = cam.get_ray(u, v);               // 发射光线
				color += ray_color(r, world, max_depth); // 着色
			}
			color.write_color(std::cout, samples_per_pixel);
		}
	}
	// 同样使用error输出流指示渲染完成
	std::cerr << "\n完事儿。\n";
}