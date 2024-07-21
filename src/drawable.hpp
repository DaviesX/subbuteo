#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

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
  sf::Texture texture;
  sf::Sprite sprite;
  MaterialInterface *material;
  unsigned layer;
};

class DrawableWorld {
public:
  DrawableWorld(std::vector<std::unique_ptr<MaterialInterface>> &&materials);

  Drawable *CreateDrawable(std::filesystem::path const &texture,
                           unsigned layer);
  std::vector<Drawable *> Drawables() const;
  MaterialInterface *Material(MaterialName const &name) const;

private:
  std::unordered_map<MaterialName, std::unique_ptr<MaterialInterface>>
      materials_;
  std::vector<std::unique_ptr<Drawable>> drawables_;
};

} // namespace subbuteo
