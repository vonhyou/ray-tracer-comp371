#include "RayTracer.h"
#include "HitRecord.h"
#include "Light.h"
#include "Optional.h"
#include "Output.h"
#include "Parser.h"
#include "Progress.h"
#include "Ray.h"

#include <Eigen/Core>
#include <Eigen/src/Core/Matrix.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <random>

using Eigen::VectorXi;
using std::priority_queue;

// help function declarations
Ray getRay(int, int, int, int);
void writeColor(int, const Vector3f &);
utils::Optional<Vector3f> trace(Ray r);
Vector3f clamp(const Vector3f &);

namespace camera {
int width, height, gridWidth, gridHeight, raysPerPixel;
Vector3f pos, u, v, du, dv, vpUpperLeft, pxUpperLeft, gdu, gdv;
bool globalIllum, antiAliasing;

void init();
} // namespace camera

/**
 * Student solution starts here
 *
 * The main.cpp provided by instructor will invoke this function
 */
void RayTracer::run() {
  parse();

  for (auto scene : scenes) {
    if (Scene::current != nullptr)
      delete Scene::current;

    Scene::current = scene;
    render();

    Output::current->write();
  }
}

/**
 * Parse the scene stored in the json file
 *
 * Example scene files are in `assets` folder
 */
void RayTracer::parse() {
  for (auto i = json["output"].begin(); i != json["output"].end(); ++i)
    scenes.push_back(Parser::getScene(*i));

  for (auto i = json["geometry"].begin(); i != json["geometry"].end(); ++i)
    geometries.push_back(Parser::getGeometry(*i));

  for (auto i = json["light"].begin(); i != json["light"].end(); ++i)
    lights.push_back(Parser::getLight(*i));
}

Vector3f gammaCorrection(Vector3f color, float gammaInv) {
  return Vector3f(std::pow(color.x(), gammaInv), std::pow(color.y(), gammaInv),
                  std::pow(color.z(), gammaInv));
}

/**
 * Render the current scene
 *
 * For direction illumination and anti-aliasing, render by phong model
 * for global illumination, use path-tracing method.
 *
 * (*) Global illumination will not work with anti-aliasing by the requirement
 */
void RayTracer::render() {
  camera::init();

  using namespace camera;
  Output::current = new Output(Scene::current->name(), width, height);

  for (int y = 0; y < height; ++y) {
    utils::Progress::of((y + 1.0f) / height);

    for (int x = 0; x < width; ++x) {
      Vector3f color = Scene::current->backgroundColor();

      int success = 0;
      Vector3f accumulate = Vector3f::Zero();
      for (int j = 0; j < gridHeight; ++j)
        for (int i = 0; i < gridWidth; ++i) {
          for (int rayNum = 0; rayNum < raysPerPixel; ++rayNum) {
            Ray ray = getRay(x, y, i, j);
            Optional<Vector3f> result = trace(ray);
            if (result.hasValue()) {
              accumulate += result.value();
              success++;
            }
          }
        }

      if (success) {
        if (globalIllum)
          color = gammaCorrection(accumulate / success, 1.0f / 2.1f);
        else
          color = accumulate / success;
      }

      writeColor(y * width + x, clamp(color));
    }
  }
  std::cout << std::endl;
}

/**
 * Calculate color using phong model
 */
Vector3f RayTracer::calculateColor(const HitRecord &hit) const {
  Vector3f result(0, 0, 0);
  for (auto light : lights)
    result += light->isUse() ? light->illumination(hit, geometries)
                             : Vector3f::Zero();

  return result;
}

/**
 * Find the nearest geometry to intersect
 */
Optional<HitRecord> RayTracer::getHitRecord(Ray r, const Geometry *self) const {
  priority_queue<HitRecord> records;
  for (auto g : geometries) {
    Optional<float> t = g->intersect(r);
    if (t.hasValue() && g != self)
      records.push(HitRecord(t.value(), r, g));
  }

  if (!records.empty()) {
    HitRecord result = records.top();
    result.calcNormal();
    return Optional<HitRecord>(result);
  }

  return Optional<HitRecord>::nullopt;
}

Optional<HitRecord> RayTracer::getHitRecord(Ray r) const {
  return getHitRecord(r, nullptr);
}

Light *RayTracer::singleLightSource() const {
  for (auto light : lights)
    if (light->isUse())
      return light;
  return nullptr;
}

