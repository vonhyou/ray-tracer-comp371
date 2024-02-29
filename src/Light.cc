#include "Light.h"

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

  return Vector3f::Zero();
}

Vector3f AreaLight::illumination(const HitRecord &hit,
                                 const vector<Geometry *> &geometries) const {
  return Vector3f::Zero();
}
