#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

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

class DrawableWorld {
public:
  Drawable *CreateDrawable();

private:
};

} // namespace subbuteo
