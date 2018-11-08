#include <utility>

#include <glad/glad.h>

// A std::vector-like interface over a mapped buffer.
namespace gl {
template <typename T>
class MappedBuffer
{
public:
	using iterator = T*;
	using const_iterator = const T*;

	auto begin() { return iterator(*this, 0); }
	auto end() { return iterator(*this, size); }
	auto cbegin() const { return const_iterator(*this, 0); }
	auto cend() const { return const_iterator(*this, size); }

	auto size() const { return sz; }
	auto capacity() const { return reserved; }

	// Maps the buffer to memory.
	void map(GLbitfield access_flags) {
		buf = 
	}

	// Unmaps the buffer from memory.
	void unmap() {
		buf = nullptr;
	}

	void push_back(T val) {
		buf[sz++] = std::move(val);
	}
	template <typename... Args>
	void emplace_back(Args&&... args) {
		buf[sz++] = T{std::forward<Args>(args)...};
	}

	void clear() {
		sz = 0;
	}

private:
	MappedBuffer(GLenum target) : target(target) {}
	~BufferStream() {
		unmap();
	}

	GLenum target;
	T* buf = nullptr;
	GLsizeiptr sz = 0;
	GLsizeiptr reserved;

	friend MappedBuffer<T> map(GLenum target, GLsizeiptr size, GLsizeiptr begin = 0);
};

template <typename T>
MappedBuffer<T> map(GLenum target, GLenum flags, GLsizeiptr size, GLsizeiptr begin = 0) {
	T* ptr = reinterpret_cast<T*>(glMapBufferRange(target, 0, size * sizeof(T), access_flags));
	MappedBuffer<T> buf(target
}
}
