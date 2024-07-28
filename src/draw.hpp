#pragma once

#include <memory>

#include "camera.hpp"
#include "scene.hpp"

namespace sf {
class RenderWindow;
} // namespace sf

namespace subbuteo {

void DrawScene(std::shared_ptr<Scene const> const &scene,
               std::shared_ptr<Camera const> const &camera,
               sf::RenderWindow *window);

} // namespace subbuteo