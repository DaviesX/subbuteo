#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include "agent.hpp"
#include "config.hpp"
#include "control.hpp"
#include "draw.hpp"
#include "game.hpp"

namespace subbuteo {

struct Match {
  std::shared_ptr<Game> game;
  Scene *scene;
  ControlQueue *control_queue;
};

Match CreateMatch(Configuration const &config, Scene *scene,
                  ControlQueue *control_queue);
Game::State RunMatch(AgentInterface const &agent0, AgentInterface const &agent1,
                     std::optional<std::filesystem::path> const &log_file_path,
                     Match *match);

} // namespace subbuteo
