// IWYU pragma: no_include <SFML/System/Vector2.inl>

#include "camera.hpp"

namespace subbuteo {

Camera::Camera(sf::Vector2u const &window_dimension)
    : window_dimension(window_dimension) {}

Camera::Camera(sf::Vector2u const &window_dimension,
               WorldPosition const &center, sf::Vector2f const &view_dimension)
    : window_dimension(window_dimension), center(center),
      view_dimension(view_dimension) {}

sf::Vector2f ComputeWindowPosition(Camera const &camera,
                                   WorldPosition const &position) {
  sf::Vector2f view_pos = position - camera.center;
  view_pos += 0.5f * camera.view_dimension;

  float wx = camera.window_dimension.x * view_pos.x / camera.view_dimension.x;
  float wy =
      camera.window_dimension.y * (1.f - view_pos.y / camera.view_dimension.y);

  return sf::Vector2f(wx, wy);
}

sf::Vector2f ComputeWindowScale(Camera const &camera,
                                sf::Vector2f const &object_dimension,
                                sf::Vector2u const &texture_dimension) {
  float rx = (static_cast<float>(camera.window_dimension.x) *
              object_dimension.x / camera.view_dimension.x) /
             static_cast<float>(texture_dimension.x);
  float ry = (static_cast<float>(camera.window_dimension.y) *
              object_dimension.y / camera.view_dimension.y) /
             static_cast<float>(texture_dimension.y);
  return sf::Vector2f(rx, ry);
}

WorldPosition ComputeWorldPosition(Camera const &camera,
                                   WindowPosition const &position) {
  float view_x =
      position.x * camera.view_dimension.x / camera.window_dimension.x;
  float view_y =
      position.y * camera.view_dimension.y / camera.window_dimension.y;

  sf::Vector2f view_pos(view_x, view_y);
  view_pos -= 0.5f * camera.view_dimension;
  return camera.center + view_pos;
}

} // namespace subbuteo
