#pragma once

#include <functional>
#include <map>
#include <string>
#include <utility>

#include "resource.hpp"

// A Cache of Resources of type ResT.
// Holds a function used to load that type of resource,
// and a map of resources keyed by their identifier type.
template <typename ResT, typename KeyT = std::string> class ResourceCache {
public:
  // Add the resource to the cache if it doesn't exist,
  // and return it.
  Resource<ResT, KeyT> load(const KeyT &key) {
    auto iter = map.find(key);
    if (iter == map.end()) {
      auto [newiter, success] =
          map.emplace(key, load_resource<ResT>(to_path(key)));
      iter = newiter;
    }
    return Resource(&(iter->second), key);
  }

  // Reload the value if the key doesn't exist in the map,
  // or load it otherwise.
  Resource<ResT, KeyT> reload(KeyT key) {
    auto [iter, success] =
        map.insert_or_assign(key, load_resource<ResT>(to_path(key)));
    return Resource(&(iter->second), key);
  }

  // Reload all values in the map.
  void reload_all() {
    for (auto &[key, val] : map) {
      val = load_resource<ResT>(to_path(key));
    }
  }

  // Unload all resources.
  void clear() { map.clear(); }

private:
  // Standard map rather than hashmap, since iterators
  // are kept valid on insertion/deletion.
  std::map<KeyT, ResT> map;
};
