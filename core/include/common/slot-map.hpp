#pragma once

#include <cassert>
#include <cstdint>
#include <deque>
#include <memory>
#include <vector>

template <typename T>
class SlotMap {
public:
  struct Handle {
    std::uint32_t id, version = 0;
  };

  using iterator = typename std::deque<T>::iterator;
  using const_iterator = typename std::deque<T>::const_iterator;

  Handle add(T item) {
    auto item_idx = size();
    data.push_back(std::move(item));

    if (free_list.empty()) {
      expand(chunk_size);
    }
    auto free = free_list.back();
    free_list.pop_back();
    last_idx = free;

    auto& handle = indirection[free];
    handle.id = item_idx;
    return Handle{free, handle.version};
  }

  iterator find(Handle handle) {
    auto obj_handle = indirection[handle.id];
    if (obj_handle.version == handle.version) {
      return begin() + obj_handle.id;
    }
    return end();
  }

  const_iterator find(Handle handle) const {
    auto obj_handle = indirection[handle.id];
    if (obj_handle.version == handle.version) {
      return begin() + obj_handle.id;
    }
    return end();
  }

  void remove(Handle handle) {
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
    assert(indirection.size() == free_list.size());
    auto new_sz = indirection.size() + count;
    indirection.resize(new_sz);
    free_list.resize(new_sz);
    for (std::uint32_t i = 0; i < count; ++i) {
      free_list.push_back(size() - i);
    }
    assert(indirection.size() == free_list.size());
  }

  std::deque<T> data;
  std::size_t last_idx;
  std::vector<Handle> indirection;
  std::vector<std::uint32_t> free_list;
};
