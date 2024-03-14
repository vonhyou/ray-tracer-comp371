#include "Scene.h"

Scene *Scene::current = nullptr;

string Scene::name() const { return name_; }

int Scene::width() { return width_; }

int Scene::height() { return height_; }

float Scene::fov() { return fov_; }

Vector3f Scene::ai() const { return ai_; }

Vector3f Scene::center() const { return center_; }

Vector3f Scene::up() const { return up_; }

Vector3f Scene::lookAt() const { return lookAt_; }

Vector3f Scene::backgroundColor() const { return bgc_; }

void Scene::setRaysPerPixel(const Eigen::VectorXi &raysPerPixel) {
  this->raysPerPixel_ = raysPerPixel;
}

void Scene::setAntialiasing(bool antialiasing) {
  this->antialiasing_ = antialiasing;
}

void Scene::setTwoSideRender(bool twoSideRender) {
  this->twoSideRender_ = twoSideRender;
}

void Scene::setGlobalIllum(bool globalIllum) {
  this->globalIllum_ = globalIllum;
}
