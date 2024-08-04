#pragma once

#include <rapidjson/document.h>

#include "agent.hpp"
#include "control.hpp"
#include "game.hpp"

namespace subbuteo {

class HumanAgent final : public AgentInterface {
public:
  explicit HumanAgent(rapidjson::Document const &config);

  Game::Move ComputeMove(Game const &game,
                         ControlQueue *control_queue) const override;
};

} // namespace subbuteo
