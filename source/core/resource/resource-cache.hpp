#pragma once

#include <function>
#include <map>
#include <utility>

#include "resource.hpp"

// A Cache of Resources of type ResT.
// Holds a function used to load that type of resource,
// and a map of resources keyed by their identifier type.
template<typename ResT>
class ResourceCache
{
public:
	using KeyT = typename Resource::Identifier;
	using FuncT = std::function<ResT(const KeyT&)>;

	ResourceCache(FuncT load_func) : load_func(std::move(load_func)) {}

	// Add the resource to the cache if it doesn't exist,
	// and return it.
	Resource<ResT> load(KeyT key) {
		[iter, success] = map.try_emplace(key, load_func(key));
		return Resource(key, iter);
	}

	// Reload the value if the key doesn't exist in the map,
	// or load it otherwise.
	Resource<ResT> reload(KeyT key) {
		auto [iter, success] = map.insert_or_assign(key, load_func(key));
		return Resource(key, iter);
	}

	// Reload all values in the map.
	void reload_all() {
		for (auto& [key, val] : map) {
			val = load_func(key);
		}
	}

	// Unload all resources.
	void clear() {
		map.clear();
	}

private:
	// Note for future me:
	// map is better in this case, as iterators
	// are kept valid on insertion/deletion.
	FuncT load_func;
	std::map<KeyT, ResT> map;
};
