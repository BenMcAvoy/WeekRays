#pragma once

#include "weekrays.h"

struct Interval {
	Interval(double min, double max) : min{ min }, max{ max } {}
	Interval() : min{ -infinity }, max{ infinity } {}

	double min;
	double max;

	double size() const {
		return max - min;
	}

	bool contains(double value) const {
		return min <= value && value <= max;
	}

	bool surrounds(double value) const {
		return min < value && value < max;
	}

	double clamp(double value) const {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	static const Interval empty;
	static const Interval universe;
};

const Interval Interval::empty		= Interval(+infinity, -infinity);
const Interval Interval::universe	= Interval(-infinity, +infinity);
