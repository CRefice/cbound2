#pragma once

#include <functional>
#include <map>
#include <string>
#include <utility>

#include "resource.hpp"

// A Cache of Resources of type ResT.
// Holds a function used to load that type of resource,
// and a map of resources keyed by their identifier type.
template<typename ResT, typename KeyT = std::string>
class ResourceCache
{
public:
	using FuncT = std::function<ResT(const KeyT&)>;

	ResourceCache(FuncT fn) : load_func(std::move(fn)) {}

	// Add the resource to the cache if it doesn't exist,
	// and return it.
	Resource<ResT, KeyT> load(const KeyT& key) {
		auto [iter, success] = map.try_emplace(key, load_func(key));
		return Resource(&(iter->second), key);
	}

	// Reload the value if the key doesn't exist in the map,
	// or load it otherwise.
	Resource<ResT, KeyT> reload(KeyT key) {
		auto [iter, success] = map.insert_or_assign(key, load_func(key));
		return Resource(&(iter->second), key);
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
