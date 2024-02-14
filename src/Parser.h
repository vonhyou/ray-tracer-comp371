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
  static PointLight *getPointLight(const nlohmann::json &);
  static AreaLight *getAreaLight(const nlohmann::json &);
  static Rectangle *getRectangle(const nlohmann::json &);
  static Sphere *getSphere(const nlohmann::json &);
};

#endif // !PARSER_H_
