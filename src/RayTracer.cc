#include "RayTracer.h"
#include "../external/simpleppm.h"
#include "Parser.h"

#include <vector>

using std::vector;

void RayTracer::parse() {
  for (auto i = json["output"].begin(); i != json["output"].end(); ++i)
    scenes.push_back(Parser::getScene(*i));
}

void RayTracer::render() {}

void RayTracer::output() {
  for (auto scene : scenes) {
    int width = scene->getWidth();
    int height = scene->getHeight();
    vector<double> buffer(3 * width * height);
    save_ppm(scene->getName(), buffer, width, height);
  }
}

void RayTracer::run() {
  parse();
  render();
  output();
}
