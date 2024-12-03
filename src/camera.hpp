#pragma once

// IWYU pragma: no_include <SFML/System/Vector2.inl>
#include <SFML/System/Vector2.hpp>

#include "coordinate.hpp"

namespace subbuteo {

struct Camera {
  Camera(sf::Vector2u const &window_dimension);
  Camera(sf::Vector2u const &window_dimension, WorldPosition const &center,
         sf::Vector2f const &view_dimension);

  sf::Vector2u const window_dimension;
  WorldPosition center;
  sf::Vector2f view_dimension;
};

sf::Vector2f ComputeWindowPosition(Camera const &camera,
                                   WorldPosition const &position);
sf::Vector2f ComputeWindowScale(Camera const &camera,
                                sf::Vector2f const &object_dimension,
                                sf::Vector2u const &texture_dimension);
WorldPosition ComputeWorldPosition(Camera const &camera,
                                   WindowPosition const &position);

} // namespace subbuteo
