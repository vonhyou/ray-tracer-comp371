#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <Eigen/Core>
#include <string>
#include <vector>

using Eigen::Vector3f;
using std::string;
using std::vector;

class Output {
public:
  Output(const Vector3f &bgc, string path, int w, int h)
      : red(vector<float>(w * h + 1, bgc.x())),
        green(vector<float>(w * h + 1, bgc.y())),
        blue(vector<float>(w * h + 1, bgc.z())), path(path), width(w),
        height(h) {}

  void write();

private:
  int width, height;
  string path;
  vector<float> red;
  vector<float> green;
  vector<float> blue;

public:
  void r(int, float);
  float r(int) const;
  void g(int, float);
  float g(int) const;
  void b(int, float);
  float b(int) const;
};

#endif // !OUTPUT_H_
