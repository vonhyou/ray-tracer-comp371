#include "RayTracer.h"
#include "HitRecord.h"
#include "Output.h"
#include "Parser.h"
#include "Progress.h"
#include "Ray.h"

#include <Eigen/Core>
#include <cmath>
#include <iostream>
#include <queue>

using Eigen::VectorXi;
using std::priority_queue;

// help function declarations
int getGridWidth(VectorXi);
int getGridHeight(VectorXi);
int getRayNumber(VectorXi);
Ray getRay(int, int, const Vector3f &, const Vector3f &, const Vector3f &);
Ray getRay(int, int, int, int, const Vector3f &, const Vector3f &,
           const Vector3f &, const Vector3f &, const Vector3f &);
void writeColor(int, const Vector3f &);
Vector3f trace();

namespace camera {
int width, height;
Vector3f position, u, v, du, dv, vpUpperLeft, pxUpperLeft;

void init();
} // namespace camera

void RayTracer::run() {
  parse();

  for (auto scene : scenes) {
    Scene::current = scene;
    render();
    Output::current->write();
  }
}

void RayTracer::parse() {
  for (auto i = json["output"].begin(); i != json["output"].end(); ++i)
    scenes.push_back(Parser::getScene(*i));

  for (auto i = json["geometry"].begin(); i != json["geometry"].end(); ++i)
    geometries.push_back(Parser::getGeometry(*i));

  for (auto i = json["light"].begin(); i != json["light"].end(); ++i)
    lights.push_back(Parser::getLight(*i));
}

void RayTracer::render() {
  camera::init();

  Output::current =
      new Output(Scene::current->name(), camera::width, camera::height);

  VectorXi data = Scene::current->raysPerPixel();
  int gridWidth = getGridWidth(data);
  int gridHeight = getGridHeight(data);
  int raysPerPixel = getRayNumber(data);

  Vector3f gdu = Vector3f::Zero();
  Vector3f gdv = Vector3f::Zero();
  if (gridWidth > 1 || gridHeight > 1) {
    gdu = camera::du / gridWidth;
    gdv = camera::dv / gridHeight;
  }

  for (int y = 0; y < camera::height; ++y) {
    // print progress bar
    utils::Progress::of((y + 1.0f) / camera::height);

    for (int x = 0; x < camera::width; ++x) {
      Vector3f color = Scene::current->backgroundColor();

      if (Scene::current->globalIllum()) {
        for (int j = 0; j < gridHeight; ++j)
          for (int i = 0; i < gridWidth; ++i) {
            Ray ray = getRay(x, y, i, j, camera::vpUpperLeft, camera::du, gdu,
                             camera::dv, gdv);
            color = trace();
          }
      } else {
        Ray ray = getRay(x, y, camera::pxUpperLeft, camera::du, camera::dv);
        priority_queue<HitRecord> records;
        for (auto g : geometries) {
          Optional<float> t = g->intersect(ray);
          if (t.hasValue())
            records.push(HitRecord(t.value(), ray, g));
        }

        if (!records.empty()) {
          HitRecord hit = records.top();
          hit.calcNormal();
          color = calculateColor(hit, y * camera::width + x);
        }
      }
      writeColor(y * camera::width + x, color);
    }
  }
  std::cout << std::endl;
}

Vector3f RayTracer::calculateColor(const HitRecord &hit, int i) const {
  Vector3f result(0, 0, 0);
  for (auto light : lights)
    result += light->isUse() ? light->illumination(hit, geometries)
                             : Vector3f::Zero();

  return result.cwiseMax(0.0f).cwiseMin(1.0f);
}

// helper functions
int getGridWidth(VectorXi data) {
  return data.size() != 2 && data.size() != 3 ? 1 : data.x();
}

int getGridHeight(VectorXi data) {
  return data.size() == 2 ? data.x() : (data.size() == 3 ? data.y() : 1);
}

int getRayNumber(VectorXi data) {
  return data.size() == 2 ? data.y() : (data.size() == 3 ? data.z() : 1);
}

Ray getRay(int x, int y, const Vector3f &upperLeft, const Vector3f &du,
           const Vector3f &dv) {
  return Ray(camera::position, upperLeft + x * du + y * dv - camera::position);
}

Ray getRay(int x, int y, int i, int j, const Vector3f &upperLeft,
           const Vector3f &du, const Vector3f &gdu, const Vector3f &dv,
           const Vector3f &gdv) {
  Vector3f camPos = Scene::current->center();
  return Ray(camPos, upperLeft + x * du + i * gdu + y * dv + j * gdv - camPos);
}

void writeColor(int i, const Vector3f &color) {
  Output::current->r(i, color.x());
  Output::current->g(i, color.y());
  Output::current->b(i, color.z());
}

Vector3f trace() { return Vector3f::Zero(); }

namespace camera {
void init() {
  width = Scene::current->width();
  height = Scene::current->height();
  position = Scene::current->center();
  Vector3f lookAt = Scene::current->lookAt();
  float vpHeight =
      2 * tan(Scene::current->fov() / 180 * M_PI / 2) * lookAt.norm();
  float vpWidth = vpHeight * camera::width / camera::height;
  u = Vector3f(vpWidth, 0, 0);
  v = Vector3f(0, -vpHeight, 0);
  du = u / camera::width;
  dv = v / camera::height;
  vpUpperLeft = camera::position + lookAt - u / 2.0 - v / 2.0;
  pxUpperLeft = vpUpperLeft + (du + dv) / 2.0;
}
} // namespace camera
