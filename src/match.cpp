#include <cassert>
#include <filesystem>
#include <fstream>
#include <optional>

#include "agent.hpp"
#include "game.hpp"
#include "log.hpp"
#include "match.hpp"

namespace subbuteo {

void RunMatch(AgentInterface const &agent0, AgentInterface const &agent1,
              std::optional<std::filesystem::path> const &log_file_path,
              Game *game) {
  std::ofstream log_file;
  if (log_file_path.has_value()) {
    log_file.open(*log_file_path);
  }

  while (Game::State::ONGOING == game->CurrentState()) {
    Game::Move move;
    AgentInterface::AgentId agent_id;

    switch (game->CurrentPlayer()) {
    case Game::Player::PLAYER0: {
      move = agent0.ComputeMove(*game);
      agent_id = agent0.Id();
    }
    case Game::Player::PLAYER1: {
      move = agent1.ComputeMove(*game);
      agent_id = agent1.Id();
    }
    default:
      assert(false);
    }

    LogMove(*game, move, agent_id, &log_file);
    game->Launch(move);
  }

  LogResult(*game, &log_file);
  log_file.close();
}

} // namespace subbuteo
