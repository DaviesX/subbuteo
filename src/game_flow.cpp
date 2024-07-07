#include <SFML/Graphics.hpp>
#include <boost/log/trivial.hpp>
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

InteractiveGameFlow::~InteractiveGameFlow() {
  BOOST_LOG_TRIVIAL(debug) << "InteractiveGameFlow: Releasing resources...";
}

int InteractiveGameFlow::Run() {
  BOOST_LOG_TRIVIAL(debug) << "InteractiveGameFlow: started";

  BOOST_LOG_TRIVIAL(debug)
      << "InteractiveGameFlow: Launching drawing thread...";
  std::thread drawing_thread(subbuteo::DrawScene, scene_, &window_);

  BOOST_LOG_TRIVIAL(debug) << "InteractiveGameFlow: Listening controls...";
  subbuteo::ListenControls(&window_, &control_queue_);

  BOOST_LOG_TRIVIAL(debug)
      << "InteractiveGameFlow: Waiting for drawing thread to terminate...";
  drawing_thread.join();

  BOOST_LOG_TRIVIAL(debug) << "InteractiveGameFlow: exiting...";
  return 0;
}

LogGeneratorGameFlow::LogGeneratorGameFlow() = default;
LogGeneratorGameFlow::~LogGeneratorGameFlow() = default;

int LogGeneratorGameFlow::Run() {
  BOOST_LOG_TRIVIAL(debug) << "LogGeneratorGameFlow: started";
  return 0;
}

} // namespace subbuteo
