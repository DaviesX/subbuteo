#pragma once

#include <filesystem>
#include <optional>

#include "agent.hpp"
#include "game.hpp"

namespace subbuteo {

void RunMatch(AgentInterface const &agent0, AgentInterface const &agent1,
              std::optional<std::filesystem::path> const &log_file_path,
              Game *game);

} // namespace subbuteo
