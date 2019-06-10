#pragma once

#include <cstdint>
#include <vector>

template <typename T>
class Matrix {
public:
  using iterator = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;

  Matrix() = default;
  Matrix(std::size_t w, std::size_t h, const T& val = T{}) : w(w), h(h) {
    vec.resize(w * h, val);
  }

  T& operator()(std::size_t x, std::size_t y) { return vec[y * w + x]; }
  const T& operator()(std::size_t x, std::size_t y) const {
    return vec[y * w + x];
  }

  T& nth(std::size_t idx) { return vec[idx]; }
  const T& nth(std::size_t idx) const { return vec[idx]; }

  std::size_t width() const { return w; };
  std::size_t height() const { return h; };
  std::size_t num_elements() const { return w * h; };

  iterator begin() { return vec.begin(); }
  iterator end() { return vec.end(); }
  const_iterator begin() const { return vec.begin(); }
  const_iterator end() const { return vec.end(); }

private:
  std::size_t w = 0, h = 0;
  std::vector<T> vec;
};
