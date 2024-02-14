#include "Scene.h"

void Scene::setRaysPerPixel(const Eigen::VectorXi &raysPerPixel) {
  this->raysPerPixel = raysPerPixel;
}

void Scene::setAntialiasing(bool antialiasing) {
  this->antialiasing = antialiasing;
}

void Scene::setTwoSideRender(bool twoSideRender) {
  this->twoSideRender = twoSideRender;
}

void Scene::setGlobalIllum(bool globalIllum) {
  this->globalIllum = globalIllum;
}
