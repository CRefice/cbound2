#pragma once

#include <cstdint>
#include <iterator>
#include <vector>

#include "rectangle.hpp"

namespace detail {
template <typename T, bool enable = false>
struct conditional_const {
  using type = T;
};

template <typename T>
struct conditional_const<T, true> {
  using type = typename std::add_const<T>::type;
};
} // namespace detail

template <typename T>
class Matrix {
public:
  using iterator = typename std::vector<T>::iterator;
  using reference = typename std::vector<T>::reference;
  using const_iterator = typename std::vector<T>::const_iterator;
  using const_reference = typename std::vector<T>::const_reference;

  Matrix() = default;
  Matrix(ssm::ivec2 size, const T& val = T{})
      : sz(size), vec(sz.x * sz.y, val) {}
  Matrix(ssm::ivec2 size, std::vector<T> vec) : sz(size), vec(std::move(vec)) {}

  reference operator[](const ssm::ivec2& pos) {
    return vec[pos.x + sz.x * pos.y];
  }
  const_reference operator[](const ssm::ivec2& pos) const {
    return vec[pos.x + sz.x * pos.y];
  }

  reference operator[](std::size_t idx) { return vec[idx]; }
  const_reference operator[](std::size_t idx) const { return vec[idx]; }

  ssm::ivec2 size() const { return sz; };
  std::size_t width() const { return sz.x; };
  std::size_t height() const { return sz.y; };
  std::size_t num_elements() const { return vec.size(); };

  iterator begin() { return vec.begin(); }
  iterator end() { return vec.end(); }
  const_iterator begin() const { return vec.begin(); }
  const_iterator end() const { return vec.end(); }

private:
  ssm::ivec2 sz;
  std::vector<T> vec;
};

template <typename T, bool is_const = false>
class MatrixViewIterator;

template <typename T, bool is_const = false>
class MatrixView {
  using MatrixRef =
      typename detail::conditional_const<Matrix<T>, is_const>::type&;

public:
  MatrixView(MatrixRef matrix, Rectangle<int> area)
      : mat(matrix), area(std::move(area)) {}

  decltype(auto) operator[](const ssm::ivec2& pos) {
    return mat[pos + area.pos()];
  }

  ssm::ivec2 size() const { return area.size(); };
  std::size_t width() const { return area.width(); };
  std::size_t height() const { return area.height(); };
  std::size_t num_elements() const { return area.area(); };

  MatrixViewIterator<T, is_const> begin();
  MatrixViewIterator<T, is_const> end();

private:
  MatrixRef mat;
  Rectangle<int> area;
};

// Useful deduction guides
template <typename T>
MatrixView(Matrix<T>&, Rectangle<int>)->MatrixView<T, false>;
template <typename T>
MatrixView(const Matrix<T>&, Rectangle<int>)->MatrixView<T, true>;

template <typename T, bool is_const>
class MatrixViewIterator {
  using ViewRef = MatrixView<T, is_const>&;

public:
  using iterator_category = std::input_iterator_tag;
  using value_type = T;
  using difference_type = void;
  using pointer = void;
  using reference = ViewRef;

  MatrixViewIterator(ViewRef ref, ssm::ivec2 pos = ssm::ivec2(0))
      : ref(ref), pos(pos) {}

  decltype(auto) operator*() { return ref[pos]; }
  auto* operator-> () { return &ref[pos]; }

  auto& operator++() {
    pos.x = pos.x + 1;
    if ((std::size_t)pos.x >= ref.width()) {
      pos.x = 0;
      pos.y = pos.y + 1;
    }
    return *this;
  }

  inline friend bool operator==(const MatrixViewIterator<T, is_const>& a,
                                const MatrixViewIterator<T, is_const>& b) {
    return a.pos == b.pos;
  }

  inline friend bool operator!=(const MatrixViewIterator<T, is_const>& a,
                                const MatrixViewIterator<T, is_const>& b) {
    return !(a == b);
  }

private:
  ViewRef ref;
  ssm::ivec2 pos;
};

template <typename T, bool is_const>
MatrixViewIterator<T, is_const> MatrixView<T, is_const>::begin() {
  return MatrixViewIterator(*this);
}

template <typename T, bool is_const>
MatrixViewIterator<T, is_const> MatrixView<T, is_const>::end() {
  return MatrixViewIterator(*this, ssm::ivec2(0, height()));
}
