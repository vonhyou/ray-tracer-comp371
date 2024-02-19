#include "RayTracer.h"
#include "../external/simpleppm.h"
#include "Parser.h"

void RayTracer::parse() {
  for (auto i = json["output"].begin(); i != json["output"].end(); ++i)
    scenes.push_back(Parser::getScene(*i));

  for (auto i = json["geometry"].begin(); i != json["geometry"].end(); ++i)
    geometries.push_back(Parser::getGeometry(*i));

  for (auto i = json["light"].begin(); i != json["light"].end(); ++i)
    lights.push_back(Parser::getLight(*i));
}

void RayTracer::render(Scene *scene) {
  int width = scene->getWidth();
  int height = scene->getHeight();
  Buffer buffer(width * height * 3);
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
