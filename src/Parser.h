#ifndef PARSER_H_
#define PARSER_H_

#include "../external/json.hpp"

#include "Geometry.h"
#include "Light.h"
#include "Scene.h"

class Parser {
public:
  static Geometry *parseGeometry(const nlohmann::json &);
  static Light *parseLight(const nlohmann::json &);
  static Scene *parseScene(const nlohmann::json &);

private:
  static PointLight *parsePointLight(const nlohmann::json &);
  static AreaLight *parseAreaLight(const nlohmann::json &);
  static Rectangle *parseRectangle(const nlohmann::json &);
  static Sphere *parseSphere(const nlohmann::json &);
};

#endif // !PARSER_H_
