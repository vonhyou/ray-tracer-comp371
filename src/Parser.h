#ifndef PARSER_H_
#define PARSER_H_

#include "../external/json.hpp"

#include "Geometry.h"
#include "Light.h"
#include "Scene.h"

class Parser {
public:
  static Geometry *getGeometry(const nlohmann::json &);
  static Light *getLight(const nlohmann::json &);
  static Scene *getScene(const nlohmann::json &);

private:
  static PointLight *getPointLight(const nlohmann::json &, const Vector3f &,
                                   const Vector3f &);
  static AreaLight *getAreaLight(const nlohmann::json &, const Vector3f &,
                                 const Vector3f &);
  static Rectangle *getRectangle(const nlohmann::json &, float, float, float,
                                 const Vector3f &, const Vector3f &,
                                 const Vector3f &, float);
  static Sphere *getSphere(const nlohmann::json &, float, float, float,
                           const Vector3f &, const Vector3f &, const Vector3f &,
                           float);
};

#endif // !PARSER_H_
