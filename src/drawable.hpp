#pragma once

// IWYU pragma: no_include <SFML/System/Vector2.inl>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "coordinate.hpp"

namespace subbuteo {

using MaterialName = std::string;

class MaterialInterface {
public:
  MaterialInterface(MaterialName const &name);
  virtual ~MaterialInterface();
  virtual sf::Sprite Sprite(sf::Sprite const &apply_to) const = 0;

public:
  MaterialName const name;
};

class SelectorMaterial final : public MaterialInterface {
public:
  SelectorMaterial(MaterialName const &name,
                   std::filesystem::path const &selector_texture_path);
  ~SelectorMaterial() override;

  sf::Sprite Sprite(sf::Sprite const &apply_to) const override;

private:
  sf::Texture selector_texture_;
};

class AimMaterial final : public MaterialInterface {
public:
  AimMaterial(MaterialName const &name);
  ~AimMaterial() override;

  sf::Sprite Sprite(sf::Sprite const &apply_to) const override;
  void SetAngle(float angle);
  void SetDragDistance(float distance);

private:
  float angle_;
  float distance_;
};

struct Drawable {
  Drawable(WorldPosition const &position, sf::Vector2f const &dimension,
           unsigned layer, float angle, sf::Texture const &texture);

  WorldPosition position;
  sf::Vector2f dimension;
  unsigned layer;
  float angle;
  sf::Texture texture;
  MaterialInterface *material;
};

class DrawableWorld {
public:
  Drawable *CreateDrawable(WorldPosition const &position,
                           sf::Vector2f const &dimension, unsigned layer,
                           float angle, sf::Texture const &texture);
  std::vector<Drawable *> Drawables() const;
  void ClearDrawables();
  void AddMaterial(std::unique_ptr<MaterialInterface> &&material);
  MaterialInterface *Material(MaterialName const &name) const;

private:
  std::unordered_map<MaterialName, std::unique_ptr<MaterialInterface>>
      materials_;
  std::vector<std::unique_ptr<Drawable>> drawables_;
};

} // namespace subbuteo
