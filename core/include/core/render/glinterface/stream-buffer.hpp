#pragma once

#include <utility>

#include <glad/glad.h>

// A std::vector-like interface over a mapped buffer.
namespace gl {
template <typename T>
class BufferStream {
public:
  using iterator = T*;
  using const_iterator = const T*;

  // Initializes the buffer range with a reference to the target
  // and a length. Note that this is not enough to actually use it!
  // You need to map it first.
  BufferStream(GLenum target, GLsizeiptr capacity)
      : target(target), reserved(capacity) {}
  ~BufferStream() { unmap(); }

  auto begin() { return iterator(*this, 0); }
  auto end() { return iterator(*this, size); }
  auto cbegin() const { return const_iterator(*this, 0); }
  auto cend() const { return const_iterator(*this, size); }

  auto size() const { return sz; }
  auto capacity() const { return reserved; }

  // Maps the buffer to memory.
  void map(GLbitfield access_flags) {
    buf = reinterpret_cast<T*>(
        glMapBufferRange(target, 0, reserved * sizeof(T), access_flags));
  }

  // Unmaps the buffer from memory.
  void unmap() {
    glUnmapBuffer(target);
    buf = nullptr;
  }

  void push_back(T val) {
    buf[sz] = std::move(val);
    sz = (sz + 1) % reserved;
  }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    buf[sz] = T{std::forward<Args>(args)...};
    sz = (sz + 1) % reserved;
  }

  void pop_back() {
		sz--;
  }

  void clear() { sz = 0; }

private:
  GLenum target;
  T* buf = nullptr;
  GLsizeiptr sz = 0;
  GLsizeiptr reserved;
};
} // namespace gl
