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

	Colour colour() const {
		Vec3 sphere = Vec3(0, 0, -1);
		double radius = 0.5;

		auto hitSphere = [](const Vec3& sphereCenter, double sphereRadius, const Ray& r) {
			Vec3 oc = r.origin() - sphereCenter;
			auto a = r.direction().lengthSquared();
			auto h = 2.0 * dot(oc, r.direction());
			auto c = oc.lengthSquared() - sphereRadius * sphereRadius;
			auto discriminant = h * h - 4 * a * c;

			if (discriminant < 0) {
				return -1.0;
			}
			else {
				return (-h - std::sqrt(discriminant)) / (2.0 * a);
			}
			};

		auto t = hitSphere(sphere, radius, *this);
		if (t > 0.0) {
			Vec3 vt = at(t);
			Vec3 normal = (vt - sphere);
			normal = normal / normal.length();
			return 0.5 * Colour(normal.x() + 1, normal.y() + 1, normal.z() + 1);
		}

		Vec3 unitDirection = direction_;
		unitDirection = unitDirection / unitDirection.length();

		t = 0.5 * (unitDirection.y() + 1.0);
		return (1.0 - t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 0.7, 1.0);
	}

private:
	Vec3 origin_;
	Vec3 direction_;
};

