#include <SFML/Graphics.hpp>
#include <boost/log/trivial.hpp>

#include "draw.hpp"

namespace subbuteo {

void DrawScene(const Scene &scene, sf::RenderWindow *window) {
  BOOST_LOG_TRIVIAL(debug) << "DrawScene: started";

  while (window->isOpen()) {
    window->clear(sf::Color::White);
    window->display();
  }

  BOOST_LOG_TRIVIAL(debug) << "DrawScene: exiting...";
}

} // namespace subbuteo
