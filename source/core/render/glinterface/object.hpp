#pragma once

#include <utility>

#include <glad/glad.h>

namespace gl {
// An OpenGL object handle wrapped in a RAII interface.
template<typename ObjTrait>
class Object
{
public:
	Object() {
		ObjTrait::create(&handle);
	}
	Object(const Object<ObjTrait>& obj) = delete;
	Object(Object<ObjTrait>&& obj) {
		*this = std::move(obj);
	}
	~Object() {
		ObjTrait::destroy(&handle);
	}

	auto& operator=(Object<ObjTrait>&& obj) {
		handle = obj.handle;
		obj.handle = 0;
		return *this;
	}
	auto& operator=(const Object<ObjTrait>& obj) = delete;

	operator GLuint() const { return handle; }

private:
	GLuint handle;
};
}
