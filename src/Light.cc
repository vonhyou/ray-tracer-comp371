#include "Light.h"

void Light::setTransform(const Matrix4f &transform) {
  this->transform = transform;
}

void Light::setGridSize(unsigned int gridSize) { this->gridSize = gridSize; }

void Light::setUseCenter(bool useCenter) { this->useCenter = useCenter; }

void PointLight::illumination() const {}

void AreaLight::illumination() const {}
