#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

#include "camera.hpp"
#include "config.hpp"
#include "control.hpp"
#include "scene.hpp"

namespace subbuteo {

class AgentInterface;

class GameFlowInterface {
public:
  GameFlowInterface(std::unique_ptr<AgentInterface> &&agent0,
                    std::unique_ptr<AgentInterface> &&agent1);
  virtual ~GameFlowInterface();

  virtual int Run() = 0;

protected:
  std::unique_ptr<AgentInterface> agent0_;
  std::unique_ptr<AgentInterface> agent1_;
};

class InteractiveGameFlow final : public GameFlowInterface {
public:
  InteractiveGameFlow(std::unique_ptr<AgentInterface> &&agent0,
                      std::unique_ptr<AgentInterface> &&agent1);
  ~InteractiveGameFlow() override;

  int Run() override;

private:
  sf::RenderWindow window_;
  Configuration config_;
  Camera camera_;
  Scene scene_;
  ControlQueue control_queue_;
  bool close_event_;
};

class LogGeneratorGameFlow final : public GameFlowInterface {
public:
  LogGeneratorGameFlow(std::unique_ptr<AgentInterface> &&agen0,
                       std::unique_ptr<AgentInterface> &&agent1);
  ~LogGeneratorGameFlow() override;

  int Run() override;
};

} // namespace subbuteo
