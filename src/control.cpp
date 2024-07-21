#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include <glog/logging.h>
#include <optional>
#include <ostream>

#include "control.hpp"
#include "coordinate.hpp"

namespace subbuteo {
namespace {

void UpdateMouseDrag(sf::Event::MouseButtonEvent const &mouse_down_event,
                     sf::Vector2u const &window_size, bool complete,
                     std::optional<DragEvent> *drag_state) {
  if (sf::Mouse::Button::Left != mouse_down_event.button) {
    return;
  }

  ScreenPosition mouse_pos =
      CreateScreenPosition(mouse_down_event.x, mouse_down_event.y, window_size);

  if (!drag_state->has_value()) {
    drag_state->emplace(DragEvent(/*origin=*/mouse_pos));
    return;
  }
  drag_state->value().current = mouse_pos;
  drag_state->value().complete = complete;
}

void PollEvents(sf::Window *window, std::optional<DragEvent> *drag_state,
                ControlQueue *control_queue) {
  sf::Event event;
  while (window->pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed: {
      window->close();
      break;
    }
    case sf::Event::MouseButtonPressed: {
      UpdateMouseDrag(event.mouseButton, window->getSize(), /*complete=*/false,
                      drag_state);

      if (drag_state->has_value()) {
        control_queue->Push(drag_state->value());
      }
      break;
    }
    case sf::Event::MouseMoved: {
      if (!drag_state->has_value()) {
        break;
      }
      if (drag_state->value().complete) {
        break;
      }

      UpdateMouseDrag(event.mouseButton, window->getSize(), /*complete=*/false,
                      drag_state);
      break;
    }
    case sf::Event::MouseButtonReleased: {
      if (!drag_state->has_value()) {
        break;
      }
      UpdateMouseDrag(event.mouseButton, window->getSize(), /*complete=*/true,
                      drag_state);

      LOG(INFO) << "PollEvents: pushing a drag event " << drag_state->value()
                << " to control queue...";
      control_queue->Push(drag_state->value());
      drag_state->reset();
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

void ListenControls(sf::RenderWindow *window,
                    std::shared_ptr<ControlQueue> const &control_queue) {
  LOG(INFO) << "ListenControls: started";

  std::optional<DragEvent> drag_state;
  while (window->isOpen()) {
    PollEvents(window, &drag_state, control_queue.get());
  }

  LOG(INFO) << "ListenControls: exiting...";
}

std::ostream &operator<<(std::ostream &stream, DragEvent const &drag_event) {
  stream << "{origin:{x:" << drag_event.origin.x << ",y:" << drag_event.origin.y
         << "},current:{x:" << drag_event.current.x
         << ",y:" << drag_event.current.y
         << "},complete=" << drag_event.complete << "}";
  return stream;
}

} // namespace subbuteo
