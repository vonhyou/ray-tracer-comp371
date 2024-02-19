#include "Scene.h"

string Scene::getName() { return name; }

int Scene::getWidth() { return width; }

int Scene::getHeight() { return height; }

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
