#pragma once

// IWYU pragma: no_include <SFML/System/Vector2.inl>
#include <SFML/System/Vector2.hpp>

#include "coordinate.hpp"

namespace subbuteo {

struct Camera {
  WorldPosition center;
  sf::Vector2f view_dimension;
  sf::Vector2u window_dimension;
};

sf::Vector2f ComputeWindowPosition(Camera const &camera,
                                   WorldPosition const &position);
sf::Vector2f ComputeWindowScale(Camera const &camera,
                                sf::Vector2f const &dimension,
                                sf::Vector2u const &texture_dimension);

} // namespace subbuteo
