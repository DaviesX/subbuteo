#include <glog/logging.h>
#include <ostream>
#include <utility>

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

Drawable::Drawable(WorldPosition const &position, sf::Vector2f const &dimension,
                   unsigned layer, float angle, sf::Texture const &texture)
    : position(position), dimension(dimension), layer(layer), angle(angle),
      texture(texture), material(nullptr) {}

Drawable *DrawableWorld::CreateDrawable(WorldPosition const &position,
                                        sf::Vector2f const &dimension,
                                        unsigned layer, float angle,
                                        sf::Texture const &texture) {
  auto drawable =
      std::make_unique<Drawable>(position, dimension, layer, angle, texture);
  drawables_.push_back(std::move(drawable));
  return drawables_.back().get();
}

std::vector<Drawable *> DrawableWorld::Drawables() const {
  std::vector<Drawable *> result;
  result.reserve(drawables_.size());
  for (auto &drawable : drawables_) {
    result.push_back(drawable.get());
  }
  return result;
}

void DrawableWorld::ClearDrawables() { drawables_.clear(); }

void DrawableWorld::AddMaterial(std::unique_ptr<MaterialInterface> &&material) {
  MaterialName name = material->name;
  bool inserted =
      materials_.insert(std::make_pair(name, std::move(material))).second;
  CHECK(inserted) << "Material with name " << name
                  << " has already been added.";
}

MaterialInterface *DrawableWorld::Material(MaterialName const &name) const {
  auto it = materials_.find(name);
  CHECK(it != materials_.end())
      << "Material with name " << name << " doesn't exist.";
  return it->second.get();
}

} // namespace subbuteo
