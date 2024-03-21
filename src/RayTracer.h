#ifndef RAY_TRACER_H_
#define RAY_TRACER_H_

#include "../external/json.hpp"
#include "Geometry.h"
#include "HitRecord.h"
#include "Light.h"
#include "Output.h"
#include "Scene.h"

#include <vector>

using std::vector;

class RayTracer {
public:
  RayTracer(const nlohmann::json &j) : json(j) {}
  void run();

private:
  nlohmann::json json;
  vector<Scene *> scenes;
  vector<Light *> lights;
  vector<Geometry *> geometries;
  vector<Output *> outputs;

  void parse();
  void render();
  Optional<HitRecord> getHitRecord(Ray, const Geometry *) const;
  Optional<HitRecord> getHitRecord(Ray) const;
  Vector3f calculateColor(const HitRecord &, int) const;
  Light *singleLightSource() const;
  Optional<Vector3f> trace(Ray) const;
  Vector3f trace(HitRecord, int, float) const;
};

#endif // !RAY_TRACER_H_
