#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "Optional.h"
#include "Ray.h"

#include <Eigen/Core>
#include <Eigen/Dense>

using Eigen::Matrix;
using Eigen::Matrix4f;
using Eigen::Vector3f;
using utils::Optional;

// Abstract class for Geometries
class Geometry {
public:
  enum class Type { SPHERE, RECTANGLE };

  virtual ~Geometry() = default;
  virtual Optional<float> intersect(const Ray &) const = 0;
  virtual Vector3f normal(const Vector3f &) const = 0;

protected:
  Geometry(Type type, float ka, float kd, float ks, const Vector3f &ca,
           const Vector3f &cd, const Vector3f &cs, float pc)
      : type_(type), ka_(ka), kd_(kd), ks_(ks), ca_(ca), cd_(cd), cs_(cs),
        phong_(pc) {}

  Type type_;
  float ka_, kd_, ks_;    // coefficients for ambient, diffuse and specular
  Vector3f ca_, cd_, cs_; // ambient, diffuse and specular reflection color
  float phong_;           // phone coefficient, for `pc`
  Matrix4f transform = Matrix4f::Identity();

public:
  Vector3f cd() const;
  Vector3f cs() const;
  Vector3f ca() const;
  float kd() const;
  float ks() const;
  float ka() const;
  float phong() const;
  Type type() const;
  void setTransform(const Matrix4f &);
};

class Sphere : public Geometry {
public:
  Sphere(float ka, float kd, float ks, const Vector3f &ca, const Vector3f &cd,
         const Vector3f &cs, float pc, float radius, const Vector3f &center)
      : Geometry(Type::SPHERE, ka, kd, ks, ca, cd, cs, pc), radius(radius),
        center(center) {}

  Optional<float> intersect(const Ray &) const override;
  Vector3f normal(const Vector3f &) const override;

private:
  float radius;
  Vector3f center;
};

class Rectangle : public Geometry {
public:
  Rectangle(float ka, float kd, float ks, const Vector3f &ca, const Vector3f cd,
            const Vector3f &cs, float pc, const Vector3f &p1,
            const Vector3f &p2, const Vector3f &p3, const Vector3f &p4)
      : Geometry(Type::RECTANGLE, ka, kd, ks, ca, cd, cs, pc), p1(p1), p2(p2),
        p3(p3), p4(p4), normal_((p2 - p1).cross(p3 - p1).normalized()) {}

  Optional<float> intersect(const Ray &) const override;
  Vector3f normal(const Vector3f &) const override;

private:
  Vector3f p1, p2, p3, p4;
  Vector3f normal_;
};

#endif // !GEOMETRY_H_
