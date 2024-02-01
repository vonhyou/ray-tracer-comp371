#include "RayTracer.h"
#include "../external/simpleppm.h"

#include <iostream>
#include <string>
#include <vector>

RayTracer::RayTracer(nlohmann::json scene) : scene(scene) {}

void RayTracer::run() {
  std::string filename;
  int sizeX = -1, sizeY = -1;
  for (auto itr = scene["output"].begin(); itr != scene["output"].end();
       ++itr) {

    if (itr->contains("filename")) {
      filename = (*itr)["filename"].get<std::string>();

      std::cout << filename << std::endl;
    }

    if (itr->contains("size")) {
      sizeX = (*itr)["size"].begin()->get<int>();
      sizeY = ((*itr)["size"].begin() + 1)->get<int>();

      std::cout << sizeX << " " << sizeY << std::endl;
    }
  }

  std::vector<double> buffer(3 * sizeX * sizeY);

  save_ppm(filename, buffer, sizeX, sizeY);
}
