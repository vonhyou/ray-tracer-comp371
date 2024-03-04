#ifndef OPTIONAL_H_

namespace utils {

// I write this class because we are using C++14
// so std::optional is not available
template <typename T> class Optional {
public:
  Optional() : hasValue_(false) {}
  Optional(T value) : value_(value), hasValue_(true) {}

  bool hasValue() const { return hasValue_; }
  T value() const { return value_; }

  static const Optional nullopt;

private:
  T value_;
  bool hasValue_;
};

template <typename T> const Optional<T> Optional<T>::nullopt = Optional<T>();
} // namespace utils

#endif // !OPTIONAL_H_
