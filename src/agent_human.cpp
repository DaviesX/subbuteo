#include <rapidjson/document.h>

#include "agent_human.hpp"

namespace subbuteo {

HumanAgent::HumanAgent(rapidjson::Document const &config) {}

Game::Move HumanAgent::ComputeMove(Game const &game,
                                   ControlQueue *control_queue) const {}

} // namespace subbuteo
