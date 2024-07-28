#pragma once

// IWYU pragma: no_include <__fwd/ostream.h>
// IWYU pragma: no_include <SFML/System/Vector2.inl>

#include <memory>
#include <ostream> // IWYU pragma: keep

#include "camera.hpp"
#include "coordinate.hpp"
#include "event_queue.hpp"

namespace sf {
class RenderWindow;
} // namespace sf

namespace subbuteo {

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

void ListenControls(sf::RenderWindow *window,
                    std::shared_ptr<Camera const> const &camera,
                    std::shared_ptr<ControlQueue> const &control_queue);
std::ostream &operator<<(std::ostream &stream, DragEvent const &drag_event);

} // namespace subbuteo
