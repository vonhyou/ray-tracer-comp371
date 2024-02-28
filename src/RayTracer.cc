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

void RayTracer::render(Scene *scene) {
  int width = scene->getWidth();
  int height = scene->getHeight();
  float fov = scene->getFov();
  Vector3f cameraPos = scene->getCenter();
  Vector3f lookAt = scene->getLookAt();
  Vector3f up = scene->getUpVector();
  float vpHeight = 2 * tan(fov / 180 * M_PI / 2) * lookAt.norm();
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
          records.push(HitRecord(t.value(), g));
      }

      if (!records.empty()) {
        HitRecord hit = records.top();
        Vector3f diffuse = hit.geometry()->diffuse();
        buffer->r(y * width + x, diffuse.x());
        buffer->g(y * width + x, diffuse.y());
        buffer->b(y * width + x, diffuse.z());
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
