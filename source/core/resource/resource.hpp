#pragma once

#include <utility>
#include <string>

template <typename ResT>
class Resource
{
public:
	using Identifier = std::string;

	Resource(Identifier id, ResT* resource) : id(std::move(id)), resource(resource) {}

	friend bool operator==(const Resource& lhs, const Resource& rhs) {
		return lhs.id == rhs.id;
	}
	friend bool operator!=(const Resource& lhs, const Resource& rhs) {
		return !(lhs == rhs);
	}

	ResT& operator*() {
		return *resource;
	}
	ResT* operator->() {
		return *resource;
	}

private:
	Identifier id;
	ResT* resource;
};
