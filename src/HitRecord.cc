#include "HitRecord.h"

bool HitRecord::operator<(const HitRecord &other) const {
  return this->t > other.t; // to get the nearest t
}

Geometry *HitRecord::geometry() const { return g; }

Vector3f HitRecord::getPoint() const {
  return r.getOrigin() + t * r.getDirection();
}

Vector3f HitRecord::normal() const { return n; }

void HitRecord::calcNormal() { n = g->getNormal(getPoint()); }
