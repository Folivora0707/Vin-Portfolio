#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include <iostream>

/*
// ����������֮�����ײ
// д��sphere���м򻯴���
double hit_sphere(const vec3& center, double radius, const ray& r) {
	/*
	// �������ֱ������ϵ���ʽ�Ƶ����йز���t��ray���at��������һԪ���η���
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - radius * radius;
	// һԪ���η����Ƿ��н�
	auto discriminant = b * b - 4 * a * c;
	// return (discriminant > 0);
	if (discriminant < 0) {
		return -1.0;
	}   // ����ײ���ظ�ֵ������Ⱦ
	else {
		return (-b - sqrt(discriminant)) / (2.0*a);
	}   // ����ײ�򷵻�t�Ľ�С��
	// *
	// ��ȥ�������򻯱��ʽ
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	if (discriminant < 0) {
		return -1.0;
	}
	else {
		// ��t�Ľ�С�⣨�����������£��ϴ�����ڱ����棬����Ⱦ��
		return (-half_b - sqrt(discriminant)) / a;
	}
}
*/

/*
vec3 ray_color(const ray& r, const hittable& world) {
	/*
	// ��������������������غ�ɫ
	// ������(0,0,-1)���뾶Ϊ0.5
	if (hit_sphere(vec3(0, 0, -1), 0.5, r))    
		return vec3(1, 0, 0);
	*
	/*
	auto t = hit_sphere(vec3(0, 0, -1), 0.5, r);
	
	// tΪ�����ʾ��������Ľ���λ�ڿ�����
	if (t > 0.0) {
		// ���㷨������N�����Դ�Ϊ������ɫ
		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
		return 0.5*vec3(N.x() + 1, N.y() + 1, N.z() + 1);
	}
	*
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {
		// ԭ�����������ߵ�����Զ�����ཻ���淨��Ϊ������ɫ
		// return 0.5 * (rec.normal + vec3(1, 1, 1));

		// ���ཻ��������������ߣ������յ��ڵ�λԲ����
		// ע�����������һֱ����������ͻ����޵ݹ�
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5 * ray_color(ray(rec.p, target - rec.p), world);
	}   

	vec3 unit_direction = unit_vector(r.direction());  // ��λ������
	// ԭY����������[-1,1]����λ�����Ȼ��С��+1 �� *0.5�����Ȼ����[0,1]
	// �Դ�ΪȨ�أ����Դ������½����ͼƬ
	auto t = 0.5*(unit_direction.y() + 1.0);
	
	// ������ɫ���򵥵����׽������
	return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}
*/
// ��ɫ
vec3 ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	// ��ӷ��������ֹ���޵ݹ�
	// �����ݹ��������Ϊ����˥����0�����غ�ɫ������Ӱ����
	if (depth <= 0)
		return vec3(0, 0, 0);

	// if (world.hit(r, 0, infinity, rec)) {
	// ���Խӽ�0�ĸ���ֵ
	if (world.hit(r, 0.001, infinity, rec)) {

		// vec3 target = rec.p + rec.normal + random_in_unit_sphere(); // ������
		// vec3 target = rec.p + random_in_hemisphere(rec.normal);     // ������
		// vec3 target = rec.p + rec.normal + random_unit_vector();    // ����ɢ��
		// ÿ�εݹ� * 0.5 ��������˥��һ��
		// return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);

		ray scattered;        // ɢ��
		vec3 attenuation;     // ˥�������ʴ���ʱ��ֵ��RGB˥�����ɲ�ͬ

		// ����ɢ����ݹ�
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return vec3(0, 0, 0); // ��Ӱ
	}   

	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

