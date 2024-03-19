#ifndef PROGRESS_H_
#define PROGRESS_H_

#include <iostream>

namespace utils {

#define BAR_WIDTH 70

class Progress {
public:
  static void of(float p) {
    std::cout << "[";
    int pos = BAR_WIDTH * p;
    for (int i = 0; i < BAR_WIDTH; ++i) {
      if (i < pos)
        std::cout << "=";
      else if (i == pos)
        std::cout << ">";
      else
        std::cout << " ";
    }
    std::cout << "] " << int(p * 100.0) << " %\r";
    std::cout.flush();
  }
};
} // namespace utils

#endif // !PROGRESS_H_
