
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <filesystem>
#include <functional>
#include <glog/logging.h>
#include <memory>
#include <ostream>
#include <thread>
#include <vector>

#include "agent.hpp"
#include "control.hpp"
#include "draw.hpp"
#include "game.hpp"
#include "game_flow.hpp"
#include "match.hpp"

namespace subbuteo {
namespace {

unsigned const kWindowWidth = 522u;
unsigned const kWindowHeight = 800u;
char const *kWindowTitle = "Subbuteo";
char const *kResourcePath = "./resource";

void DoInterfactiveMatchOpponent(Configuration const &config, Scene *scene,
                                 unsigned *player_0_params_index,
                                 unsigned *player_1_params_index) {
  scene->Clear();
  CHECK_GE(config.AvailableSoccererTextures().size(), 2);
  *player_0_params_index = 0;
  *player_1_params_index = 1;
}

void DoInteractivePickOffendingPlayer(Scene *scene, Game::Player *offense) {
  scene->Clear();
  *offense = Game::Player::PLAYER0;
}

void DoInteractiveMatch(Configuration const &config,
                        rapidjson::Document const &agent0_config,
                        rapidjson::Document const &agent1_config,
                        Camera *camera, ControlQueue *control_queue,
                        Scene *scene) {
  LOG(INFO) << "Matching opponents...";
  unsigned player_0_params_index;
  unsigned player_1_params_index;
  DoInterfactiveMatchOpponent(config, scene, &player_0_params_index,
                              &player_1_params_index);

  LOG(INFO) << "Picking offending player...";
  Game::Player offense;
  DoInteractivePickOffendingPlayer(scene, &offense);

  LOG(INFO) << "Loading match...";
  LoadMatch(config, offense, player_0_params_index, player_1_params_index,
            camera, scene);

  LOG(INFO) << "Staring match...";
  Game game(scene, offense, &config);
  std::unique_ptr<AgentInterface> agent0 =
      CreateAgent(agent0_config, control_queue);
  std::unique_ptr<AgentInterface> agent1 =
      CreateAgent(agent1_config, control_queue);
}

} // namespace

GameFlowInterface::GameFlowInterface(rapidjson::Document const &agent0_config,
                                     rapidjson::Document const &agent1_config)
    : agent0_config_(agent0_config), agent1_config_(agent1_config) {}

GameFlowInterface::~GameFlowInterface() = default;

InteractiveGameFlow::InteractiveGameFlow(
    rapidjson::Document const &agent0_config,
    rapidjson::Document const &agent1_config)
    : GameFlowInterface(agent0_config, agent1_config),
      window_({kWindowWidth, kWindowHeight}, kWindowTitle),
      config_(kResourcePath), camera_(window_.getSize()),
      scene_(/*visualizable=*/true), close_event_(false) {}

InteractiveGameFlow::~InteractiveGameFlow() {
  LOG(INFO) << "Releasing resources...";
}

int InteractiveGameFlow::Run() {
  LOG(INFO) << "InteractiveGameFlow started";

  LOG(INFO) << "Launching drawing thread...";
  window_.setActive(false);
  std::thread drawing_thread(DrawScene, std::cref(scene_), std::cref(camera_),
                             &close_event_, &window_);

  LOG(INFO) << "Launching matching thread...";
  std::thread match_thread(DoInteractiveMatch, std::cref(config_),
                           std::cref(agent0_config_), std::cref(agent1_config_),
                           &camera_, &control_queue_, &scene_);

  LOG(INFO) << "Listening controls...";
  close_event_ = ListenControls(&window_, std::cref(camera_), &control_queue_);
  CHECK(close_event_) << "Error listening to controls.";

  LOG(INFO) << "Waiting for matching thread to terminate...";
  match_thread.join();

  LOG(INFO) << "Waiting for drawing thread to terminate...";
  drawing_thread.join();

  LOG(INFO) << "Closing window...";
  window_.close();

  LOG(INFO) << "InteractiveGameFlow exiting...";
  return 0;
}

LogGeneratorGameFlow::LogGeneratorGameFlow(
    rapidjson::Document const &agent0_config,
    rapidjson::Document const &agent1_config)
    : GameFlowInterface(agent0_config, agent1_config) {}

LogGeneratorGameFlow::~LogGeneratorGameFlow() = default;

int LogGeneratorGameFlow::Run() {
  LOG(INFO) << "LogGeneratorGameFlow started";
  return 0;
}

} // namespace subbuteo
