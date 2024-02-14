#ifndef LIGHT_H_
#define LIGHT_H_

#include <Eigen/Core>

using Eigen::Matrix;
using Eigen::Matrix4f;
using Eigen::Vector3f;

// Abstract base class for Lights
class Light {
public:
  enum class Type { Point, Area };

  virtual ~Light() = default;
  virtual void illumination() const = 0;

protected:
  Light(Type type, const Vector3f &id, const Vector3f &is)
      : type(type), diffuse(id), specular(is) {}

  Type type;
  Vector3f diffuse;
  Vector3f specular;
  Matrix4f transform = Matrix4f::Identity(); // optional member `transform`
  unsigned int gridSize = 0;                 // optional member `n`
  bool useCenter = false;                    // optional member `usecenter`

public:
  // setters for optional members
  void setTransform(const Matrix4f &);
  void setGridSize(unsigned int);
  void setUseCenter(bool);
};

class PointLight : public Light {
public:
  PointLight(const Vector3f &id, const Vector3f &is, Vector3f &center)
      : Light(Type::Point, id, is), center(center) {}

  virtual void illumination() const override;

private:
  Vector3f center;
};

class AreaLight : public Light {
public:
  AreaLight(const Vector3f &id, const Vector3f &is,
            const Matrix<float, 3, 4> &corners)
      : Light(Type::Area, id, is), corners(corners) {}

  virtual void illumination() const override;

private:
  Matrix<float, 3, 4> corners; // stores `p1`, `p2`, `p3` and `p4`
};

#endif // !LIGHT_H_
