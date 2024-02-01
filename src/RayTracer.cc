#include "RayTracer.h"
#include "../external/simpleppm.h"
#include "Scene.h"

#include <vector>

RayTracer::RayTracer(nlohmann::json json)
    : json(json), scene(Scene(json["output"])) {}

void RayTracer::render() {
  int width = scene.getWidth();
  int height = scene.getHeight();

  std::vector<double> buffer(3 * width * height);
  save_ppm(scene.getName(), buffer, width, height);
}

void RayTracer::run() { render(); }
