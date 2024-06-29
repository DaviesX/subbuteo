#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace subbuteo {

void Loop() {
    sf::RenderWindow window({505u, 672u}, "Subbuteo");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}

} // namespace subbuteo
