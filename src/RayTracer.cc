#include "RayTracer.h"
#include "../external/simpleppm.h"
#include "Parser.h"
#include "Ray.h"

void RayTracer::parse() {
  for (auto i = json["output"].begin(); i != json["output"].end(); ++i)
    scenes.push_back(Parser::getScene(*i));

  for (auto i = json["geometry"].begin(); i != json["geometry"].end(); ++i)
    geometries.push_back(Parser::getGeometry(*i));

  for (auto i = json["light"].begin(); i != json["light"].end(); ++i)
    lights.push_back(Parser::getLight(*i));
}

Ray getRay(int x, int y, const Vector3f &camPos, const Vector3f &lookat,
           float fov, int width, int height) {
  // TODO: compute ray
  return Ray(Vector3f(), Vector3f());
}

void RayTracer::render(Scene *scene) {
  int width = scene->getWidth();
  int height = scene->getHeight();
  float fov = scene->getFov();
  Vector3f cameraPos = scene->getCenter();
  Vector3f lookAt = scene->getLookAt();
  Vector3f up = scene->getUpVector();

  Buffer buffer(width * height * 3);
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      Ray ray = getRay(x, y, cameraPos, lookAt, fov, width, height);

      for (auto geometry : geometries)
        if (geometry->intersect(ray)) {
          buffer[3 * y * width + 3 * x + 0] = 1;
          buffer[3 * y * width + 3 * x + 1] = 1;
          buffer[3 * y * width + 3 * x + 2] = 1;
          break;
        }
    }

  Task *task = new Task(scene, buffer);
  tasks.push_back(task);
}

void RayTracer::output(Task *task) {
  string path = task->first->getName();
  int width = task->first->getWidth();
  int height = task->first->getHeight();

  save_ppm(path, task->second, width, height);
}

void RayTracer::run() {
  parse();

  for (auto scene : scenes)
    render(scene);

  for (auto task : tasks)
    output(task);
}
