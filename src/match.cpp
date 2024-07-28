// IWYU pragma: no_include <__fwd/fstream.h>

#include <filesystem>
#include <fstream>
#include <glog/logging.h>
#include <memory>
#include <optional>
#include <string>

#include "agent.hpp"
#include "game.hpp"
#include "log.hpp"
#include "match.hpp"

namespace subbuteo {

void LoadMatch(Configuration const &config, Game::Player offense,
               unsigned player_0_texture_index, unsigned player_1_texture_index,
               std::shared_ptr<Scene> const &scene) {
  // TODO: Loads the scoreboard etc.
  LoadGame(config, offense, player_0_texture_index, player_1_texture_index,
           scene);
}

Game::State RunMatch(AgentInterface const &agent0, AgentInterface const &agent1,
                     std::optional<std::filesystem::path> const &log_file_path,
                     Match *match) {
  std::ofstream log_file;
  if (log_file_path.has_value()) {
    log_file.open(*log_file_path);
  }

  while (Game::State::ONGOING == match->game->CurrentState()) {
    Game::Move move;
    AgentInterface::AgentId agent_id;

    switch (match->game->CurrentPlayer()) {
    case Game::Player::PLAYER0: {
      move = agent0.ComputeMove(*match->game, match->control_queue.get());
      agent_id = agent0.Id();
    }
    case Game::Player::PLAYER1: {
      move = agent1.ComputeMove(*match->game, match->control_queue.get());
      agent_id = agent1.Id();
    }
    default:
      CHECK(false) << "Unknown player " << match->game->CurrentPlayer();
    }

    LogMove(*match->game, move, agent_id, &log_file);
    match->game->Launch(move);
  }

  LogResult(*match->game, &log_file);
  log_file.close();

  return match->game->CurrentState();
}

} // namespace subbuteo
