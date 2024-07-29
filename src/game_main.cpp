#include <gflags/gflags.h>
#include <glog/logging.h>
#include <memory>

#include "game_flow.hpp"

DEFINE_bool(log_generator_mode, false,
            "Sets the program to generate and log new matches.");
DEFINE_uint32(match_count, 1, "The number of matches to generate.");
DEFINE_string(player_0_model_path, "",
              "Sets the path to the model used by player 0.");
DEFINE_string(player_1_model_path, "",
              "Sets the path to the model used by player 1.");

int main(int argc, char *argv[]) {
  FLAGS_alsologtostderr = true;

  gflags::ParseCommandLineFlags(&argc, &argv, /*remove_flags=*/true);
  google::InitGoogleLogging(argv[0]);

  std::unique_ptr<subbuteo::GameFlowInterface> game_flow;
  if (FLAGS_match_count > 0 && FLAGS_log_generator_mode) {
    game_flow = std::make_unique<subbuteo::LogGeneratorGameFlow>();
  } else {
    game_flow = std::make_unique<subbuteo::InteractiveGameFlow>();
  }

  return game_flow->Run();
}