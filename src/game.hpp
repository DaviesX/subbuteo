#pragma once

// IWYU pragma: no_include <SFML/System/Vector2.inl>

#include <memory>
#include <vector>

#include "config.hpp"
#include "coordinate.hpp"
#include "scene.hpp"

namespace subbuteo {

class Game {
public:
  enum Player { NONE, PLAYER0, PLAYER1 };
  enum SoccererType { Baller, GoalKeeper };
  enum State { ONGOING, PLAYER0_GOAL, PLAYER1_GOAL };

  struct Soccerer {
    Scene::EntityId id;
    Player owner;
    SoccererType type;
    WorldPosition position;
  };

  struct Ball {
    WorldPosition position;
  };

  struct Move {
    Scene::EntityId id;
    float angle;
    float power;
  };

  Game(std::shared_ptr<Scene> const &scene);
  Game(Game const &other);

  void Launch(Move const &move);
  unsigned CurrentRound() const;
  State CurrentState() const;
  Player CurrentPlayer() const;
  std::vector<Soccerer> CurrentSoccerers() const;
  WorldPosition CurrentBall() const;

private:
  std::shared_ptr<Scene> scene_;
};

void LoadGame(Configuration const &config, Game::Player offense,
              unsigned player_0_texture_index, unsigned player_1_texture_index,
              std::shared_ptr<Scene> const &scene);

} // namespace subbuteo
