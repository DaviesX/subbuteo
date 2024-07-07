#pragma once

#include <SFML/Graphics.hpp>
#include <eigen3/Eigen/Core>
#include <ostream>

#include "coordinate.hpp"
#include "event_queue.hpp"

namespace subbuteo {

ScreenPosition const NullDragPosition = ScreenPosition(-1, -1);

struct DragEvent {
  DragEvent()
      : origin(NullDragPosition), current(NullDragPosition), complete(false) {}
  DragEvent(ScreenPosition const &origin)
      : origin(origin), current(NullDragPosition), complete(false) {}
  DragEvent(ScreenPosition const &origin, ScreenPosition const &current)
      : origin(origin), current(current), complete(false) {}
  DragEvent(ScreenPosition const &origin, ScreenPosition const &current,
            bool complete)
      : origin(origin), current(current), complete(complete) {}

  ScreenPosition origin;
  ScreenPosition current;
  bool complete;
};

using ControlQueue = EventQueue<DragEvent>;

void ListenControls(sf::RenderWindow *window, ControlQueue *control_queue);
std::ostream &operator<<(std::ostream &stream, DragEvent const &drag_event);

} // namespace subbuteo
