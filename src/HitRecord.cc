#include "HitRecord.h"

bool HitRecord::operator<(const HitRecord &other) const {
  return this->t > other.t; // to get the nearest t
}

Geometry *HitRecord::geometry() const { return g; }
