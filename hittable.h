#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "math_utils.h"

class Material;

struct HitRecord {

	point3 hitPoint;
	vec3 normal;
	shared_ptr<Material> materialPtr;
	double t;
	bool frontFace;

	inline void SetFaceNormal(const ray& r, const vec3& outwardNormal)
	{
		frontFace = dot(r.direction(), outwardNormal) < 0;
		if(frontFace)
		{
			normal = outwardNormal;
			return;
		}
		normal = -outwardNormal;
	}
};


class Hittable {

public:
	virtual bool Hit(const ray& r, double tMin, double tMax, HitRecord& record) const = 0;

};

#endif // !HITTABLE_H
