#include "Geometry.h"

void Geometry::setTransform(const Matrix4f &transform) {
  this->transform = transform;
}

bool Sphere::intersect(const Ray &r) const {
  Vector3f originCenter = r.getOrigin() - center;
  int a = r.getDirection().dot(r.getDirection());
  int b = 2 * originCenter.dot(r.getDirection());
  int c = originCenter.dot(originCenter) - radius * radius;

  return b * b - 4 * a * c >= 0;
}

bool Rectangle::intersect(const Ray &r) const { return false; }
