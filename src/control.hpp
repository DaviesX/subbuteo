#pragma once

#include <SFML/Window.hpp>
#include <eigen3/Eigen/Core>

#include "coordinate.hpp"
#include "event_queue.hpp"

namespace subbuteo {

struct DragEvent {
  ScreenPosition origin;
  ScreenPosition current;
};

using ControlQueue = EventQueue<DragEvent>;

void ListenControls(sf::Window *window, ControlQueue *control_queue);

} // namespace subbuteo
