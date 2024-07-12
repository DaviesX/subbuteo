#pragma once

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "control.hpp"
#include "draw.hpp"

namespace subbuteo {

class GameFlowInterface {
public:
  GameFlowInterface();
  virtual ~GameFlowInterface();

  virtual int Run() = 0;
};

class InteractiveGameFlow final : public GameFlowInterface {
public:
  InteractiveGameFlow();
  ~InteractiveGameFlow() override;

  int Run() override;

private:
  sf::RenderWindow window_;
  Configuration config_;
  Scene scene_;
  ControlQueue control_queue_;
};

class LogGeneratorGameFlow final : public GameFlowInterface {
public:
  LogGeneratorGameFlow();
  ~LogGeneratorGameFlow() override;

  int Run() override;
};

} // namespace subbuteo
