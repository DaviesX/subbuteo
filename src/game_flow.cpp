
#include <SFML/System/String.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <filesystem>
#include <glog/logging.h>
#include <ostream>
#include <thread>

#include "control.hpp"
#include "draw.hpp"
#include "game_flow.hpp"

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
      config_(kResourcePath), camera_(window_.getSize()),
      scene_(/*visualizable=*/true), close_event_(false) {}

InteractiveGameFlow::~InteractiveGameFlow() {
  LOG(INFO) << "Releasing resources...";
}

int InteractiveGameFlow::Run() {
  LOG(INFO) << "InteractiveGameFlow started";

  LOG(INFO) << "Launching drawing thread...";
  window_.setActive(false);
  std::thread drawing_thread(subbuteo::DrawScene, scene_, camera_,
                             &close_event_, &window_);

  LOG(INFO) << "Listening controls...";
  close_event_ = subbuteo::ListenControls(&window_, camera_, &control_queue_);
  CHECK(close_event_) << "Error listening to controls.";

  LOG(INFO) << "Waiting for drawing thread to terminate...";
  drawing_thread.join();

  LOG(INFO) << "Closing window...";
  window_.close();

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
