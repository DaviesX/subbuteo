// IWYU pragma: no_include <__fwd/fstream.h>

#include <filesystem>
#include <fstream>
#include <glog/logging.h>
#include <optional>
#include <string>

#include "agent.hpp"
#include "camera.hpp"
#include "game.hpp"
#include "log.hpp"
#include "match.hpp"

namespace subbuteo {
namespace {

float kScoreBoardHeight = 9;

} // namespace

void LoadMatch(Configuration const &config, Game::Player offense,
               unsigned player_0_params_index, unsigned player_1_params_index,
               Camera *camera, Scene *scene) {
  CHECK_NOTNULL(camera);
  CHECK_NOTNULL(scene);

  camera->center.x = 0;
  camera->center.y = kScoreBoardHeight / 2;
  camera->view_dimension.x = config.FieldDimension().x;
  camera->view_dimension.y = config.FieldDimension().y + kScoreBoardHeight;

  // TODO: Loads the scoreboard etc.
  return LoadGameScene(config, offense, player_0_params_index,
                       player_1_params_index, scene);
}

Game::State RunMatch(AgentInterface const &agent0, AgentInterface const &agent1,
                     std::optional<std::filesystem::path> const &log_file_path,
                     Game *game) {
  CHECK_NOTNULL(game);

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
      CHECK(false) << "Unknown player " << game->CurrentPlayer();
    }

    LogMove(*game, move, agent_id, &log_file);
    game->Launch(move);
  }

  LogResult(*game, &log_file);
  log_file.close();

  return game->CurrentState();
}

} // namespace subbuteo
