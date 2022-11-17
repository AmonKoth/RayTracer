#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "math_utils.h"
#include "hittable.h"

class Sphere : public Hittable
{
public:
	Sphere() {}
	Sphere(point3 centr, double rad, shared_ptr<Material> mat) : center(centr), radius(rad),materialPtr(mat) {};

	virtual bool Hit(const ray& r, double tMin, double tMax, HitRecord& hitRecord) const override;

public:
	point3 center;
	double radius;
	shared_ptr<Material> materialPtr;
};

bool Sphere::Hit(const ray& ray, double tMin, double tMax, HitRecord& hitRecord) const {
	//(x-Cx)^2 + (y-Cy)^2 + (z-Cz)^2=r^2
	vec3 oc = ray.origin() - center;
	auto a = ray.direction().length_squared();
	auto halfB = dot(oc, ray.direction());
	auto c = oc.length_squared() - radius * radius;
	//b^2 - 4 a c
	auto discriminant = halfB * halfB - a * c;

	if (discriminant < 0) return false;
	
	auto sqrtDiscriminant = sqrt(discriminant);
	//find the nearest root withn the acceptable range
	auto root =  (-halfB - sqrtDiscriminant	) / a;
	if (root<tMin || root>tMax)
	{
		root =  (-halfB + sqrtDiscriminant	) / a;
		if (root<tMin || root>tMax)
		{
		return false;
		}
	}

	hitRecord.t = root;
	hitRecord.hitPoint = ray.at(hitRecord.t);
	vec3 outwardNormal = (hitRecord.hitPoint - center) / radius;
	hitRecord.SetFaceNormal(ray, outwardNormal);
	hitRecord.materialPtr = materialPtr;
	return true;
}


#endif // !SPHERE_H
