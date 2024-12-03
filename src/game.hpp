#pragma once

// IWYU pragma: no_include <__fwd/ostream.h>
// IWYU pragma: no_include <SFML/System/Vector2.inl>

#include <SFML/System/Vector2.hpp>
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
    Soccerer(Scene::EntityId id, Player owner, SoccererType type,
             WorldPosition position, float radius)
        : id(id), owner(owner), type(type), position(position), radius(radius) {
    }

    Scene::EntityId id;
    Player owner;
    SoccererType type;
    WorldPosition position;
    float radius;
  };

  struct Move {
    Move() : id(0), angle(0), power(0) {}
    Move(Scene::EntityId id, float angle, float power)
        : id(id), angle(angle), power(power) {}

    Scene::EntityId id;
    float angle;
    float power;
  };

  Game(Scene *scene, Game::Player offense, Configuration const *config);
  Game(Game &&other);
  Game(Game const &) = delete;

  bool Launch(Move const &move);
  unsigned CurrentRound() const;
  State CurrentState() const;
  Player CurrentPlayer() const;
  std::vector<Soccerer> CurrentSoccerers() const;
  WorldPosition CurrentBall() const;

private:
  Scene *scene_;
  State current_state_;
  Player current_player_;
  unsigned num_rounds_;
  Configuration const *config_;
};

void LoadGameScene(Configuration const &config, Game::Player offense,
                   unsigned player_0_params_index,
                   unsigned player_1_params_index, Scene *scene);
std::ostream &operator<<(std::ostream &stream, Game::Move const &move);

} // namespace subbuteo
