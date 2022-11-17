#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include"math_utils.h"

struct HitRecord;

class Material {
public:
	virtual bool scatter(const ray& ray_in, const HitRecord& record,
						color& attunation, ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const color& color) :albedo(color){}

	virtual bool scatter(const ray& ray_in, const HitRecord& record, 
						 color& attunation, ray& scattered) const override
	{
		auto scatterDireciton = record.normal + randomUnitVector();
		if (scatterDireciton.nearZero())
		{
			scatterDireciton = record.normal;
		}
		scattered = ray(record.hitPoint, scatterDireciton);
		attunation = albedo;
		return true;
	}
public:
	color albedo;
};

class Metal : public Material {
public:

	Metal(const color& color, double fuzzy): albedo(color),fuzzyness(fuzzy){}

	virtual bool scatter(const ray& ray_in, const HitRecord& record,
		color& attunation, ray& scattered) const override
	{
		vec3 reflected = Reflect(unitVector(ray_in.direction()), record.normal) ;
		scattered = ray(record.hitPoint, reflected + fuzzyness*randomInUnitSphere());
		attunation = albedo;
		return (dot(scattered.direction(),record.normal))>0;
	}
public:
	color albedo;
	double fuzzyness;
};

class Dielectric :public Material {
public:
	Dielectric(double indexOfRefraciton) : iofRefraction(indexOfRefraciton) {}
	virtual bool scatter(const ray& ray_in, const HitRecord& record,
		color& attunation, ray& scattered) const override{
		attunation = color(1.0, 1.0, 1.0);
		double refractionRatio = record.frontFace ? (1.0 / iofRefraction) : iofRefraction;
		vec3 unitDireciton = unitVector(ray_in.direction());

		double cosTheta = fmin(dot(-unitDireciton, record.normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0;
		vec3 direciton;

		if (cannotRefract||Reflectance(cosTheta,refractionRatio)>RandomDouble())
		{
			direciton=Reflect(unitDireciton, record.normal);
			
		}
		else
		{
			direciton = Refract(unitDireciton, record.normal, refractionRatio);
		}

		scattered = ray(record.hitPoint, direciton);
		return true;
	}


public:
	double iofRefraction;

private: 
	static double Reflectance(double cosine, double ref_idx)
	{
		//schlicks approximation
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}


};


#endif // !MATERIAL_H
