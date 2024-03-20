#ifndef RANDOM_H_
#define RANDOM_H_

#include <chrono>
#include <random>
namespace utils {
class Random {
public:
  /**
   * A way to generate higher quality random number
   * since C++ 11
   */
  static float get() {
    std::default_random_engine generator(
        std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    return distribution(generator);
  }
};
} // namespace utils

#endif // !RANDOM_H_
