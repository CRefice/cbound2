#pragma once

#include <cassert>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <vector>

struct SlotMapHandle {
  std::uint32_t id, version = 0;
};

template <typename T>
class SlotMap {
public:
  using iterator = typename std::deque<T>::iterator;
  using const_iterator = typename std::deque<T>::const_iterator;

  SlotMapHandle add(T item) {
    std::uint32_t item_idx = size();
    data.push_back(std::move(item));

    if (free_list.empty()) {
      free_list.push_back(item_idx);
      indirection.push_back(SlotMapHandle{item_idx});
    }

    auto free = free_list.back();
    free_list.pop_back();
    last_idx = free;

    auto& handle = indirection[free];
    handle.id = item_idx;
    return SlotMapHandle{free, handle.version};
  }

  iterator find(SlotMapHandle handle) {
    auto obj_handle = indirection[handle.id];
    if (obj_handle.version == handle.version) {
      return begin() + obj_handle.id;
    }
    return end();
  }

  const_iterator find(SlotMapHandle handle) const {
    auto obj_handle = indirection[handle.id];
    if (obj_handle.version == handle.version) {
      return begin() + obj_handle.id;
    }
    return end();
  }

  void remove(SlotMapHandle handle) {
    auto& obj_handle = indirection[handle.id];
    if (obj_handle.version == handle.version) {
      free_list.push_back(handle.id);
      obj_handle.version++;
      std::swap(data[obj_handle.id], data.back());
      data.pop_back();
    }
  }

  std::size_t size() const { return data.size(); }

  iterator begin() { return data.begin(); }
  const_iterator begin() const { return data.begin(); }
  iterator end() { return data.end(); }
  const_iterator end() const { return data.end(); }

private:
  static constexpr std::size_t chunk_size = 256;

  void expand(std::size_t count) {
    auto new_sz = indirection.size() + count;
    indirection.resize(new_sz);
    free_list.reserve(new_sz);
    for (std::uint32_t i = 0; i < count; ++i) {
      free_list.push_back(size() - i);
    }
  }

  std::deque<T> data;
  std::size_t last_idx;
  std::vector<SlotMapHandle> indirection;
  std::vector<std::uint32_t> free_list;
};

namespace std {
template <>
struct hash<SlotMapHandle> {
  std::size_t operator()(const SlotMapHandle& handle) const {
    auto a = hash<std::uint32_t>()(handle.id);
    auto b = hash<std::uint32_t>()(handle.version);
    return a >= b ? a * a + a + b : a + b * b;
  }
};
} // namespace std

inline bool operator==(const SlotMapHandle& a, const SlotMapHandle& b) {
  return a.id == b.id && a.version == b.version;
}
