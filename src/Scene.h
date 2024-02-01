#ifndef SCENE_H_
#define SCENE_H_

#include "../external/json.hpp"

#include <string>

using std::string;

class Scene {
public:
  Scene(const nlohmann::json &);
  string getName();
  int getWidth();
  int getHeight();

private:
  string name;
  int width;
  int height;
};

#endif // !SCENE_H_
