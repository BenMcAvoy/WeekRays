#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
	HittableList() = default;

	/*void add(const std::shared_ptr<Hittable>& object) {
		objects_.push_back(object);
	}*/

	template <typename T, typename... Args>
	void add(Args&&... args) {
		objects_.push_back(std::make_shared<T>(std::forward<Args>(args)...));
	}

	bool hit(const Ray& r, Interval tInterval, HitRecord& rec) const override {
		HitRecord tempRec;
		bool hitAnything = false;
		auto closestSoFar = tInterval.max;

		for (const auto& object : objects_) {
			if (object->hit(r, { tInterval.min, closestSoFar }, tempRec)) {
				hitAnything = true;
				closestSoFar = tempRec.t;
				rec = tempRec;
			}
		}

		return hitAnything;
	}

private:
	std::vector<std::shared_ptr<Hittable>> objects_;
};
