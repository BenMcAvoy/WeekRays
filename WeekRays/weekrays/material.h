#pragma once

#include "ray.h"
#include "hittable.h"

class Material {
public:
	virtual ~Material() = default;

	// returns true if the ray is scattered, false otherwise (absorption)
	virtual bool scatter(
		const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered
	) const {
		return false;
	}
};

class Lambertian : public Material {
public:
	Lambertian(const Vec3& albedo) : albedo_{ albedo } {}

	bool scatter(
		const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered
	) const override {
		auto scatterDir = rec.normal + Vec3::randomUnit();
		if (scatterDir.nearZero()) // degenerate scatter direction
			scatterDir = rec.normal;

		scattered = Ray(rec.point, scatterDir);
		attenuation = albedo_;

		return true; // always scatter
	}

private:
	Vec3 albedo_;
};

class Metal : public Material {
public:
	Metal(const Vec3& albedo) : albedo_{ albedo } {}

	bool scatter(
		const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered
	) const override {
		Vec3 reflected = rayIn.direction().reflected(rec.normal);
		scattered = Ray(rec.point, reflected);
		attenuation = albedo_;
		return true;
	}

private:
	Vec3 albedo_;
};
