#include <iostream>
#include "math_utils.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

HittableList randomScene()
{
	HittableList world;
	auto groundMaterial = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
	world.Add(make_shared<Sphere>(point3(0, -1000, 0), 1000, groundMaterial));
	
	for (int a = -5; a < 11; a++)
	{
		for (int b = -5; b < 11; b++)
		{
			auto chooseMat = RandomDouble();
			point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

			if ((center - point3(4, 0.2, 0)).length() > 0.9)
			{
				shared_ptr<Material> sphereMaterial;
				if (chooseMat < 0.8)
				{
					//diffuse
					auto albedo = color::random() * color::random();
					sphereMaterial = make_shared<Lambertian>(albedo);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else if (chooseMat < 0.95)
				{
					//metal
					auto albedo = color::random();
					auto fuzz = RandomDouble(0, 0.5);
					sphereMaterial = make_shared<Metal>(albedo, fuzz);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else
				{
					//glass
					sphereMaterial = make_shared<Dielectric>(1.5);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
			}


		}
	}
	
	auto material1 = make_shared<Dielectric>(1.5);
	world.Add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
	world.Add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
	world.Add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));



	return world;
}

color raycolor(const ray& r, const Hittable& world, int depth) {
	HitRecord record;
	if (depth <= 0)
	{
		return color(0, 0, 0);
	}
	if(world.Hit(r,0.001,infinity,record))
	{
		ray scattered;
		color attenuation;
		if (record.materialPtr->scatter(r, record, attenuation, scattered))
		{
			return attenuation * raycolor(scattered, world, depth - 1);
		}
		return color(0,0,0);
	}
	vec3 unitDirection = unitVector(r.direction());
	auto t = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
	//Image
	const auto aspectRatio = 3.0 / 2.0;
	const int imageWidth = 400;
	const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
	const int samplesPerPixel = 50;
	const int maxDepth = 50;

	//World
	auto world = randomScene();


	//Camera
	point3 lookFrom(10, 2, 2);
	point3 lookAt(0, 0, 0);
	vec3 vUp(0, 1, 0);
	auto distToFocus = 10;
	auto aperture =0.1;

	Camera camera(lookFrom, lookAt, vUp, 90.0, aspectRatio,aperture,distToFocus);

	std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
	for (int j = imageHeight-1; j >= 0; --j)
	{
		std::cerr << " \rScanlines remaning:" << j << " " << std::flush;
		for (int i = 0; i < imageWidth; ++i)
		{
			color pixelColor(0, 0, 0); 

			for (int s = 0; s < samplesPerPixel; ++s)
			{
				auto u = (i + RandomDouble()) / (imageWidth - 1);
				auto v = (j + RandomDouble()) / (imageWidth - 1);
				ray Ray = camera.getRay(u, v);

				pixelColor += raycolor(Ray, world,maxDepth);
			}
			writeColor(std::cout, pixelColor,samplesPerPixel);
			pixelColor = color(0, 0, 0);
		}
	}
	std::cerr << "\nDone!\n";
}