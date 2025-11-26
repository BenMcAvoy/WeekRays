#pragma once

#include <cmath>
#include <cstdlib>

#include <memory>

#include <iostream>

#include <limits>
#include <numbers>

#include <fstream>
#include <chrono>
#include <print>
#include <mutex>
#include <span>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = std::numbers::pi;

inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

inline double randomDouble() {
	// [0.0, 1.0]
    return rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max) {
    // [min, max)
    return min + (max - min) * randomDouble();
}

#include "colour.h"
#include "ray.h"
#include "vec.h"
#include "interval.h"
