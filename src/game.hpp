#pragma once

#include <eigen3/Eigen/Core>
#include <memory>
#include <vector>

#include "config.hpp"
#include "scene.hpp"

namespace subbuteo {

class Game {
public:
  enum Player { NONE, PLAYER0, PLAYER1 };
  enum SoccererType { Baller, GoalKeeper };
  enum State { ONGOING, PLAYER0_GOAL, PLAYER1_GOAL };

  using SoccererId = unsigned;

  struct Soccerer {
    SoccererId id;
    Player owner;
    SoccererType type;
    Eigen::Vector2f position;
  };

  struct Ball {
    Eigen::Vector2f position;
  };

  struct Move {
    SoccererId id;
    float angle;
    float power;
  };

  Game(Configuration const *config, std::shared_ptr<Scene> const &scene);
  Game(Game const &other);

  void Launch(Move const &move);
  unsigned CurrentRound() const;
  State CurrentState() const;
  Player CurrentPlayer() const;
  std::vector<Soccerer> CurrentSoccerers() const;
  Eigen::Vector2f CurrentBall() const;

private:
  std::shared_ptr<Scene> scene_;
};

} // namespace subbuteo
