#ifndef RAY_TRACER_H_
#define RAY_TRACER_H_

#include "../external/json.hpp"
#include "Scene.h"

class RayTracer {
public:
  RayTracer(const nlohmann::json &);
  void render();
  void run();

private:
  Scene scene;
  nlohmann::json json;
};

#endif // !RAY_TRACER_H_
