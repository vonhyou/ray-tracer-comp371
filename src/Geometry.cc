#include "Geometry.h"

void Geometry::setTransform(const Matrix4f &transform) {
  this->transform = transform;
}

bool Sphere::intersect() const { return false; }

bool Rectangle::intersect() const { return false; }
