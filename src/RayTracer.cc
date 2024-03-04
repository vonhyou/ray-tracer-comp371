#include "RayTracer.h"
#include "../external/simpleppm.h"
#include "HitRecord.h"
#include "Output.h"
#include "Parser.h"
#include "Ray.h"

#include <Eigen/Core>
#include <cmath>
#include <queue>

using std::priority_queue;

void RayTracer::parse() {
  for (auto i = json["output"].begin(); i != json["output"].end(); ++i)
    scenes.push_back(Parser::getScene(*i));

  for (auto i = json["geometry"].begin(); i != json["geometry"].end(); ++i)
    geometries.push_back(Parser::getGeometry(*i));

  for (auto i = json["light"].begin(); i != json["light"].end(); ++i)
    lights.push_back(Parser::getLight(*i));
}

Ray getRay(int x, int y, const Vector3f &camPos, const Vector3f &pxUpperLeft,
           const Vector3f &du, const Vector3f &dv) {
  return Ray(camPos, pxUpperLeft + x * du + y * dv - camPos);
}

void RayTracer::calculateColor(const HitRecord &hit, Output *buffer, int i) {
  buffer->r(i, 0);
  buffer->g(i, 0);
  buffer->b(i, 0);

  for (auto light : lights)
    if (light->isUse()) {
      Vector3f contribution = light->illumination(hit, geometries);
      buffer->r(i, buffer->r(i) + contribution.x());
      buffer->g(i, buffer->g(i) + contribution.y());
      buffer->b(i, buffer->b(i) + contribution.z());
    }
}

void RayTracer::render(Scene *scene) {
  int width = scene->getWidth();
  int height = scene->getHeight();
  Vector3f cameraPos = scene->getCenter();
  Vector3f lookAt = scene->getLookAt();
  float vpHeight = 2 * tan(scene->getFov() / 180 * M_PI / 2) * lookAt.norm();
  float vpWidth = vpHeight * width / height;
  Vector3f vpU = Vector3f(vpWidth, 0, 0);
  Vector3f vpV = Vector3f(0, -vpHeight, 0);
  Vector3f du = vpU / width;
  Vector3f dv = vpV / height;

  Vector3f vpUpperLeft = cameraPos + lookAt - vpU / 2.0 - vpV / 2.0;
  Vector3f pxUpperLeft = vpUpperLeft + (du + dv) / 2.0;

  Output *buffer =
      new Output(scene->getBackgroundColor(), scene->getName(), width, height);

  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      Ray ray = getRay(x, y, cameraPos, pxUpperLeft, du, dv);
      priority_queue<HitRecord> records;
      for (auto g : geometries) {
        Optional<float> t = g->intersect(ray);
        if (t.hasValue())
          records.push(HitRecord(t.value(), ray, g));
      }

      if (!records.empty()) {
        HitRecord hit = records.top();
        hit.calcNormal();
        calculateColor(hit, buffer, y * width + x);
      }
    }

  outputs.push_back(buffer);
}

void RayTracer::output() {
  for (auto output : outputs)
    output->write();
}

void RayTracer::run() {
  parse();

  for (auto scene : scenes)
    render(scene);

  output();
}
