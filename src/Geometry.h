#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <Eigen/Core>

using Eigen::Matrix4f;
using Eigen::Vector3f;

class Geometry {
public:
  enum class Type { SPHERE, RECTANGLE };

  virtual ~Geometry() = default;

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

#endif // !GEOMETRY_H_
