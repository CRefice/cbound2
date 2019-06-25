#include <filesystem>
#include <pugixml.hpp>

#include "common/logging.hpp"

#include "editor/io.hpp"

namespace fs = std::filesystem;
using namespace render;
using namespace std::literals;

namespace editor::io {
void save_collision(const fs::path& path, const TileSet& set) {
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(path.c_str());
  if (!result) {
    ERROR_LOG("Error parsing file \"{}\": {}", path.string(),
              result.description());
    return;
  }

  auto tileset = doc.child("tileset");
  auto properties = tileset.child("properties");
  if (!properties) {
    properties = tileset.prepend_child("properties");
  }
  auto collision = properties.find_child([](const auto& n) {
    return n.attribute("name").value() == "collision_data"s;
  });

  if (!collision) {
    collision = properties.append_child("property");
    collision.append_attribute("name");
    collision.append_attribute("type");
    collision.append_attribute("value");
    collision.attribute("name") = "collision_data";
    collision.attribute("type") = "file";
  }

  auto coll_name = path.stem().string() + "-coll.xml";
  collision.attribute("value") = coll_name.c_str();
  doc.save_file(path.c_str());

  pugi::xml_document coll_doc;
  collision = coll_doc.append_child("collision");
  auto subcols = collision.append_attribute("subcolumns");
  auto subrows = collision.append_attribute("subrows");
  subcols = set.collision.subtile_res.x;
  subrows = set.collision.subtile_res.y;
  auto data = collision.append_child("data");
  std::stringstream ss;
  auto it = set.collision.data.begin();
  ss << *(it++);
  for (; it != set.collision.data.end(); ++it) {
    ss << ',' << *it;
  }
  auto text = data.text();
  text.set(ss.str().c_str());

  auto coll_path = path.parent_path() / coll_name;
  coll_doc.save_file(coll_path.c_str());
}
} // namespace editor::io
