#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <rapidjson/document.h>

#include "camera.hpp"
#include "config.hpp"
#include "control.hpp"
#include "scene.hpp"

namespace subbuteo {

class GameFlowInterface {
public:
  GameFlowInterface(rapidjson::Document const &agent0_config,
                    rapidjson::Document const &agent1_config);
  virtual ~GameFlowInterface();

  virtual int Run() = 0;

protected:
  rapidjson::Document const &agent0_config_;
  rapidjson::Document const &agent1_config_;
};

class InteractiveGameFlow final : public GameFlowInterface {
public:
  InteractiveGameFlow(rapidjson::Document const &agent0_config,
                      rapidjson::Document const &agent1_config);
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
  LogGeneratorGameFlow(rapidjson::Document const &agent0_config,
                       rapidjson::Document const &agent1_config);
  ~LogGeneratorGameFlow() override;

  int Run() override;
};

} // namespace subbuteo
