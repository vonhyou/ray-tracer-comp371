#include "Light.h"
#include <algorithm>
#include <cmath>

void Light::setTransform(const Matrix4f &transform) {
  this->transform = transform;
}

void Light::setGridSize(unsigned int gridSize) { this->gridSize = gridSize; }

void Light::setUseCenter(bool useCenter) { this->useCenter = useCenter; }

void Light::setIsUse(bool isUse) { this->use = isUse; }

Vector3f Light::getDiffuse() const { return diffuse; }

Vector3f Light::getSpecular() const { return specular; }

bool Light::isUse() const { return use; }

Vector3f PointLight::illumination(const HitRecord &hit,
                                  const vector<Geometry *> &geometries) const {
  Vector3f shadingPoint = hit.getPoint();
  Vector3f rayDirection = (center - shadingPoint).normalized();
  Geometry *geometry = hit.geometry();
  Ray shadowRay(shadingPoint, rayDirection);

  for (auto g : geometries)
    if (g != geometry && g->intersect(shadowRay).hasValue() &&
        g->type() == Geometry::Type::SPHERE)
      return Vector3f::Zero();

  Vector3f ambient_ = geometry->ka() * geometry->ca();

  Vector3f diffuse_ = geometry->kd() * geometry->cd().array() *
                      diffuse.array() *
                      std::max(0.0f, hit.normal().dot(rayDirection));

  Vector3f halfWay = (hit.viewDirection() + rayDirection).normalized();
  Vector3f specular_ =
      geometry->ks() * geometry->cs().array() * specular.array() *
      pow(std::max(0.0f, hit.normal().dot(halfWay)), geometry->phong());

  return specular_ + ambient_ + diffuse_;
}

Vector3f AreaLight::illumination(const HitRecord &hit,
                                 const vector<Geometry *> &geometries) const {
  Vector3f u = p4 - p1;
  Vector3f v = p2 - p1;

  Vector3f color = Vector3f::Zero();

  if (useCenter) {
    color += PointLight(*this, p1 + (u + v) / 2).illumination(hit, geometries);
  } else {
    for (int y = 0; y < gridSize; ++y)
      for (int x = 0; x < gridSize; ++x) {
        Vector3f contribution =
            PointLight(*this, p1 + (u * x + v * y) / gridSize)
                .illumination(hit, geometries);
        color += contribution;
      }
  }

  return color / gridSize / gridSize;
}
