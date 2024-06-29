#pragma once

#include "agent.hpp"
#include "game.hpp"
#include "log.hpp"

namespace subbuteo {

void RunMatch(AgentInterface const &agent0, AgentInterface const &agent1, Game *game,
              Logger *logger);

} // namespace subbuteo
