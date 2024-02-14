#include "Parser.h"

#include <Eigen/Core>
#include <string>

using Eigen::Vector3f;
using Eigen::VectorXi;
using std::string;

// a helper function to get Vector3f
const Vector3f getVector3f(const nlohmann::json &j) {
  return Vector3f(j[0].get<float>(), j[1].get<float>(), j[2].get<float>());
}

// A helper function to get raysperpixel array
const VectorXi getRpp(const nlohmann::json &j) {
  VectorXi rpp(j.size());

  for (int i = 0; i < j.size(); ++i) {
    rpp[i] = j[i].get<int>();
  }

  return rpp;
}

Scene *Parser::getScene(const nlohmann::json &j) {
  string name = j["filename"].get<string>();
  int width = j["size"][0].get<int>();
  int height = j["size"][1].get<int>();
  float fov = j["fov"].get<float>();
  Vector3f lookAt = getVector3f(j["lookat"]);
  Vector3f up = getVector3f(j["up"]);
  Vector3f center = getVector3f(j["centre"]);
  Vector3f ai = getVector3f(j["ai"]);
  Vector3f bgc = getVector3f(j["bkc"]);

  Scene *sc = new Scene(name, width, height, fov, center, up, lookAt, ai, bgc);
  sc->setAntialiasing(j.value("antialiasing", false));
  sc->setTwoSideRender(j.value("twosiderender", false));
  sc->setGlobalIllum(j.value("globalillum", false));
  if (j.contains("raysperpixel"))
    sc->setRaysPerPixel(getRpp(j["raysperpixel"]));

  return sc;
}
