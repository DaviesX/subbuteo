#include <rapidjson/document.h>

#include "agent_human.hpp"
#include "control.hpp"

namespace subbuteo {

HumanAgent::HumanAgent(rapidjson::Document const &config,
                       ControlQueue *control_queue)
    : control_queue_(control_queue) {}

Game::Move HumanAgent::ComputeMove(Game const &gamee) const {}

} // namespace subbuteo
