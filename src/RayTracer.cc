#include "RayTracer.h"
#include "Parser.h"

void RayTracer::parse() {
  for (auto i = json["output"].begin(); i != json["output"].end(); ++i)
    scenes.push_back(Parser::getScene(*i));
}

void RayTracer::render() {}

void RayTracer::output() {}

void RayTracer::run() {
  parse();
  render();
  output();
}
