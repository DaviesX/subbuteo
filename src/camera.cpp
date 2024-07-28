#include "camera.hpp"

namespace subbuteo {

sf::Vector2f ComputeWindowPosition(Camera const &camera,
                                   WorldPosition const &position) {
  sf::Vector2f view_pos = position - camera.center;
  view_pos += 0.5f * camera.view_dimension;

  float wx = view_pos.x * static_cast<float>(camera.window_dimension.x) /
             camera.view_dimension.x;
  float wy = view_pos.y * static_cast<float>(camera.window_dimension.y) /
             camera.view_dimension.y;

  return sf::Vector2f(wx, wy);
}

sf::Vector2f ComputeWindowScale(Camera const &camera,
                                sf::Vector2f const &dimension,
                                sf::Vector2u const &texture_dimension) {
  float rx = (static_cast<float>(camera.window_dimension.x) * dimension.x /
              camera.view_dimension.x) /
             static_cast<float>(texture_dimension.x);
  float ry = (static_cast<float>(camera.window_dimension.y) * dimension.y /
              camera.view_dimension.y) /
             static_cast<float>(texture_dimension.y);
  return sf::Vector2f(rx, ry);
}

} // namespace subbuteo
