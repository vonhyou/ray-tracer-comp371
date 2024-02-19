#ifndef RAY_TRACER_H_
#define RAY_TRACER_H_

#include "../external/json.hpp"
#include "Geometry.h"
#include "Light.h"
#include "Scene.h"

#include <vector>

class RayTracer {
public:
  RayTracer(const nlohmann::json &j) : json(j) {}
  void run();

private:
  nlohmann::json json;
  std::vector<Scene *> scenes;
  std::vector<Light *> lights;
  std::vector<Geometry *> geometries;

  void parse();
  void render();
  void output();
};

#endif // !RAY_TRACER_H_
