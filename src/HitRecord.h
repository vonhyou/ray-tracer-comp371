#ifndef HITRECORD_H_
#define HITRECORD_H_

#include "Geometry.h"

class HitRecord {
public:
  HitRecord(float t, Geometry *g) : t(t), g(g) {}
  bool operator<(const HitRecord &) const;

private:
  float t;
  Geometry *g;

public:
  Geometry *geometry() const;
};

#endif // !HITRECORD_H_
