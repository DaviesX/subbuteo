#pragma once

namespace sf {
class RenderWindow;
} // namespace sf

namespace subbuteo {

struct Camera;
class Scene;

void DrawScene(Scene const &scene, Camera const &camera,
               bool const *close_event, sf::RenderWindow *window);

} // namespace subbuteo