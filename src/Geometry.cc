#include "Geometry.h"

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
  Vector3f p1p2 = corners.col(1) - corners.col(0);
  Vector3f p2p3 = corners.col(2) - corners.col(1);
  Vector3f p3p1 = corners.col(0) - corners.col(2);
  Vector3f p1p4 = corners.col(3) - corners.col(0);
  Vector3f p4p3 = corners.col(2) - corners.col(3);

  Vector3f n = -p1p2.cross(p2p3) / p1p2.cross(p2p3).norm();

  if (n.dot(r.getOrigin() + r.getDirection()) == 0) {
  }
  return false;
}
