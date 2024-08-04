#include <glog/logging.h>
#include <ostream>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

#include "agent.hpp"
#include "agent_human.hpp"

namespace subbuteo {

AgentInterface::AgentInterface() = default;
AgentInterface::~AgentInterface() = default;

AgentInterface::AgentId const &AgentInterface::Id() const { return id_; }

std::unique_ptr<AgentInterface> CreateAgent(rapidjson::Document const &config) {
  AgentInterface::AgentId id = config["id"].GetString();
  if (id == "human") {
    return std::make_unique<HumanAgent>(config);
  } else {
    CHECK(false) << "Unknown agent ID " << id;
    return nullptr;
  }
}

} // namespace subbuteo
