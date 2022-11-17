#pragma once
#ifndef COLOR_H
#define COLOR_H

#include<iostream>
#include "vec3.h"

void writeColor(std::ostream& out, color pixelColor, int samplesPerPixel)
{
	auto r = pixelColor.x();
	auto g = pixelColor.y();
	auto b = pixelColor.z();

	auto scale = 1.0 / samplesPerPixel;
	r = sqrt(scale*r);
	g = sqrt(scale*g);
	b = sqrt(scale*b);
	

	out << static_cast<int>(256 * Clamp(r,0.0,0.999)) << " "
		<< static_cast<int>(256* Clamp(g, 0.0, 0.999)) << " "
		<< static_cast<int>(256 * Clamp(b, 0.0, 0.999)) << "\n ";
}
#endif // !COLOR_H
