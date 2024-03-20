#ifndef RAY_H_
#define RAY_H_

#include <Eigen/Core>

using Eigen::Vector3f;

class Ray {
public:
  Ray() : origin_(Vector3f::Zero()), direction_(Vector3f::Zero()) {}
  Ray(const Vector3f &o, const Vector3f &d) : origin_(o), direction_(d) {}

private:
  Vector3f origin_;
  Vector3f direction_;

public:
  Vector3f origin() const;
  Vector3f direction() const;
};

#endif // !RAY_H_
