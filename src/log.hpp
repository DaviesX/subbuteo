#pragma once

#include <fstream>

#include "agent.hpp"
#include "game.hpp"

namespace subbuteo {

void LogMove(Game const &game, Game::Move const &move,
             AgentInterface::AgentId const &agent_id, std::ofstream *log_file);
void LogResult(Game const &game, std::ofstream *log_file);

} // namespace subbuteo
