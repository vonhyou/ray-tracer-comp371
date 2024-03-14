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
      : name_(name), width_(width), height_(height), fov_(fov), center_(center),
        up_(up), lookAt_(lookAt), ai_(ai), bgc_(bgc) {}

private:
  string name_;
  int width_;
  int height_;
  float fov_;
  Vector3f center_;
  Vector3f up_;
  Vector3f lookAt_;
  Vector3f ai_; // ambient intensity
  Vector3f bgc_;

  Eigen::VectorXi raysPerPixel_;
  bool antialiasing_ = false;
  bool twoSideRender_ = false;
  bool globalIllum_ = false;

public:
  string name() const;
  int width();
  int height();
  float fov();
  Vector3f center() const;
  Vector3f up() const;
  Vector3f lookAt() const;
  Vector3f backgroundColor() const;
  void setRaysPerPixel(const Eigen::VectorXi &);
  void setAntialiasing(bool);
  void setTwoSideRender(bool);
  void setGlobalIllum(bool);
};

#endif // !SCENE_H_
