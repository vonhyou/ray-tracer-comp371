#include "Scene.h"

Scene::Scene(const nlohmann::json &output) {
  for (auto i = output.begin(); i != output.end(); ++i) {
    if (i->contains("filename")) {
      this->name = (*i)["filename"].get<string>();
    }

    if (i->contains("size")) {
      this->width = (*i)["size"].at(0).get<int>();
      this->height = (*i)["size"].at(1).get<int>();
    }
  }
}

string Scene::getName() { return this->name; }
int Scene::getWidth() { return this->width; }
int Scene::getHeight() { return this->height; }