// This should generate a higher quality random number
float getRandomNumber() {
  static std::uniform_real_distribution<float> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

Ray getRay(int x, int y, int i, int j) {
  using namespace camera;
  Vector3f offset = Vector3f::Zero();

  if (globalIllum || antiAliasing)
    offset = getRandomNumber() * gdu + getRandomNumber() * gdv;

  return Ray(pos,
             vpUpperLeft + x * du + i * gdu + y * dv + j * gdv + offset - pos);
}

Vector3f clamp(const Vector3f &color) {
  return color.cwiseMax(0.0f).cwiseMin(1.0f);
}

void writeColor(int i, const Vector3f &color) {
  Output::current->r(i, color.x());
  Output::current->g(i, color.y());
  Output::current->b(i, color.z());
}

// Generate a randon point on a unit hemisphere
Vector3f getRandomDirection() {
RETRY_RANDOM:
  float x = getRandomNumber() * 2 - 1;
  float y = getRandomNumber() * 2 - 1;
  if (x * x + y * y > 1)
    goto RETRY_RANDOM;

  return Vector3f(x, y, std::sqrt(1 - x * x - y * y));
}

Vector3f getGlobalRandDirection(Vector3f normal) {
  Vector3f tangent = normal.cross(Vector3f::UnitX());
  if (tangent.norm() < 1e-6f)
    tangent = normal.cross(Vector3f::UnitY());

  tangent.normalize();
  Vector3f binormal = normal.cross(tangent);
  Eigen::Matrix3f local2World;
  local2World.col(0) = tangent;
  local2World.col(1) = binormal.normalized();
  local2World.col(2) = normal.normalized();

  return local2World * getRandomDirection();
}

// Check if a light source in on a surface (or really near)
bool lightOnSurface(HitRecord hit, const Light *l) {
  Vector3f center = l->getCenter();
  Geometry *g = hit.geometry();
  Geometry::Type type = g->type();
  if (type == Geometry::Type::RECTANGLE)
    return (g->sample() - center).dot(g->normal(center)) < 1e-5;

  return false;
}

Vector3f RayTracer::trace(HitRecord hit, int bounce, float prob) const {
RETRY_TRACING:
  bool finish = !bounce || (getRandomNumber() < prob);
  Vector3f point = hit.point();
  Light *light = singleLightSource();
  Vector3f direction;
  Geometry *geometry = hit.geometry();

  if (finish)
    direction = light->getCenter() - point;
  else
    direction = getGlobalRandDirection(hit.normal());

  direction.normalize();
  Ray ray(point + hit.normal() * 1e-6, direction);

  Optional<HitRecord> hitRecord = getHitRecord(ray, geometry);
  Vector3f traceColor = Vector3f::Zero();
  if (!finish && hitRecord.hasValue())
    traceColor = trace(hitRecord.value(), bounce - 1, prob);
  else if (!finish && !hitRecord.hasValue())
    goto RETRY_TRACING;
  else if (finish)
    if (!hitRecord.hasValue() ||
        (hitRecord.hasValue() && lightOnSurface(hitRecord.value(), light)))
      traceColor = light->id();

  return traceColor.array() * geometry->cd().array() *
         std::max(0.0f, hit.normal().dot(direction));
}

Optional<Vector3f> RayTracer::trace(Ray r) const {
  Optional<HitRecord> hitRecord = getHitRecord(r);
  if (!camera::globalIllum) {
    if (hitRecord.hasValue())
      return Optional<Vector3f>(calculateColor(hitRecord.value()));
    else if (camera::antiAliasing)
      return Optional<Vector3f>(Scene::current->backgroundColor());
  } else {
    if (hitRecord.hasValue()) {
      Vector3f color = trace(hitRecord.value(), Scene::current->maxBounce(),
                             Scene::current->probTerminate());

      return Optional<Vector3f>(color);
    }
  }

  return Optional<Vector3f>::nullopt;
}

namespace camera {
int getGridWidth(VectorXi data) {
  return data.size() != 2 && data.size() != 3 ? 1 : data.x();
}

int getGridHeight(VectorXi data) {
  return data.size() == 2 ? data.x() : (data.size() == 3 ? data.y() : 1);
}

int getRayNumber(VectorXi data) {
  return data.size() == 2 ? data.y() : (data.size() == 3 ? data.z() : 1);
}

/**
 * Initialize camera parameters
 *
 * Construct the surface for viewing the world
 */
void init() {
  width = Scene::current->width();
  height = Scene::current->height();
  pos = Scene::current->center();
  Vector3f lookAt = Scene::current->lookAt();
  float vpHeight =
      2 * tan(Scene::current->fov() / 180 * M_PI / 2) * lookAt.norm();
  float vpWidth = vpHeight * width / height;
  u = Vector3f(vpWidth, 0, 0);
  v = Vector3f(0, -vpHeight, 0);
  du = u / width;
  dv = v / height;
  vpUpperLeft = pos + lookAt - u / 2.0 - v / 2.0;
  pxUpperLeft = vpUpperLeft + (du + dv) / 2.0;

  globalIllum = Scene::current->globalIllum();
  antiAliasing = Scene::current->antiAliasing();

  VectorXi data = !(globalIllum || antiAliasing)
                      ? VectorXi(1)
                      : Scene::current->raysPerPixel();
  gridWidth = getGridWidth(data);
  gridHeight = getGridHeight(data);
  raysPerPixel = getRayNumber(data);

  gdu = Vector3f::Zero();
  gdv = Vector3f::Zero();
  if (gridWidth > 1 || gridHeight > 1) {
    gdu = du / gridWidth;
    gdv = dv / gridHeight;
  }
}
} // namespace camera
