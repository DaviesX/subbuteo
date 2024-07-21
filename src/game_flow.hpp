#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

#include "config.hpp"
#include "control.hpp"

namespace subbuteo {

class Scene;

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
  std::shared_ptr<Scene> scene_;
  std::shared_ptr<ControlQueue> control_queue_;
};

class LogGeneratorGameFlow final : public GameFlowInterface {
public:
  LogGeneratorGameFlow();
  ~LogGeneratorGameFlow() override;

  int Run() override;
};

} // namespace subbuteo
