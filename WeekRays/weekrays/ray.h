#pragma once

#include "vec.h"
#include "colour.h"

class Ray;

/// @return t value where ray hits sphere, -1.0 if no hit

class Ray {
public:
	Ray() : origin_{}, direction_{} {}
	Ray(const Vec3& origin, const Vec3& direction)
		: origin_{ origin }, direction_{ direction } {
	}

	const Vec3& origin() const {
		return origin_;
	}

	const Vec3& direction() const {
		return direction_;
	}

	Vec3 at(double t) const {
		return origin_ + t * direction_;
	}

private:
	Vec3 origin_;
	Vec3 direction_;
};

