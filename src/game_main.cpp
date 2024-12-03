#include <gflags/gflags.h>
#include <glog/logging.h>
#include <memory>
#include <ostream>
#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/encodings.h>
#include <rapidjson/rapidjson.h>
#include <string>

#include "game_flow.hpp"

DEFINE_bool(log_generator_mode, false,
            "Sets the program to generate and log new matches.");
DEFINE_uint32(match_count, 1, "The number of matches to generate.");
DEFINE_string(agent_0_config, R"({"id": "human", "settings": {}})",
              "Sets the configuration string for agent 0.");
DEFINE_string(agent_1_config, R"({"id": "human", "settings": {}})",
              "Sets the configuration string for agent 1.");

int main(int argc, char *argv[]) {
  FLAGS_alsologtostderr = true;
  gflags::ParseCommandLineFlags(&argc, &argv, /*remove_flags=*/true);
  google::InitGoogleLogging(argv[0]);

  LOG(INFO) << "Creating agents...";
  rapidjson::Document agent0_config;
  rapidjson::Document agent1_config;
  agent0_config.Parse<0>(FLAGS_agent_0_config.c_str());
  agent1_config.Parse<0>(FLAGS_agent_1_config.c_str());
  CHECK(!agent0_config.HasParseError());
  CHECK(!agent1_config.HasParseError());

  LOG(INFO) << "Creating game flow...";
  std::unique_ptr<subbuteo::GameFlowInterface> game_flow;
  if (FLAGS_match_count > 0 && FLAGS_log_generator_mode) {
    game_flow = std::make_unique<subbuteo::LogGeneratorGameFlow>(agent0_config,
                                                                 agent1_config);
  } else {
    game_flow = std::make_unique<subbuteo::InteractiveGameFlow>(agent0_config,
                                                                agent1_config);
  }

  LOG(INFO) << "Running game flow...";
  return game_flow->Run();
}