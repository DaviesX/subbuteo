#include <SFML/Graphics.hpp>
#include <thread>

#include "control.hpp"
#include "draw.hpp"
#include "game_flow.hpp"

namespace subbuteo {
namespace {

unsigned const kWindowWidth = 505u;
unsigned const kWindowHeight = 672u;
char const *kWindowTitle = "Subbuteo";

} // namespace

GameFlowInterface::GameFlowInterface() = default;
GameFlowInterface::~GameFlowInterface() = default;

InteractiveGameFlow::InteractiveGameFlow()
    : window_({kWindowWidth, kWindowHeight}, kWindowTitle) {}

InteractiveGameFlow::~InteractiveGameFlow() = default;

int InteractiveGameFlow::Run() {
  std::thread draw_thread(subbuteo::DrawScene, scene_, &window_);
  subbuteo::ListenControls(&window_, &control_queue_);
  draw_thread.join();
  return 0;
}

LogGeneratorGameFlow::LogGeneratorGameFlow() = default;
LogGeneratorGameFlow::~LogGeneratorGameFlow() = default;

int LogGeneratorGameFlow::Run() { return 0; }

} // namespace subbuteo
