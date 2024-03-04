#ifndef SCENE_H_
#define SCENE_H_

#include <Eigen/Core>
#include <string>

using Eigen::Vector3f;
using std::string;

class Scene {
public:
  Scene(string name, int width, int height, float fov, const Vector3f &center,
        const Vector3f &up, const Vector3f &lookAt, const Vector3f &ai,
        const Vector3f &bgc)
      : name(name), width(width), height(height), fov(fov), center(center),
        up(up), lookAt(lookAt), ai(ai), backgroundColor(bgc) {}

private:
  string name;
  int width;
  int height;
  float fov;
  Vector3f center;
  Vector3f up;
  Vector3f lookAt;
  Vector3f ai; // ambient intensity
  Vector3f backgroundColor;

  Eigen::VectorXi raysPerPixel;
  bool antialiasing = false;
  bool twoSideRender = false;
  bool globalIllum = false;

public:
  string getName() const;
  int getWidth();
  int getHeight();
  float getFov();
  Vector3f getCenter() const;
  Vector3f getUpVector() const;
  Vector3f getLookAt() const;
  Vector3f getBackgroundColor() const;
  void setRaysPerPixel(const Eigen::VectorXi &);
  void setAntialiasing(bool);
  void setTwoSideRender(bool);
  void setGlobalIllum(bool);
};

#endif // !SCENE_H_
