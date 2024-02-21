#include "Geometry.h"

#include <Eigen/Dense>

void Geometry::setTransform(const Matrix4f &transform) {
  this->transform = transform;
}

bool Sphere::intersect(const Ray &r) const {
  Vector3f originCenter = r.getOrigin() - center;
  float a = r.getDirection().dot(r.getDirection());
  float b = 2.0f * originCenter.dot(r.getDirection());
  float c = originCenter.dot(originCenter) - radius * radius;

  return b * b - 4 * a * c >= 0;
}

bool Rectangle::intersect(const Ray &r) const {
  Vector3f normal = (p2 - p1).cross(p3 - p1).normalized();
  float d = normal.dot(p1);

  float denom = normal.dot(r.getDirection());
  if (abs(denom) < 1e-6f)
    return false;

  float t = (normal.dot(r.getOrigin() - p1) - d) / denom;
  if (t <= 0)
    return false;

  Vector3f p = r.getOrigin() + t * r.getDirection();
  return true;
}
