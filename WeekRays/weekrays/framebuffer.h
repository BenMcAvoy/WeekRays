#pragma once

#include "weekrays.h"

class Framebuffer {
public:
	Framebuffer(int width, int height)
		: pixels_{ new Colour[width * height], static_cast<size_t>(width * height) }, width_{ width }, height_{ height } {
	}

	~Framebuffer() {
		delete[] pixels_.data();
	}

	[[nodiscard]] Colour& at(int x, int y, int width) {
		return pixels_[y * width + x];
	}

	[[nodiscard]] std::span<Colour> pixels() const {
		return pixels_;
	}

	void clear(const Colour& clearColour) {
		for (auto& pixel : pixels_) {
			pixel = clearColour;
		}
	}

	void saveAsPPM(std::string_view filename) const {
		std::ofstream ofs(filename.data());
		ofs << "P3\n" << width_ << ' ' << height_ << "\n255\n";
		for (const auto& pixel : pixels_) {
			writeColour(ofs, pixel);
		}
	}

private:
	std::span<Colour> pixels_;
	int width_;
	int height_;
};
