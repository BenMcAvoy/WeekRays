#pragma once

#include "weekrays.h"

class Sphere : public Hittable {
public:
	Sphere(const Vec3& center, double radius, Material* material)
		: center_{ center }, radius_{ radius }, material_{ material } {
	}

	bool hit(const Ray& r, Interval tInterval, HitRecord& rec) const override {
		Vec3 oc = r.origin() - center_;
		auto a = r.direction().lengthSquared();
		auto half_b = dot(oc, r.direction());
		auto c = oc.lengthSquared() - radius_ * radius_;
		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) {
			return false;
		}

		auto sqrtd = std::sqrt(discriminant);
		// Find the nearest root that lies in the acceptable range.
		auto root = (-half_b - sqrtd) / a;
		if (!tInterval.surrounds(root)) {
			root = (-half_b + sqrtd) / a;
			if (!tInterval.surrounds(root)) {
				return false;
			}
		}

		rec.t = root;
		rec.point = r.at(rec.t);
		Vec3 outwardNormal = (rec.point - center_) / radius_;
		rec.setFaceNormal(r, outwardNormal);
		rec.material = material_;

		return true;
	}

private:
	Vec3 center_;
	double radius_;
	Material* material_;
};
