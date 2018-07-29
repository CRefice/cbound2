#pragma once

#include "common/paths.hpp"

#include <string>
#include <utility>

template <typename ResT, typename IdT = std::string>
class Resource
{
public:
	Resource(ResT* resource, IdT id) : id(std::move(id)), resource(resource) {}

	ResT& operator*() {
		return *resource;
	}
	ResT* operator->() {
		return resource;
	}

	const ResT& operator*() const {
		return *resource;
	}
	const ResT* operator->() const {
		return resource;
	}

	auto identifier() const { return id; }

private:
	IdT id;
	ResT* resource;
};

template <typename KeyT>
std::filesystem::path to_path(const KeyT& id) {
	return path::install_dir() / "resources" / id;
}
