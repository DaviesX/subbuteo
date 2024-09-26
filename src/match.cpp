// IWYU pragma: no_include <__fwd/fstream.h>
// IWYU pragma: no_include <SFML/System/Vector2.inl>

#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <fstream>
#include <glog/logging.h>
#include <optional>
#include <string>

#include "agent.hpp"
#include "camera.hpp"
#include "coordinate.hpp"
#include "drawable.hpp"
#include "game.hpp"
#include "log.hpp"
#include "match.hpp"
#include "scene.hpp"

namespace subbuteo {
namespace {

unsigned const kScoreboardLayer = 0;
Scene::EntityId const kScoreBoardId = 74198;
float const kScoreBoardHeight = 9;

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

  scene->AddEntity(
      /*id=*/kScoreBoardId,
      /*create_body_fn=*/nullptr,
      /*create_drawable_fn=*/[&config](DrawableWorld *world) {
        return world->CreateDrawable(
            WorldPosition(0, config.FieldDimension().y / 2.f +
                                 kScoreBoardHeight / 2.f),
            sf::Vector2f(config.FieldDimension().x, kScoreBoardHeight),
            kScoreboardLayer,
            /*angle=*/0.f, config.ScoreBoardTexture());
      });
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
      break;
    }
    case Game::Player::PLAYER1: {
      move = agent1.ComputeMove(*game);
      agent_id = agent1.Id();
      break;
    }
    default:
      CHECK(false) << "Unknown player " << game->CurrentPlayer();
    }

    LogMove(*game, move, agent_id, &log_file);
    if (!game->Launch(move)) {
      LOG(ERROR) << "Failed to launch the move.";
    }
  }

  LogResult(*game, &log_file);
  log_file.close();

  return game->CurrentState();
}

} // namespace subbuteo
