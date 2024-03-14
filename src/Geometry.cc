#include "Geometry.h"

#include <Eigen/Dense>
#include <cmath>

Vector3f Geometry::cd() const { return cd_; }
Vector3f Geometry::cs() const { return cs_; }
Vector3f Geometry::ca() const { return ca_; }
float Geometry::kd() const { return kd_; }
float Geometry::ks() const { return ks_; }
float Geometry::ka() const { return ka_; }
float Geometry::phong() const { return phong_; }
Geometry::Type Geometry::type() const { return type_; }

void Geometry::setTransform(const Matrix4f &transform) {
  this->transform = transform;
}

Optional<float> Sphere::intersect(const Ray &r) const {
  Vector3f originCenter = r.origin() - center;
  float a = r.direction().dot(r.direction());
  float b = 2.0f * originCenter.dot(r.direction());
  float c = originCenter.dot(originCenter) - radius * radius;

  float delta = b * b - 4 * a * c;
  if (delta >= 0) {
    float t1 = (-b + sqrt(delta)) / 2.0f / a;
    float t2 = (-b - sqrt(delta)) / 2.0f / a;
    float mint = std::min(t1, t2);
    float maxt = std::max(t1, t2);
    return maxt <= 0
               ? Optional<float>::nullopt
               : (mint < 0 ? Optional<float>(maxt) : Optional<float>(mint));
  }

  return Optional<float>::nullopt;
}

Vector3f Sphere::normal(const Vector3f &p) const {
  return (p - center).normalized();
}

bool isInRectangle(const Vector3f &p, const Vector3f &a, const Vector3f &b,
                   const Vector3f &c, const Vector3f &d, const Vector3f &n) {
  float s1 = (b - a).cross(p - a).dot(n);
  float s2 = (c - b).cross(p - b).dot(n);
  float s3 = (d - c).cross(p - c).dot(n);
  float s4 = (a - d).cross(p - d).dot(n);

  return (s1 >= 0 && s2 >= 0 && s3 >= 0 && s4 >= 0) ||
         (s1 <= 0 && s2 <= 0 && s3 <= 0 && s4 <= 0);
}

Optional<float> Rectangle::intersect(const Ray &r) const {
  float denom = normal_.dot(r.direction());
  if (abs(denom) < 1e-6f)
    return Optional<float>::nullopt;

  float t = -normal_.dot(r.origin() - p1) / denom;
  if (t <= 0)
    return Optional<float>::nullopt;

  Vector3f p = r.origin() + t * r.direction();

  return isInRectangle(p, p1, p2, p3, p4, normal_) ? Optional<float>(t)
                                                   : Optional<float>::nullopt;
}

Vector3f Rectangle::normal(const Vector3f &p) const { return normal_; }
