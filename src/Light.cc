#include "Light.h"
#include <algorithm>
#include <cmath>

void Light::setTransform(const Matrix4f &transform) {
  this->transform = transform;
}

void Light::setGridSize(unsigned int gridSize) { this->gridSize = gridSize; }

void Light::setUseCenter(bool useCenter) { this->useCenter = useCenter; }

Vector3f PointLight::illumination(const HitRecord &hit,
                                  const vector<Geometry *> &geometries) const {
  Vector3f shadingPoint = hit.getPoint();
  Vector3f rayDirection = (center - shadingPoint).normalized();
  Geometry *geometry = hit.geometry();
  Ray shadowRay(shadingPoint, rayDirection);

  for (auto g : geometries)
    if (g != geometry && g->intersect(shadowRay).hasValue())
      return Vector3f::Zero();

  float distance = (center - shadingPoint).norm();
  float att = 1.0f / distance / distance;

  Vector3f ambient_ = geometry->coefAmbient() * geometry->ambient();
  Vector3f diffuse_ = att * geometry->coefDiffuse() * diffuse *
                      std::max(0.0f, hit.normal().dot(rayDirection));

  Vector3f halfWay = (hit.viewDirection() + rayDirection).normalized();
  Vector3f specular_ =
      att * geometry->coefSpecular() * specular *
      pow(std::max(0.0f, hit.normal().dot(halfWay)), geometry->getPhong());
  return ambient_ + diffuse_ + specular_;
}

Vector3f AreaLight::illumination(const HitRecord &hit,
                                 const vector<Geometry *> &geometries) const {
  return Vector3f::Zero();
}
