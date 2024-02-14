#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <Eigen/Core>

using Eigen::Matrix;
using Eigen::Matrix4f;
using Eigen::Vector3f;

// Abstract class for Geometries
class Geometry {
public:
  enum class Type { SPHERE, RECTANGLE };

  virtual ~Geometry() = default;
  virtual bool intersect() const = 0;

protected:
  Geometry(Type type, float ka, float kd, float ks, const Vector3f &ca,
           const Vector3f &cd, const Vector3f &cs, float pc)
      : type(type), ka(ka), kd(kd), ks(ks), ca(ca), cd(cd), cs(cs), phong(pc) {}

  Type type;
  float ka, kd, ks;    // coefficients for ambient, diffuse and specular
  Vector3f ca, cd, cs; // ambient, diffuse and specular reflection color
  float phong;         // phone coefficient, for `pc`
  Matrix4f transform = Matrix4f::Identity();

public:
  void setTransform(const Matrix4f &);
};

class Sphere : public Geometry {
public:
  Sphere(float ka, float kd, float ks, const Vector3f &ca, const Vector3f &cd,
         const Vector3f &cs, float pc, float radius, const Vector3f &center)
      : Geometry(Type::SPHERE, ka, kd, ks, ca, cd, cs, pc), radius(radius),
        center(center) {}

  bool intersect() const override;

private:
  float radius;
  Vector3f center;
};

class Rectangle : public Geometry {
public:
  Rectangle(float ka, float kd, float ks, const Vector3f &ca, const Vector3f cd,
            const Vector3f &cs, float pc, const Matrix<float, 3, 4> &corners)
      : Geometry(Type::RECTANGLE, ka, kd, ks, ca, cd, cs, pc),
        corners(corners) {}

  bool intersect() const override;

private:
  Matrix<float, 3, 4> corners;
};

#endif // !GEOMETRY_H_
