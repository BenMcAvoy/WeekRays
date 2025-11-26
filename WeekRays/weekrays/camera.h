#pragma once

#include "hittable.h"
#include "material.h"

class Camera {
public:
	Camera(int width, int height = 0) {
		imageWidth_ = width;

		aspectRatio_ = height != 0 ? static_cast<double>(width) / static_cast<double>(height) : aspectRatio_;
		int imageHeight = height != 0 ? height : static_cast<int>(imageWidth_ / aspectRatio_);

		auto viewportHeight = 2.0;
		auto viewportWidth = aspectRatio_ * viewportHeight;
		auto focalLength = 1.0;
		centerPos_ = Vec3(0, 0, 0);
		horizontal_ = Vec3(viewportWidth, 0, 0);
		vertical_ = Vec3(0, -viewportHeight, 0);
		pixel00Pos_ = centerPos_ - (horizontal_ / 2) - (vertical_ / 2) - Vec3(0, 0, focalLength);
		pixelDeltaU_ = horizontal_ / imageWidth_; // horizontal increment per pixel
		pixelDeltaV_ = vertical_ / imageHeight; // vertical increment per pixel
	}

	void render(const Hittable& world, std::ostream& os) {
		os << "P3\n" << imageWidth_ << ' ' << static_cast<int>(imageWidth_ / aspectRatio_) << "\n255\n";

		for (int j = 0; j < static_cast<int>(imageWidth_ / aspectRatio_); j++) {
			std::clog << "\rScanlines remaining: " << (static_cast<int>(imageWidth_ / aspectRatio_) - j) << ' ' << std::flush;
			for (int i = 0; i < imageWidth_; i++) {
				Colour pixelColour(0, 0, 0);
				for (int s = 0; s < samplesPerPixel_; s++) {
					Ray r = getRay(i, j);
					pixelColour += rayColour(r, maxDepth_, world);
				}
				writeColour(os, pixelColour * pixelSampleScale_);
			}
		}

		std::clog << "\nDone.\n";
	}

private:
	Colour rayColour(const Ray& r, int depth, const Hittable& world) {
		if (depth <= 0)
			return { 0, 0, 0 };

		HitRecord rec;

		if (world.hit(r, Interval(0.001, infinity), rec)) {
			Ray scattered;
			Colour attenuation;

			if (rec.material->scatter(r, rec, attenuation, scattered)) {
				return attenuation * rayColour(scattered, depth - 1, world);
			}

			return { 0, 0, 0 };
		}

		Vec3 unit_direction = r.direction().normalized();
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * Colour(1.0, 1.0, 1.0) + a * Colour(0.5, 0.7, 1.0);
	}

	Ray getRay(int pixelX, int pixelY) {
		auto u = (static_cast<double>(pixelX) + randomDouble()) / static_cast<double>(imageWidth_ - 1);
		auto v = (static_cast<double>(pixelY) + randomDouble()) / static_cast<double>(static_cast<int>(imageWidth_ / aspectRatio_) - 1);
		auto pixelPos = pixel00Pos_ + u * horizontal_ + v * vertical_;
		auto rayDir = pixelPos - centerPos_;
		return { centerPos_, rayDir };
	}

	double aspectRatio_ = 16.0 / 9.0;
	int imageWidth_ = 400;

	int samplesPerPixel_ = 100;
	double pixelSampleScale_ = 1.0 / static_cast<double>(samplesPerPixel_);

	int maxDepth_ = 20;

	Vec3 centerPos_;
	Vec3 pixel00Pos_;

	Vec3 horizontal_;
	Vec3 vertical_;

	Vec3 pixelDeltaU_;
	Vec3 pixelDeltaV_;
};
