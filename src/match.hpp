#pragma once

#include <filesystem>
#include <optional>

#include "config.hpp"
#include "control.hpp"
#include "game.hpp"

namespace subbuteo {

class AgentInterface;
struct Camera;
class Scene;

void LoadMatch(Configuration const &config, Game::Player offense,
               unsigned player_0_texture_index, unsigned player_1_texture_index,
               Camera *camera, Scene *scene);
Game::State RunMatch(AgentInterface const &agent0, AgentInterface const &agent1,
                     std::optional<std::filesystem::path> const &log_file_path,
                     ControlQueue *control_queue, Scene *scene);

} // namespace subbuteo
