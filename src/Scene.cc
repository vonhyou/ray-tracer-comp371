#include "Scene.h"

Scene *Scene::current = nullptr;

string Scene::name() const { return name_; }

int Scene::width() { return width_; }

int Scene::height() { return height_; }

float Scene::fov() { return fov_; }

bool Scene::globalIllum() { return globalIllum_; }

int Scene::maxBounce() { return maxBounce_; }

float Scene::probTerminate() { return probTerminate_; }

Eigen::VectorXi Scene::raysPerPixel() const { return raysPerPixel_; }

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

void Scene::setMaxBounce(int maxBounce) { this->maxBounce_ = maxBounce; }

void Scene::setProbTerminate(float probTerminate) {
  this->probTerminate_ = probTerminate;
}
