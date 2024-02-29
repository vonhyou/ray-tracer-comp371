#ifndef HITRECORD_H_
#define HITRECORD_H_

#include "Geometry.h"
#include "Ray.h"
#include <Eigen/Core>

using Eigen::Vector3f;

class HitRecord {
public:
  HitRecord(float t, const Ray &r, Geometry *g) : t(t), r(r), g(g) {}
  bool operator<(const HitRecord &) const;

private:
  float t;
  Ray r;
  Vector3f n;
  Geometry *g;

public:
  Geometry *geometry() const;
  Vector3f getPoint() const;
  Vector3f viewDirection() const;
  Vector3f normal() const;
  void calcNormal();
};

#endif // !HITRECORD_H_
