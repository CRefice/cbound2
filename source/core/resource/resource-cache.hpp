#pragma once

#include <function>
#include <string>
#include <unordered_map>
#include <utility>

#include "resource.hpp"

template<typename ResType, typename KeyType = std::string>
class ResourceCache
{
public:
	using FuncType = std::function<ResType(const KeyType&)>;

	ResourceCache(FuncType load_func) : load_func(std::move(load_func)) {}

	Resource<ResType> load(KeyType key) {
		if(res_map.find(key) == res_map.end()) {
			res_map.insert({ key, load_func(key) });
		}
		return &res_map[key];
	}

	void reload_all() {
		for (auto& [key, val] : res_map) {
			val = load_func(key);
		}
	}

	void reset() {
		res_map.clear();
	}

private:
	const FuncType load_func;
	std::unordered_map<KeyType, ResType> res_map;
};
