#ifndef HITRECORD_H_
#define HITRECORD_H_

#include "Geometry.h"
#include "Ray.h"
#include <Eigen/Core>

using Eigen::Vector3f;

class HitRecord {
public:
  HitRecord()
      : t(0), ray_(Ray()), normal_(Vector3f::Zero()), geometry_(nullptr) {}
  HitRecord(float t, const Ray &r, Geometry *g) : t(t), ray_(r), geometry_(g) {}
  bool operator<(const HitRecord &) const;

private:
  float t;
  Ray ray_;
  Vector3f normal_;
  Geometry *geometry_;

public:
  Geometry *geometry() const;
  Vector3f point() const;
  Vector3f viewDirection() const;
  Vector3f normal() const;
  void calcNormal();
};

#endif // !HITRECORD_H_
