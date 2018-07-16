#pragma once

template<typename ObjTrait>
class GlObject
{
public:
	GlObject() : handle(ObjTrait::create()) {}
	GlObject(const GlObject<ObjTrait>& obj) = delete;
	GlObject(GlObject<ObjTrait>&& obj) {
		*this = obj;
	}
	~GlObject() {
		ObjTrait::destroy(handle);
	}

	auto& operator=(GlObject<ObjTrait>&& obj) {
		handle = obj.handle;
		obj.handle = 0;
		return *this;
	}
	auto& operator=(const GlObject<ObjTrait>& obj) = delete;

	operator typename ObjTrait::value_type() const { return handle; }

private:
	typename ObjTrait::value_type handle = 0;
};
