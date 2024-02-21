#ifndef RAY_H_
#define RAY_H_

#include <Eigen/Core>

using Eigen::Vector3f;

class Ray {
public:
  Ray(const Vector3f &o, const Vector3f &d) : origin(o), direction(d) {}

private:
  Vector3f origin;
  Vector3f direction;

public:
  Vector3f getOrigin() const;
  Vector3f getDirection() const;
};

#endif // !RAY_H_
