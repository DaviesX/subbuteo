#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <unordered_map>

#include "coordinate.hpp"

namespace subbuteo {

struct Drawable {
  ScreenPosition position;
  float rotation;
  float scale;
  unsigned depth;
  sf::Texture texture;
  std::optional<sf::Texture> selector;
};

using DrawableId = std::string;
using Scene = std::unordered_map<DrawableId, Drawable>;

void DrawScene(const Scene &scene, sf::RenderWindow *window);

} // namespace subbuteo