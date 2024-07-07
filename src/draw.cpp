#include <SFML/Graphics.hpp>

#include "draw.hpp"

namespace subbuteo {

void DrawScene(const Scene &scene, sf::RenderWindow *window) {
  while (window->isOpen()) {
    window->clear(sf::Color::White);
    window->display();
  }
}

} // namespace subbuteo
