#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "math_utils.h"

class Camera {
public:
	Camera(point3 lookFrom, point3 lookAt, vec3 viewUp, double fov, double aspectRatio,double aperture,double focustdistance) {
		
		auto theta = degreesToRadians(fov);
		auto h = tan(theta / 2);
		auto viewPortHeight = 2*h;
		auto viewPortWidth = aspectRatio * viewPortHeight;

		w = unitVector(lookFrom - lookAt);
		u = unitVector(cross(viewUp, w));
		v = cross(w, u);
		
		origin = lookFrom;
		horizontal = focustdistance* viewPortWidth*u;
		vertical =  focustdistance* viewPortHeight*v;
		lowerLeftCorner = origin - (horizontal / 2) - (vertical / 2) - focustdistance* w;
		lensRadius = aperture;
	}

	ray getRay(double s, double t) const 
	{
		vec3 rd = lensRadius * RandomUnitInDisk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin-offset);
	}


private:
	point3 origin;
	point3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	double lensRadius;
};

#endif // !CAMERA_H
