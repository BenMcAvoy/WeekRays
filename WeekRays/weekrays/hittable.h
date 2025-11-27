#pragma once

#include "weekrays.h"

class Material;

struct HitRecord {
	Vec3 point;
	Vec3 normal;
	double t;

	bool frontFace;

	Material* material;

	void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
		frontFace = dot(r.direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable {
public:
	virtual ~Hittable() = default;
	virtual bool hit(const Ray& r, Interval tInterval, HitRecord& rec) const = 0;
};
