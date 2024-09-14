// IWYU pragma: no_include <__math/inverse_trigonometric_functions.h>
// IWYU pragma: no_include <__math/roots.h>
// IWYU pragma: no_include <SFML/System/Vector2.inl>

#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <glog/logging.h>
#include <optional>
#include <ostream>
#include <rapidjson/document.h>
#include <vector>

#include "agent_human.hpp"
#include "control.hpp"
#include "coordinate.hpp"

namespace subbuteo {
namespace {

float const kMaxDragDistance = 2.5f;

std::optional<Game::Soccerer>
SelectSoccerer(WorldPosition const &pos,
               std::vector<Game::Soccerer> const &soccerers) {
  for (auto const &soccerer : soccerers) {
    sf::Vector2f r = pos - soccerer.position;
    if (r.x * r.x + r.y * r.y < soccerer.radius * soccerer.radius) {
      return soccerer;
    }
  }
  return std::nullopt;
}

} // namespace

HumanAgent::HumanAgent(rapidjson::Document const & /*config*/,
                       ControlQueue *control_queue)
    : control_queue_(control_queue) {}

Game::Move HumanAgent::ComputeMove(Game const &game) const {
  LOG(INFO) << "Human agent making a move.";

  DragEvent drag;
  std::optional<Game::Soccerer> selected_soccerer;
  while (!selected_soccerer.has_value() || !drag.complete) {
    drag = control_queue_->Pop();
    selected_soccerer = SelectSoccerer(drag.origin, game.CurrentSoccerers());
    LOG(INFO) << drag;
  }

  sf::Vector2f arrow = selected_soccerer->position - drag.current;
  float angle = std::atan2(arrow.y, arrow.x);
  float len = std::sqrt(arrow.x * arrow.x + arrow.y * arrow.y);
  float power = std::min(len, kMaxDragDistance) / kMaxDragDistance;
  return Game::Move(selected_soccerer->id, angle, power);
}

} // namespace subbuteo
