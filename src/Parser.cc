#include "Parser.h"
#include "Geometry.h"

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

// TODO: Get Matrix4f transform for geometries and lights
const Matrix4f getTransform(const nlohmann::json &j) { return Matrix4f(); }

Geometry *Parser::getGeometry(const nlohmann::json &j) {
  float ka = j["ka"].get<float>();
  float kd = j["kd"].get<float>();
  float ks = j["ks"].get<float>();
  Vector3f ca = getVector3f(j["ac"]);
  Vector3f cd = getVector3f(j["dc"]);
  Vector3f cs = getVector3f(j["sc"]);
  float phong = j["pc"].get<float>();

  Geometry *g;

  if (j["type"].get<string>().compare("rectangle"))
    g = getSphere(j, ka, kd, ks, ca, cd, cs, phong);
  else
    g = getRectangle(j, ka, kd, ks, ca, cd, cs, phong);

  if (j.contains("transform")) {
    g->setTransform(getTransform(j["transform"]));
  }

  return g;
}

// TODO: return four corners for rectangles and area lights
const Matrix<float, 3, 4> getCorners(const nlohmann::json &j) {
  return Matrix<float, 3, 4>();
}

Sphere *Parser::getSphere(const nlohmann::json &j, float ka, float kd, float ks,
                          const Vector3f &ca, const Vector3f &cd,
                          const Vector3f &cs, float pc) {
  float radius = j["radius"].get<float>();
  Vector3f center = getVector3f(j["centre"]);

  return new Sphere(ka, kd, ks, ca, cd, cs, pc, radius, center);
}

Rectangle *Parser::getRectangle(const nlohmann::json &j, float ka, float kd,
                                float ks, const Vector3f &ca,
                                const Vector3f &cd, const Vector3f &cs,
                                float pc) {
  Matrix<float, 3, 4> corners = getCorners(j);

  return new Rectangle(ka, kd, ks, ca, cd, cs, pc, corners);
}
