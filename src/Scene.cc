#include "Scene.h"

string Scene::getName() const { return name; }

int Scene::getWidth() { return width; }

int Scene::getHeight() { return height; }

float Scene::getFov() { return fov; }

Vector3f Scene::getCenter() const { return center; }

Vector3f Scene::getUpVector() const { return up; }

Vector3f Scene::getLookAt() const { return lookAt; }

Vector3f Scene::getBackgroundColor() const { return backgroundColor; }

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
