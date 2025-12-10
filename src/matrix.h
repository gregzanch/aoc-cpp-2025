#pragma once
#include <functional>
#include <iostream>
#include <optional>
#include <print>
#include <vector>
#include "util.h"

template <typename T>
class Matrix {
 public:
  struct Position {
    size_t x;
    size_t y;
    const T& value;
  };

  class Iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Position;
    using difference_type = std::ptrdiff_t;
    using pointer = const Position*;
    using reference = Position;

    Iterator(const Matrix<T>* matrix, size_t x, size_t y) : m_matrix(matrix), m_x(x), m_y(y) {}

    Position operator*() const { return {m_x, m_y, m_matrix->m_data[m_y][m_x]}; }

    Iterator& operator++() {
      ++m_x;
      if (m_x >= m_matrix->width()) {
        m_x = 0;
        ++m_y;
      }
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const Iterator& other) const { return m_x == other.m_x && m_y == other.m_y; }

    bool operator!=(const Iterator& other) const { return !(*this == other); }

   private:
    const Matrix<T>* m_matrix;
    size_t m_x;
    size_t m_y;
  };

  class IteratorView {
   public:
    IteratorView(const Matrix<T>* matrix) : m_matrix(matrix) {}
    Iterator begin() const { return Iterator(m_matrix, 0, 0); }
    Iterator end() const { return Iterator(m_matrix, 0, m_matrix->height()); }

   private:
    const Matrix<T>* m_matrix;
  };

  Matrix() = default;
  Matrix(std::vector<std::vector<T>>&& data) { set_data(data); }
  ~Matrix() = default;
  Matrix(const Matrix& other) = default;
  Matrix(Matrix&&) = default;

  inline void set_data(const std::vector<std::vector<T>>& data) noexcept {
    m_data = data;
    update_dims();
  }
  inline void set_data(std::vector<std::vector<T>>&& data) noexcept {
    m_data = std::move(data);
    update_dims();
  }
  inline size_t width() const noexcept { return m_width; }
  inline size_t height() const noexcept { return m_height; }

  const T* get_ptr(size_t x, size_t y) const noexcept {
    if (!within(x, 0, m_width, LEFT_INCLUSIVE) || !within(y, 0, m_height, LEFT_INCLUSIVE)) {
      return nullptr;
    }
    return m_data.at(y).at(x);
  }

  std::optional<std::reference_wrapper<const T>> get_ref(size_t x, size_t y) const noexcept {
    if (!within(x, 0, m_width, LEFT_INCLUSIVE) || !within(y, 0, m_height, LEFT_INCLUSIVE)) {
      return std::nullopt;
    }
    return std::cref(m_data.at(y).at(x));
  }

  std::optional<std::reference_wrapper<T>> get_ref_mut(size_t x, size_t y) noexcept {
    if (!within(x, 0, m_width, LEFT_INCLUSIVE) || !within(y, 0, m_height, LEFT_INCLUSIVE)) {
      return std::nullopt;
    }
    return std::ref(m_data.at(y).at(x));
  }

  void set(size_t x, size_t y, const T& value) noexcept {
    if (within(x, 0, m_width, LEFT_INCLUSIVE) && within(y, 0, m_height, LEFT_INCLUSIVE)) {
      m_data.at(y).at(x) = value;
    }
  }

  std::optional<T> get(size_t x, size_t y) const noexcept {
    if (!within(x, 0, m_width, LEFT_INCLUSIVE) || !within(y, 0, m_height, LEFT_INCLUSIVE)) {
      return {};
    }
    return m_data.at(y).at(x);
  }

  inline std::vector<std::vector<T>> get_data() const noexcept { return m_data; }

  IteratorView iter() const { return IteratorView(this); }

  void print() const noexcept {
    for (const auto& line : m_data) {
      for (const auto& item : line) {
        std::cout << item;
      }
      std::cout << std::endl;
    }
  }

 private:
  inline void update_dims() noexcept {
    m_width = m_data.size() > 0 ? m_data.at(0).size() : 0;
    m_height = m_data.size();
  }

  std::vector<std::vector<T>> m_data;
  size_t m_width = 0;
  size_t m_height = 0;

  friend class Iterator;
};

template <typename T>
struct std::formatter<Matrix<T>> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
  auto format(const Matrix<T>& obj, std::format_context& ctx) const {
    auto out = ctx.out();
    for (size_t i = 0; i < obj.height(); ++i) {
      for (size_t j = 0; j < obj.width(); ++j) {
        out = std::format_to(out, "{}", obj.get(j, i).value());
      }
      *out++ = '\n';
    }
    return out;
  }
};