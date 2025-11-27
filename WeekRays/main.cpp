#include "weekrays/all.h"

constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;

std::atomic<int> nextTile{ 0 };

static void worker(int totalTiles, int tilesX, int tileWidth, int tileHeight, Camera& camera, const Hittable& world, Framebuffer& framebuffer) {
	for (;;) {
		int tile = nextTile.fetch_add(1);
		if (tile >= totalTiles) break;

		camera.renderTile(
			(tile % tilesX) * tileWidth,
			(tile / tilesX) * tileHeight,
			tileWidth,
			tileHeight,
			world,
			framebuffer
		);
	}
}

int main() {
	auto matGround = Lambertian{ {1.0, 1.0, 1.0} };

	auto matCenter = Lambertian{ {0.8, 0.7, 0.8} };

	auto matLeft = Dielectric{ 1.5 };
	auto matLeftInner = Dielectric{ 1.0 / 1.5 }; // air

	auto matRight = Metal{ Colour(0.8, 0.6, 0.2), 0.1 };

	HittableList world;
	world.add<Sphere>(Vec3(0, 0, -1), 0.5, &matCenter);
	world.add<Sphere>(Vec3(0, -100.5, -1), 100, &matGround);
	world.add<Sphere>(Vec3(-1, 0, -1), 0.5, &matLeft);
	world.add<Sphere>(Vec3(-1, 0, -1), 0.4, &matLeftInner); // hollow sphere
	world.add<Sphere>(Vec3(1, 0, -1), 0.5, &matRight);

	Framebuffer framebuffer(WIDTH, HEIGHT);
	Camera camera(WIDTH, HEIGHT);

	auto start = std::chrono::high_resolution_clock::now();

	auto numThreads = std::thread::hardware_concurrency();
	const int tileWidth = 64;
	const int tileHeight = 64;
	const int tilesX = (WIDTH + tileWidth - 1) / tileWidth;
	const int tilesY = (HEIGHT + tileHeight - 1) / tileHeight;
	const int totalTiles = tilesX * tilesY;
	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < numThreads; i++) {
		threads.emplace_back(worker, totalTiles, tilesX, tileWidth, tileHeight, std::ref(camera), std::ref(world), std::ref(framebuffer));
	}
	for (auto& t : threads) {
		t.join();
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end - start;
	std::print("Render time: {} seconds\n", diff.count());

	framebuffer.saveAsPPM("output.ppm");
}