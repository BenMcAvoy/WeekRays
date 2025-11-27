#pragma once

#include <cmath>
#include <ostream>

class Vec3 {
public:
	Vec3() : e_{0, 0, 0} {}
	Vec3(double e0, double e1, double e2) : e_{e0, e1, e2} {}

	double x() const { return e_[0]; }
	double y() const { return e_[1]; }
	double z() const { return e_[2]; }

	Vec3 operator-() const { return Vec3(-e_[0], -e_[1], -e_[2]); }
	double operator[](int i) const { return e_[i]; }
	double& operator[](int i) { return e_[i]; }

	Vec3& operator+=(const Vec3& v) {
		e_[0] += v.e_[0];
		e_[1] += v.e_[1];
		e_[2] += v.e_[2];
		return *this;
	}

	Vec3& operator*=(const double t) {
		e_[0] *= t;
		e_[1] *= t;
		e_[2] *= t;
		return *this;
	}

	Vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	inline std::ostream& operator<<(std::ostream& out) {
		return out << e_[0] << ' ' << e_[1] << ' ' << e_[2];
	}

	inline friend Vec3 operator+(const Vec3& u, const Vec3& v) {
		return Vec3(u.e_[0] + v.e_[0], u.e_[1] + v.e_[1], u.e_[2] + v.e_[2]);
	}

	inline friend Vec3 operator-(const Vec3& u, const Vec3& v) {
		return Vec3(u.e_[0] - v.e_[0], u.e_[1] - v.e_[1], u.e_[2] - v.e_[2]);
	}

	inline friend Vec3 operator*(const Vec3& u, const Vec3& v) {
		return Vec3(u.e_[0] * v.e_[0], u.e_[1] * v.e_[1], u.e_[2] * v.e_[2]);
	}

	inline friend Vec3 operator*(double t, const Vec3& v) {
		return Vec3(t * v.e_[0], t * v.e_[1], t * v.e_[2]);
	}

	inline friend Vec3 operator*(const Vec3& v, double t) {
		return t * v;
	}

	inline friend Vec3 operator/(const Vec3& v, double t) {
		return (1 / t) * v;
	}

	inline friend double dot(const Vec3& u, const Vec3& v) {
		return u.e_[0] * v.e_[0] + u.e_[1] * v.e_[1] + u.e_[2] * v.e_[2];
	}

	inline friend Vec3 cross(const Vec3& u, const Vec3& v) {
		return Vec3(u.e_[1] * v.e_[2] - u.e_[2] * v.e_[1],
					u.e_[2] * v.e_[0] - u.e_[0] * v.e_[2],
					u.e_[0] * v.e_[1] - u.e_[1] * v.e_[0]);
	}

	double length() const {
		return std::hypot(e_[0], e_[1], e_[2]);
	}

	double lengthSquared() const {
		return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2];
	}

	double* data() {
		return e_;
	}

	void normalize() {
		double len = length();
		e_[0] /= len;
		e_[1] /= len;
		e_[2] /= len;
	}
	Vec3 normalized() const {
		double len = length();
		return Vec3(e_[0] / len, e_[1] / len, e_[2] / len);
	}

	bool nearZero() const {
        // Return true if the vector is close to zero in all dimensions.
		// This is used to stop NaN errors when the direction is the opposite of the normal

        auto s = 1e-8;
        return (std::fabs(e_[0]) < s) && (std::fabs(e_[1]) < s) && (std::fabs(e_[2]) < s);
    }

	Vec3 reflected(const Vec3& normal) const {
		const Vec3& v = *this;
		return v - 2 * dot(v, normal) * normal;
	}

	Vec3 refract(const Vec3& normal, double etaRatio) const {
		const Vec3& uv = *this;

		auto cosTheta = std::fmin(dot(-uv, normal), 1.0);
		Vec3 rOutPerp = etaRatio * (uv + cosTheta * normal);
		Vec3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.lengthSquared())) * normal;
		return rOutPerp + rOutParallel;
	}

	static Vec3 random() {
		return Vec3(randomDouble(), randomDouble(), randomDouble());
	}

	static Vec3 random(double min, double max) {
		return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
	}

	static Vec3 randomUnit() {
		while (true) {
			auto p = Vec3::random(-1, 1);
			auto lensq = p.lengthSquared();
			if (1e-160 < lensq && lensq < 1) {
				return p / std::sqrt(lensq);
			}
		}
	}

	static Vec3 randomUnitOnHemi(const Vec3& normal) {
		Vec3 unit = randomUnit();
		if (dot(unit, normal) > 0.0)
			return unit;
		else
			return -unit;
	}

private:
	double e_[3];
};
