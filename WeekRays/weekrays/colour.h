#pragma once

#include "vec.h"
#include "interval.h"

using Colour = Vec3;

inline double mapColour(double linearComp) {
    if (linearComp > 0)
        return std::sqrt(linearComp);

    return 0;
}

void writeColour(std::ostream& out, Colour pixelColour) {
	auto r = mapColour(pixelColour.x());
	auto g = mapColour(pixelColour.y());
	auto b = mapColour(pixelColour.z());

	static const Interval intensity(0.0, 0.999);
	int rbyte = static_cast<int>(256 * intensity.clamp(r));
	int gbyte = static_cast<int>(256 * intensity.clamp(g));
	int bbyte = static_cast<int>(256 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
