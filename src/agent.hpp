#pragma once

#include <string>

#include "control.hpp"
#include "game.hpp"

namespace subbuteo {

class AgentInterface {
public:
  using AgentId = std::string;

  AgentId const &Id() const;
  virtual Game::Move ComputeMove(Game const &game,
                                 ControlQueue *control_queue) const = 0;

private:
  AgentId id_;
};

} // namespace subbuteo
