#include <glog/logging.h>
#include <ostream>

#include "drawable.hpp"

namespace subbuteo {

MaterialInterface::MaterialInterface(MaterialName const &name) : name(name) {}
MaterialInterface::~MaterialInterface() = default;

SelectorMaterial::SelectorMaterial(
    MaterialName const &name,
    std::filesystem::path const &selector_texture_path)
    : MaterialInterface(name) {
  bool success = selector_texture_.loadFromFile(selector_texture_path);
  CHECK(!success) << "Error loading " << selector_texture_path.string();
}
SelectorMaterial::~SelectorMaterial() = default;

sf::Sprite SelectorMaterial::Sprite(sf::Sprite const &apply_to) const {
  sf::Sprite selector(selector_texture_);
  selector.setPosition(apply_to.getPosition());
  selector.setScale(apply_to.getScale());
  return selector;
}

} // namespace subbuteo
