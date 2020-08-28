#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

// Serves a double purpose: holds an index into the indirection list,
// which itself holds indices to the data array. Thus, every array access
// goes through two indirections.
template <typename T>
struct SlotMapHandle {
  std::uint32_t id, version = 0;
};

template <typename T>
class SlotMap {
public:
  // Internal storage is a packed std::vector<T>, so iterator
  // types are just aliases to the vector's.
  using iterator = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;

  SlotMap() = default;

  // Inserts item into the map, returning its identifier handle.
  SlotMapHandle<T> add(T item) {
    // Internal data is always packed and contiguous,
    // so we do a regular push_back into it.
    std::uint32_t item_idx = size();
    data.push_back(std::move(item));

    // Invariant: free list is non-empty on insertion.
    if (free_list.empty()) {
      free_list.push_back(item_idx);
      indirection.push_back(SlotMapHandle<T>{item_idx});
    }

    // Consume most recent spot in free list.
    auto free = free_list.back();
    free_list.pop_back();

    assert(free < indirection.size());
    // Update selected element in indirection list
    // to point to the newly added item in the data array.
    auto& handle = indirection[free];
    handle.id = item_idx;

    return SlotMapHandle<T>{free, handle.version};
  }

  const_iterator find(SlotMapHandle<T> handle) const {
    if (handle.id < indirection.size()) {
      auto obj_handle = indirection[handle.id];
      if (obj_handle.version == handle.version) {
        return begin() + obj_handle.id;
      }
    }
    return end();
  }

  iterator find(SlotMapHandle<T> handle) {
    if (handle.id < indirection.size()) {
      auto obj_handle = indirection[handle.id];
      if (obj_handle.version == handle.version) {
        return begin() + obj_handle.id;
      }
    }
    return end();
  }

  T& operator[](SlotMapHandle<T> handle) {
    auto iter = find(handle);
    assert(iter != end());
    return *iter;
  }

  const T& operator[](SlotMapHandle<T> handle) const {
    auto iter = find(handle);
    assert(iter != end());
    return *iter;
  }

  void remove(SlotMapHandle<T> handle) {
    auto& obj_handle = indirection.at(handle.id);
    if (obj_handle.version == handle.version) {
      // Increment the version number on deletion, so access
      // from copied handles will fail.
      obj_handle.version++;
      // Mark spot taken up by removed item as free to use.
      free_list.push_back(handle.id);

      // Use the swap-and-pop trick to remove the item
      // at the given index.
      std::swap(data.at(obj_handle.id), data.back());
      data.pop_back();

      auto previous_idx = data.size();
      // Iterate backwards through indirection list to find
      // which slot pointed to the moved item, and update it.
      // In practice, this will usually return quickly.
      auto prev_slot = std::find_if(
          indirection.rbegin(), indirection.rend(),
          [=](const auto& slot) { return slot.id == previous_idx; });
      prev_slot->id = obj_handle.id;
    }
  }

  std::size_t size() const noexcept { return data.size(); }

  iterator begin() noexcept { return data.begin(); }
  const_iterator begin() const noexcept { return data.begin(); }
  iterator end() noexcept { return data.end(); }
  const_iterator end() const noexcept { return data.end(); }

private:
  std::vector<T> data;
  std::vector<SlotMapHandle<T>> indirection;
  std::vector<std::uint32_t> free_list;
};

template <typename T>
inline bool operator==(const SlotMapHandle<T>& a, const SlotMapHandle<T>& b) {
  return a.id == b.id && a.version == b.version;
}
