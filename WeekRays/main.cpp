#include <print>

#include <fstream>
#include <iostream>
#include <limits>

#include "weekrays/all.h"

int main() {
	std::ofstream ofs("output.ppm");

	auto matGround = std::make_shared<Lambertian>(Colour(1.0, 1.0, 1.0));
	auto matCenter = std::make_shared<Lambertian>(Colour(0.4, 0.2, 0.1));
	auto matLeft = std::make_shared<Metal>(Colour(0.7, 0.6, 0.5));

	HittableList world;
	world.add<Sphere>(Vec3(0, 0, -1), 0.5, matCenter);
	world.add<Sphere>(Vec3(0, -100.5, -1), 100, matGround);
	world.add<Sphere>(Vec3(-1, 0, -1), 0.5, matLeft);

	Camera camera(1920);
	camera.render(world, ofs);
}