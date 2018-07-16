#include <utility>

#include <glad/glad.h>

#include "common/logging.hpp"

// A non-const range over a mapped buffer.
template <typename T>
class GlStreamBuffer
{
public:
	using iterator = T*;
	using const_iterator = const T*;

	// Initializes the buffer range with a reference to the target
	// and a length. Note that this is not enough to actually use it!
	// You need to map it first.
	GlStreamBuffer(GLenum target, GLsizeiptr capacity) : target(target), sz(0), reserved(capacity) { }
	~GlStreamBuffer() {
		unmap();
	}

	auto begin() { return iterator(*this, 0); }
	auto end() { return iterator(*this, size); }
	auto cbegin() const { return const_iterator(*this, 0); }
	auto cend() const { return const_iterator(*this, size); }

	auto size() const { return sz; }
	auto capacity() const { return reserved; }

	// Maps the buffer to memory.
	void map(GLbitfield access_flags) {
		buf = reinterpret_cast<T*>(glMapBufferRange(target, 0, sz, access_flags));
		if (buf == nullptr) {
			ERROR_LOG("Failed to map buffer {}", target);
		}
	}

	// Unmaps the buffer from memory.
	void unmap() {
		glUnmapBuffer(target);
		buf = nullptr;
	}

	void push_back(const T& val) {
		buf[sz++] = val;
	}
	template <typename... Args>
	void emplace_back(Args&&... args) {
		buf[sz++] = T{std::forward<Args>(args)...};
	}

	void clear() {
		sz = 0;
	}

private:
	GLenum target;
	T* buf = nullptr;
	GLsizeiptr sz;
	GLsizeiptr reserved;
};
