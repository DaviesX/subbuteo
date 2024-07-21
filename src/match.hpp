#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include "agent.hpp"
#include "config.hpp"
#include "control.hpp"
#include "game.hpp"

namespace subbuteo {

struct Match {
  std::shared_ptr<Game> game;
  std::shared_ptr<ControlQueue> control_queue;
};

Match CreateMatch(Configuration const &config,
                  std::shared_ptr<ControlQueue> const &control_queue,
                  std::shared_ptr<Scene> const &scene);
Game::State RunMatch(AgentInterface const &agent0, AgentInterface const &agent1,
                     std::optional<std::filesystem::path> const &log_file_path,
                     Match *match);

} // namespace subbuteo
