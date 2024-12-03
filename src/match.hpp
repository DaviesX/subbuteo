#pragma once

#include <filesystem>
#include <optional>

#include "config.hpp"
#include "game.hpp"

namespace subbuteo {

class AgentInterface;
struct Camera;
class Scene;

void LoadMatch(Configuration const &config, Game::Player offense,
               unsigned player_0_params_index, unsigned player_1_params_index,
               Camera *camera, Scene *scene);
Game::State RunMatch(AgentInterface const &agent0, AgentInterface const &agent1,
                     std::optional<std::filesystem::path> const &log_file_path,
                     Game *game);

} // namespace subbuteo
