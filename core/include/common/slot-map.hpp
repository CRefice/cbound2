#pragma once

#include <cstdint>
#include <deque>
#include <memory>
#include <vector>

template <typename T>
class SlotMap {
public:
  struct Handle {
    std::uint32_t id, version;
  };

  template <typename U>
  Handle add(U&& item) {
    if (free_list.empty()) {
      chunks.push_back(create_chunk());
    }
    auto free = free_list.back();
    free_list.pop_back();
    auto& obj = chunks[free / chunk_size][free % chunk_size];
    obj.item = std::forward<T>(item);
    return obj.handle;
  }

  T* find(Handle handle) {
    auto& obj = chunks[handle.id / chunk_size][handle.id % chunk_size];
    if (obj.handle == handle) {
      return &obj.item;
    }
    return nullptr;
  }

  std::size_t size() const { return chunk_size * chunks.size(); }

private:
  static constexpr std::size_t chunk_size = 256;

  struct Object {
    T item;
    Handle handle;
  };
  using Chunk = std::unique_ptr<Object[]>;

  Chunk create_chunk() {
    auto chunk = std::make_unique<Object>(chunk_size);
    for (std::uint32_t i = 0; i < chunk_size; ++i) {
      chunk[i].handle = Handle{size() + i, 0};
      // Update free list in reverse, so 0th index is used first
      free_list.push_back(size() - i);
    }
    return chunk;
  }

  std::vector<Chunk> chunks;
  std::vector<std::uint32_t> indirection;
  std::vector<std::uint32_t> free_list;
};