// �����
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
	const int image_width = 200;       // ��px��
	const int image_height = 100;      // �ߣ�px��
	const int samples_per_pixel = 100; // ��������
	const int max_depth = 50;          // �ݹ����
	// ��߱�
	const auto aspect_ratio = double(image_width) / image_height;          
	
	// PPMͼƬ��ʽǰ����
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	/*
	// ����һ��λ��ԭ����������4��2ȡ����Z������-1
	// ��ΪX�ᣬ��ΪY�ᣬ��ΪZ�ᣬǰΪ��Ļ��������������ϵ����
	// Ϊ�˼��㷽�㣬��û��ʹ�õ�λ����
	vec3 lower_left_corner(-2.0, -1.0, -1.0);  // ��Ļ���½�
	vec3 horizontal(4.0, 0.0, 0.0);            // X�ᣬˮƽ����
	vec3 vertical(0.0, 2.0, 0.0);              // Y�ᣬ��ֱ����
	vec3 origin(0.0, 0.0, 0.0);                // ԭ��
	*/
	/*

	hittable_list world;

	world.add(make_shared<sphere>(
		// С�򣬺����������ɫ
		vec3(0, 0, -1), 0.5, make_shared<lambertian>(vec3(0.8, 0.2, 0.8))));
		// ȫ����С��
		// vec3(0, 0, -1), 0.5, make_shared<dielectric>(1.5/1.0)));

	world.add(make_shared<sphere>(
		// ���򣨵��棩��ɫ˥���٣���ƫ��
		vec3(0, -100.5, -1), 100, make_shared<lambertian>(vec3(0.3, 0.7, 0.3))));

	world.add(make_shared<sphere>(vec3(1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.6, 0.2), 0.3)));  // ����ƫ�ƣ��ֲ�ָ��0.3
	// world.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.8, 0.8), 1.0))); // ����ƫ�ף��ֲ�ָ��1.0
	
	// ����������ȡ1.0�����������ʣ�1.3-1.7��ȡ1.5
	world.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<dielectric>(1.5/1.0)));
	// ��ͬλ�ô���һ��С�򣬱��淴�򣬴�����Ĳ�����Ч��
	world.add(make_shared<sphere>(vec3(-1, 0, -1), -0.45, make_shared<dielectric>(1.5)));  

	camera cam;                     // ������ʼ���
	*/
	/*
	camera cam(90, double(image_width) / image_height); // ������
	auto R = cos(pi / 4);
	hittable_list world;
	world.add(make_shared<sphere>(vec3(-R, 0, -1), R, make_shared<lambertian>(vec3(0, 1, 0))));
	world.add(make_shared<sphere>(vec3(R, 0, -1), R, make_shared<lambertian>(vec3(1, 0, 1))));
	*/
	/*
	// һ�����磬�ĸ���
	hittable_list world;
	world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5, make_shared<lambertian>(vec3(0.8, 0.2, 0.8))));
	world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100, make_shared<lambertian>(vec3(0.3, 0.7, 0.3))));
	world.add(make_shared<sphere>(vec3(1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.6, 0.2), 0.3)));
	world.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<dielectric>(1.5)));
	*/
	/*
	vec3 vup(0, 1, 0);          // �̶�������Ϸ���
	// λ�ã�����View up���ӽǣ���߱�
	// camera cam(vec3(-2, 2, 1), vec3(0, 0, -1), vup, 90, aspect_ratio); // Զ���ӽ�
	// �������λ�ã�x,y,z�������������ģ�0,0,-1��
	// camera cam(vec3(-1, 2, 0), vec3(0, 0, -1), vup, 90, aspect_ratio); // ����
	camera cam(vec3(-2, 2, 1), vec3(0, 0, -1), vup, 30, aspect_ratio); //�ӽǵ�С��������Ұ
	*/
	// hittable_list world;            // ��������
	// world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));       // ������С��
	// world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));  // ������������Ϊ����
	/*
	// ���о����������
	vec3 lookfrom(3, 3, 2);
	vec3 lookat(0, 0, -1);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = (lookfrom - lookat).length(); // ����
	auto aperture = 2.0;                               // ��Ȧ
	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
	*/

	// ��������
	auto world = random_scene();

	// �������
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	// ��Ⱦ
	for (int j = image_height - 1; j >= 0; --j) {
		// ʹ��error�����ָʾ����
		std::cerr << "\rδ��Ⱦ������" << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			/* Rֵ������Gֵ�ݼ���Bֵ�̶��Ľ���ͼƬ
			// δ����vec3��֮ǰ
			auto r = double(i) / image_width;
			auto g = double(j) / image_height;
			auto b = 0.2;
			// static_cast<int> ǿ��ת��Ϊint
			int ir = static_cast<int>(255.999 * r); 
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);
			// PPMͼƬ���� R G B ����ֵռ��һ�У�Ҳ����һ��ֵռ��һ��
			std::cout << ir << ' ' << ig << ' ' << ib << '\n';

			// ʹ��vec3���ж���ķ���ֱ���������ֵ
			vec3 color(double(i) / image_width, double(j) / image_height, 0.2);
			color.write_color(std::cout);
			*
			auto u = double(i) / image_width;
			auto v = double(j) / image_height;

			// ��ԭ������Ļ�������
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			vec3 color = ray_color(r, world);   // ��ɫ
			color.write_color(std::cout);
			*/
			vec3 color(0, 0, 0);    // ��ʼ��Ϊ��ɫ 
			// �����������β���
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + random_double()) / image_width;
				auto v = (j + random_double()) / image_height;
				ray r = cam.get_ray(u, v);               // �������
				color += ray_color(r, world, max_depth); // ��ɫ
			}
			color.write_color(std::cout, samples_per_pixel);
		}
	}
	// ͬ��ʹ��error�����ָʾ��Ⱦ���
	std::cerr << "\n���¶���\n";
}