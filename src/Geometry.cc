#include "Geometry.h"

#include <Eigen/Dense>
#include <cmath>

Vector3f Geometry::diffuse() const { return cd; }
Vector3f Geometry::specular() const { return cs; }
Vector3f Geometry::ambient() const { return ca; }

void Geometry::setTransform(const Matrix4f &transform) {
  this->transform = transform;
}

Optional<float> Sphere::intersect(const Ray &r) const {
  Vector3f originCenter = r.getOrigin() - center;
  float a = r.getDirection().dot(r.getDirection());
  float b = 2.0f * originCenter.dot(r.getDirection());
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
  Vector3f normal = (p2 - p1).cross(p3 - p1).normalized();

  float denom = normal.dot(r.getDirection());
  if (abs(denom) < 1e-6f)
    return Optional<float>::nullopt;

  float t = -normal.dot(r.getOrigin() - p1) / denom;
  if (t <= 0)
    return Optional<float>::nullopt;

  Vector3f p = r.getOrigin() + t * r.getDirection();

  return isInRectangle(p, p1, p2, p3, p4, normal) ? Optional<float>(t)
                                                  : Optional<float>::nullopt;
}
