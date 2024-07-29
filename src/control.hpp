#pragma once

// IWYU pragma: no_include <__fwd/ostream.h>
// IWYU pragma: no_include <SFML/System/Vector2.inl>

#include <ostream> // IWYU pragma: keep

#include "coordinate.hpp"
#include "event_queue.hpp"

namespace sf {
class RenderWindow;
} // namespace sf

namespace subbuteo {

struct Camera;

WorldPosition const NullDragPosition = WorldPosition(-1, -1);

struct DragEvent {
  DragEvent()
      : origin(NullDragPosition), current(NullDragPosition), complete(false) {}
  DragEvent(WorldPosition const &origin)
      : origin(origin), current(NullDragPosition), complete(false) {}
  DragEvent(WorldPosition const &origin, WorldPosition const &current)
      : origin(origin), current(current), complete(false) {}
  DragEvent(WorldPosition const &origin, WorldPosition const &current,
            bool complete)
      : origin(origin), current(current), complete(complete) {}

  WorldPosition origin;
  WorldPosition current;
  bool complete;
};

using ControlQueue = EventQueue<DragEvent>;

bool ListenControls(sf::RenderWindow *window, Camera const &camera,
                    ControlQueue *control_queue);
std::ostream &operator<<(std::ostream &stream, DragEvent const &drag_event);

} // namespace subbuteo
