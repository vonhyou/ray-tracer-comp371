#include "Output.h"

#include <fstream>

void Output::write() {
  std::ofstream fout(path, std::ios_base::out | std::ios_base::binary);
  fout << "P6\n" << width << ' ' << height << '\n' << "255" << std::endl;

  for (unsigned int y = 0; y < height; ++y)
    for (unsigned int x = 0; x < width; ++x)
      fout << (char)(255.0f * red[y * width + x])
           << (char)(255.0f * green[y * width + x])
           << (char)(255.0f * blue[y * height + x]);
  fout.close();
}

float Output::r(int index) const { return red.at(index); }
float Output::g(int index) const { return green.at(index); }
float Output::b(int index) const { return blue.at(index); }
void Output::r(int index, float value) { red.at(index) = value; }
void Output::g(int index, float value) { green.at(index) = value; }
void Output::b(int index, float value) { blue.at(index) = value; }
