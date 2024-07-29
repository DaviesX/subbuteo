#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include <glog/logging.h>
#include <optional>
#include <ostream>

#include "camera.hpp"
#include "control.hpp"
#include "coordinate.hpp"

namespace subbuteo {
namespace {

void UpdateMouseDrag(sf::Event::MouseButtonEvent const &mouse_down_event,
                     Camera const &camera, bool complete,
                     std::optional<DragEvent> *drag_state) {
  if (sf::Mouse::Button::Left != mouse_down_event.button) {
    return;
  }

  WorldPosition pos = ComputeWorldPosition(
      camera, WindowPosition(mouse_down_event.x, mouse_down_event.y));

  if (!drag_state->has_value()) {
    drag_state->emplace(DragEvent(/*origin=*/pos));
    return;
  }
  drag_state->value().current = pos;
  drag_state->value().complete = complete;
}

bool PollEvents(sf::Window *window, Camera const &camera,
                std::optional<DragEvent> *drag_state,
                ControlQueue *control_queue) {
  sf::Event event;
  while (window->pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed: {
      return false;
    }
    case sf::Event::MouseButtonPressed: {
      UpdateMouseDrag(event.mouseButton, camera, /*complete=*/false,
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

      UpdateMouseDrag(event.mouseButton, camera, /*complete=*/false,
                      drag_state);
      break;
    }
    case sf::Event::MouseButtonReleased: {
      if (!drag_state->has_value()) {
        break;
      }
      UpdateMouseDrag(event.mouseButton, camera, /*complete=*/true, drag_state);

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
  return true;
}

} // namespace

bool ListenControls(sf::RenderWindow *window, Camera const &camera,
                    ControlQueue *control_queue) {
  LOG(INFO) << "ListenControls: started";

  bool listening = true;
  std::optional<DragEvent> drag_state;
  while (listening) {
    listening = PollEvents(window, camera, &drag_state, control_queue);
  }

  LOG(INFO) << "ListenControls: exiting...";
  return true;
}

std::ostream &operator<<(std::ostream &stream, DragEvent const &drag_event) {
  stream << "{origin:{x:" << drag_event.origin.x << ",y:" << drag_event.origin.y
         << "},current:{x:" << drag_event.current.x
         << ",y:" << drag_event.current.y
         << "},complete=" << drag_event.complete << "}";
  return stream;
}

} // namespace subbuteo
