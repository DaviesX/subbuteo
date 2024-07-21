#pragma once

#include <memory>

#include "scene.hpp"

namespace sf {
class RenderWindow;
} // namespace sf

namespace subbuteo {

void DrawScene(std::shared_ptr<Scene const> const &scene, sf::RenderWindow *window);

} // namespace subbuteo