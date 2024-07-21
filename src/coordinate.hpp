#pragma once

// IWYU pragma: no_include <SFML/System/Vector2.inl>

#include <SFML/System/Vector2.hpp>

namespace subbuteo {

using WindowPosition = sf::Vector2u;
using ScreenPosition = sf::Vector2f;
using WorldPosition = sf::Vector2f;

inline ScreenPosition CreateScreenPosition(unsigned x, unsigned y,
                                           sf::Vector2u const &win_size) {
  return ScreenPosition(static_cast<float>(x) / win_size.x,
                        static_cast<float>(y) / win_size.y);
}

} // namespace subbuteo
