
#include <SFML/System/String.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <filesystem>
#include <glog/logging.h>
#include <ostream>
#include <thread>

#include "control.hpp"
#include "draw.hpp"
#include "game_flow.hpp"
#include "scene.hpp"

namespace subbuteo {
namespace {

unsigned const kWindowWidth = 505u;
unsigned const kWindowHeight = 672u;
char const *kWindowTitle = "Subbuteo";
char const *kResourcePath = "./resource";

} // namespace

GameFlowInterface::GameFlowInterface() = default;
GameFlowInterface::~GameFlowInterface() = default;

InteractiveGameFlow::InteractiveGameFlow()
    : window_({kWindowWidth, kWindowHeight}, kWindowTitle),
      config_(kResourcePath),
      scene_(std::make_shared<Scene>(/*visualizable=*/true)),
      control_queue_(std::make_shared<ControlQueue>()) {}

InteractiveGameFlow::~InteractiveGameFlow() {
  LOG(INFO) << "Releasing resources...";
}

int InteractiveGameFlow::Run() {
  LOG(INFO) << "InteractiveGameFlow started";

  LOG(INFO) << "Launching drawing thread...";
  std::thread drawing_thread(subbuteo::DrawScene, scene_, &window_);

  LOG(INFO) << "Listening controls...";
  subbuteo::ListenControls(&window_, control_queue_);

  LOG(INFO) << "Waiting for drawing thread to terminate...";
  drawing_thread.join();

  LOG(INFO) << "InteractiveGameFlow exiting...";
  return 0;
}

LogGeneratorGameFlow::LogGeneratorGameFlow() = default;
LogGeneratorGameFlow::~LogGeneratorGameFlow() = default;

int LogGeneratorGameFlow::Run() {
  LOG(INFO) << "LogGeneratorGameFlow started";
  return 0;
}

} // namespace subbuteo
