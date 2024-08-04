#pragma once

#include <memory>
#include <string>

#include "control.hpp"
#include "game.hpp"
#include "rapidjson/document.h"

namespace subbuteo {

class AgentInterface {
public:
  using AgentId = std::string;

  AgentInterface();
  virtual ~AgentInterface();

  AgentId const &Id() const;
  virtual Game::Move ComputeMove(Game const &game,
                                 ControlQueue *control_queue) const = 0;

private:
  AgentId id_;
};

std::unique_ptr<AgentInterface> CreateAgent(rapidjson::Document const &config);

} // namespace subbuteo
