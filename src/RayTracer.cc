#include "RayTracer.h"
#include "../external/simpleppm.h"
#include "Parser.h"
#include "Scene.h"

void RayTracer::run() {
  nlohmann::json sceneJson = this->json["output"][0];
  Scene *sc = Parser::getScene(sceneJson);
}
