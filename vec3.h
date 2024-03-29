#pragma once
#ifndef VEC_H
#define VEC_H

#include <math.h>
#include <iostream>

using std::sqrt;

class vec3 {

public:
	vec3() : e{ 0,0,0 } {}
	vec3(double e0, double e1, double e2) : e{ e0,e1,e2 } {};

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator -() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator [] (int i) const { return e[i]; }
	double& operator[] (int i) { return e[i]; }

	vec3& operator +=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*= (const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/= (const double t)
	{
		e[0] /= t;
		e[1] /= t;
		e[2] /= t;
		return *this;
	}

	double length() const
	{
		return sqrt(length_squared());
	}

	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	inline static vec3 random()
	{
		return vec3(RandomDouble(), RandomDouble(), RandomDouble());
	}
	inline static vec3 random(double min, double max)
	{
		return vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
	}

	bool nearZero() const 
	{
		const auto s = 1e-8;
		return(fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

public:
	double e[3];
};


using point3 = vec3; // 3d point
using color = vec3; //rgb

inline std::ostream& operator<<(std::ostream& out, const vec3 &v)
{
	return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& u)
{
	return vec3( t* u.e[0], t* u.e[1], t* u.e[2]);
}

inline vec3 operator*(const vec3& u , double t)
{
	return t*u;
}

inline vec3 operator/(vec3 u, double t)
{
	return(1 / t) * u;
}

inline double dot(const vec3& u, const vec3& v)
{
	return u.e[0] * v.e[0] +
		   u.e[1] * v.e[1] +
		   u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v)
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unitVector(vec3 v)
{
	return v / v.length();
}

inline vec3 randomInUnitSphere()
{
	while (true)
	{
		auto p = vec3::random(-1,1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}
vec3 randomUnitVector()
{
	return unitVector(randomInUnitSphere());
}
vec3 RandomInHemisphere(const vec3& normal)
{
	vec3 inUnitSphere = randomInUnitSphere();
	if (dot(inUnitSphere, normal) > 0.0)
	{
		return inUnitSphere;
	}
	return -inUnitSphere;
}

vec3 Reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}
vec3 Refract(const vec3& uv, const vec3& n, double etaIoveretaT)
{
	//Snells law : Refract= eta/etaPrim*(R+(-R.n)*n)
	auto cosTheta = fmin(dot(-uv, n), 1.0);
	vec3 rOutPerp = etaIoveretaT * (uv + cosTheta * n);
	vec3 rOutParallel = -sqrt(fabs(1.0f - rOutPerp.length_squared()))*n;
	return rOutPerp + rOutParallel;
}

vec3 RandomUnitInDisk()
{
	while (true)
	{
		auto p = vec3(RandomDouble(-1,1),RandomDouble(-1,1),0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

#endif // !VEC_H
