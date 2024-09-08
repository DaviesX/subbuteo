#pragma once

#include <rapidjson/document.h>

#include "agent.hpp"
#include "control.hpp"
#include "game.hpp"

namespace subbuteo {

class HumanAgent final : public AgentInterface {
public:
  HumanAgent(rapidjson::Document const &config, ControlQueue *control_queue);

  Game::Move ComputeMove(Game const &game) const override;

private:
  ControlQueue *control_queue_;
};

} // namespace subbuteo
