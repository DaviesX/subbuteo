#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <eigen3/Eigen/Core>
#include <optional>

#include "control.hpp"
#include "coordinate.hpp"
#include "event_queue.hpp"

namespace subbuteo {
namespace {

std::optional<DragEvent>
StartMouseDrag(sf::Event::MouseButtonEvent const &mouse_down_event,
               sf::Vector2u const &window_size) {
  if (sf::Mouse::Button::Left != mouse_down_event.button) {
    return std::nullopt;
  }
  return DragEvent{.origin = CreateScreenPosition(
                       mouse_down_event.x, mouse_down_event.y, window_size)};
}

bool EndMouseDrag(sf::Event::MouseButtonEvent const &mounse_up_event,
                  sf::Vector2u const &window_size, DragEvent *drag) {
  if (sf::Mouse::Button::Left != mounse_up_event.button) {
    return false;
  }
  drag->current =
      CreateScreenPosition(mounse_up_event.x, mounse_up_event.y, window_size);
  return true;
}

void PollEvents(sf::Window *window, ControlQueue *control_queue) {
  sf::Event event;
  std::optional<DragEvent> current_drag;

  while (window->pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed: {
      window->close();
      break;
    }
    case sf::Event::MouseButtonPressed: {
      current_drag = StartMouseDrag(event.mouseButton, window->getSize());
      break;
    }
    case sf::Event::MouseButtonReleased: {
      if (!current_drag.has_value()) {
        break;
      }
      if (!EndMouseDrag(event.mouseButton, window->getSize(),
                        &current_drag.value())) {
        break;
      }

      control_queue->Push(std::move(*current_drag));
      current_drag.reset();
      break;
    }
    default: {
      // Do nothing.
      break;
    }
    }
  }
}

} // namespace

void ListenControls(sf::Window *window, ControlQueue *control_queue) {
  bool complete_drag = false;
  DragEvent drag_event;

  while (window->isOpen()) {
    PollEvents(window, control_queue);
  }
}

} // namespace subbuteo
