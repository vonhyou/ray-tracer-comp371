#include "RayTracer.h"
#include "HitRecord.h"
#include "Light.h"
#include "Optional.h"
#include "Output.h"
#include "Parser.h"
#include "Progress.h"
#include "Random.h"
#include "Ray.h"

#include <Eigen/Core>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>

using Eigen::VectorXi;
using std::priority_queue;

// help function declarations
Ray getRay(int, int);
Ray getRay(int, int, int, int);
void writeColor(int, const Vector3f &);
utils::Optional<Vector3f> trace(Ray r);
Vector3f clamp(const Vector3f &);

namespace camera {
int width, height, gridWidth, gridHeight, raysPerPixel;
Vector3f pos, u, v, du, dv, vpUpperLeft, pxUpperLeft, gdu, gdv;

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

      if (Scene::current->globalIllum()) {
        int success = 0;
        Vector3f accumulate = Vector3f::Zero();
        for (int j = 0; j < gridHeight; ++j)
          for (int i = 0; i < gridWidth; ++i) {
            Ray ray = getRay(x, y, i, j);
            for (int rayNum = 0; rayNum < raysPerPixel; ++rayNum) {
              utils::Optional<Vector3f> result = trace(ray);
              if (result.hasValue()) {
                accumulate += result.value();
                success++;
              }
            }
          }

        if (success)
          color = accumulate / success;
      } else {
        Ray ray = getRay(x, y);
        Optional<HitRecord> hitRecord = getHitRecord(ray);

        if (hitRecord.hasValue()) {
          HitRecord hit = hitRecord.value();
          color = calculateColor(hit, y * width + x);
        }
      }
      writeColor(y * width + x, clamp(color));
    }
  }
  std::cout << std::endl;
}

/**
 * Calculate color using phong model
 */
Vector3f RayTracer::calculateColor(const HitRecord &hit, int i) const {
  Vector3f result(0, 0, 0);
  for (auto light : lights)
    result += light->isUse() ? light->illumination(hit, geometries)
                             : Vector3f::Zero();

  return result;
}

/**
 * Find the nearest geometry to intersect
 */
Optional<HitRecord> RayTracer::getHitRecord(Ray r, const Geometry *self,
                                            bool notSphere) const {
  priority_queue<HitRecord> records;
  for (auto g : geometries) {
    Optional<float> t = g->intersect(r);
    if (t.hasValue() && g != self)
      if (!notSphere || notSphere && g->type() != Geometry::Type::SPHERE)
        records.push(HitRecord(t.value(), r, g));
  }

  if (!records.empty()) {
    HitRecord result = records.top();
    result.calcNormal();
    return Optional<HitRecord>(result);
  }

  return Optional<HitRecord>::nullopt;
}

Optional<HitRecord> RayTracer::getHitRecord(Ray r, const Geometry *g) const {
  return getHitRecord(r, g, false);
}

Optional<HitRecord> RayTracer::getHitRecord(Ray r) const {
  return getHitRecord(r, nullptr, false);
}

Light *RayTracer::singleLightSource() const {
  for (auto light : lights)
    if (light->isUse())
      return light;
  return nullptr;
}

// helper functions
Ray getRay(int x, int y) {
  using namespace camera;
  return Ray(pos, pxUpperLeft + x * du + y * dv - pos);
}

Ray getRay(int x, int y, int i, int j) {
  using namespace camera;
  return Ray(pos, vpUpperLeft + x * du + i * gdu + y * dv + j * gdv - pos);
}

Vector3f clamp(const Vector3f &color) {
  return color.cwiseMax(0.0f).cwiseMin(1.0f);
}

void writeColor(int i, const Vector3f &color) {
  Output::current->r(i, color.x());
  Output::current->g(i, color.y());
  Output::current->b(i, color.z());
}

Vector3f getRandomDirection() {
RETRY_RANDOM:
  float x = utils::Random::get();
  float y = utils::Random::get();
  if (x * x + y * y > 1)
    goto RETRY_RANDOM;

  return Vector3f(x, y, std::sqrt(1 - x * x - y * y));
}

Vector3f RayTracer::trace(HitRecord hit, int bounce, float prob) const {
  bool notFinish = bounce && (utils::Random::get() > prob);
  Vector3f point = hit.point();
  Light *light = singleLightSource();
  Vector3f direction;
  Geometry *geometry = hit.geometry();

  if (notFinish)
    direction = point + getRandomDirection();
  else
    direction = light->getCenter() - point;

  direction.normalize();
  Ray ray(point, direction);

  Optional<HitRecord> hitRecord = getHitRecord(ray, geometry, !notFinish);
  Vector3f traceColor = Vector3f::Zero();
  if (notFinish && hitRecord.hasValue())
    traceColor = trace(hitRecord.value(), bounce - 1, prob);
  else if (!notFinish)
    traceColor = light->id();

  return traceColor.array() * geometry->cd().array() *
         std::max(0.0f, hit.normal().dot(direction));
}

utils::Optional<Vector3f> RayTracer::trace(Ray r) const {
  Optional<HitRecord> hitRecord = getHitRecord(r);
  if (hitRecord.hasValue()) {
    Vector3f color = trace(hitRecord.value(), Scene::current->maxBounce(),
                           Scene::current->probTerminate());

    if (color != Vector3f::Zero())
      return utils::Optional<Vector3f>(color);
  }

  return utils::Optional<Vector3f>::nullopt;
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

  VectorXi data = Scene::current->raysPerPixel();
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
