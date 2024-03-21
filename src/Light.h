#ifndef LIGHT_H_
#define LIGHT_H_

#include "HitRecord.h"
#include <Eigen/Core>
#include <vector>

using Eigen::Matrix;
using Eigen::Matrix4f;
using Eigen::Vector3f;
using std::vector;

// Abstract base class for Lights
class Light {
public:
  enum class Type { Point, Area };

  virtual ~Light() = default;
  virtual Vector3f illumination(const HitRecord &,
                                const vector<Geometry *> &) const = 0;
  virtual Vector3f getCenter() const = 0;

protected:
  Light(Type type, const Vector3f &id, const Vector3f &is)
      : type_(type), id_(id), is_(is) {}

  Type type_;
  Vector3f id_;
  Vector3f is_;
  Matrix4f transform = Matrix4f::Identity(); // optional member `transform`
  unsigned int gridSize = 0;                 // optional member `n`
  bool useCenter = false;                    // optional member `usecenter`
  bool use = true;                           // this appears in a json file

public:
  void setTransform(const Matrix4f &);
  void setGridSize(unsigned int);
  void setUseCenter(bool);
  void setIsUse(bool);
  Type type() const;
  Vector3f id() const;
  Vector3f is() const;
  bool isUse() const;
};

class AreaLight : public Light {
public:
  AreaLight(const Vector3f &id, const Vector3f &is, const Vector3f &p1,
            const Vector3f &p2, const Vector3f &p3, const Vector3f &p4)
      : Light(Type::Area, id, is), p1(p1), p2(p2), p3(p3), p4(p4) {}

  virtual Vector3f illumination(const HitRecord &,
                                const vector<Geometry *> &) const override;

private:
  Vector3f p1, p2, p3, p4;

public:
  Vector3f getCenter() const override;
};

class PointLight : public Light {
public:
  PointLight(const Vector3f &id, const Vector3f &is, const Vector3f &center)
      : Light(Type::Point, id, is), center(center) {}

  PointLight(const AreaLight &al, const Vector3f &center)
      : PointLight(al.id(), al.is(), center) {}

  virtual Vector3f illumination(const HitRecord &,
                                const vector<Geometry *> &) const override;

private:
  Vector3f center;

public:
  Vector3f getCenter() const override;
};

#endif // !LIGHT_H_
