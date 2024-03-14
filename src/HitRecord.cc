#include "HitRecord.h"

bool HitRecord::operator<(const HitRecord &other) const {
  return this->t > other.t; // to get the nearest t
}

Geometry *HitRecord::geometry() const { return geometry_; }

Vector3f HitRecord::point() const {
  return ray_.origin() + t * ray_.direction();
}

Vector3f HitRecord::viewDirection() const {
  return -ray_.direction().normalized();
}

Vector3f HitRecord::normal() const { return normal_; }

void HitRecord::calcNormal() { normal_ = geometry_->normal(point()); }
