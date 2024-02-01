#ifndef RAY_TRACER_H_
#define RAY_TRACER_H_

#include "../external/json.hpp"

class RayTracer {
public:
  RayTracer(nlohmann::json);
  void run();

private:
  nlohmann::json scene;
};

#endif // !RAY_TRACER_H_
