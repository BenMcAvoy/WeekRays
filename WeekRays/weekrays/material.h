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
	Metal(const Vec3& albedo, double fuzz) : albedo_{ albedo }, fuzz_{ fuzz < 1 ? fuzz : 1 } {}

	bool scatter(
		const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered
	) const override {
		Vec3 reflected = rayIn.direction().reflected(rec.normal);
		reflected = reflected.normalized() + (fuzz_ * Vec3::randomUnit());
		scattered = Ray(rec.point, reflected);
		attenuation = albedo_;

		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	Vec3 albedo_;
	double fuzz_;
};

class Dielectric : public Material {
public:
	Dielectric(double refractionIndex_) : refractionIndex_{ refractionIndex_ } {}

	bool scatter(
		const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered
	) const override {
		attenuation = { 1.0, 1.0, 1.0 };
		double ri = rec.frontFace ? (1.0 / refractionIndex_) : refractionIndex_;

		Vec3 unitDir = rayIn.direction().normalized();
        double cosTheta = std::fmin(dot(-unitDir, rec.normal), 1.0);
        double sinTheta = std::sqrt(1.0 - cosTheta*cosTheta);

        bool cannotRefract = ri * sinTheta > 1.0;
        Vec3 direction;

		if (cannotRefract || reflectance(cosTheta) > randomDouble())
			direction = unitDir.reflected(rec.normal);
		else
			direction = unitDir.refract(rec.normal, ri);

		scattered = Ray(rec.point, direction);
		return true;
	}

private:
	double refractionIndex_;

	double reflectance(double cosine) const {
		auto r0 = (1 - refractionIndex_) / (1 + refractionIndex_);
		r0 = r0 * r0;

		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};
