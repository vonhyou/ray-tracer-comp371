#ifndef RAY_TRACER_H_
#define RAY_TRACER_H_

#include "../external/json.hpp"

class RayTracer {
public:
  RayTracer(const nlohmann::json &j) : json(j) {}
  void run();

private:
  nlohmann::json json;
};

#endif // !RAY_TRACER_H_
