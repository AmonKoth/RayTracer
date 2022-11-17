#pragma once
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>
#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
public:
	HittableList() {}
	HittableList(shared_ptr<Hittable> object) { Add(object); }

	void Clear() { hittableObjects.clear(); }
	void Add(shared_ptr<Hittable> object) { hittableObjects.push_back(object); }

	virtual bool Hit(const ray& r, double tMin, double tMax, HitRecord& record) const override;

public:
	std::vector<shared_ptr<Hittable>> hittableObjects;
};

bool HittableList::Hit(const ray& r, double tMin, double tMax, HitRecord& record) const
{
	HitRecord tempHit;
	bool hitAnything = false;
	auto closestSoFar = tMax;

	for (const auto& object : hittableObjects)
	{
		if (object->Hit(r, tMin, closestSoFar, tempHit))
		{
			hitAnything = true;
			closestSoFar = tempHit.t;
			record = tempHit;
		}
	}

	return hitAnything;
}


#endif // !HITTABLE_LIST_H
