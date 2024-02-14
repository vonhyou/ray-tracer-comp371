#include "Parser.h"
#include <string>

static Scene *parseScene(const nlohmann::json &j) {
  string name = j["filename"].get<string>();
}
