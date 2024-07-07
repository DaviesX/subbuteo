#include <boost/log/attributes.hpp>
#include <boost/log/core.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <string>

#include "game_flow.hpp"

namespace {

char const *kArgLogGeneratorMode = "log-generator-mode";
char const *kArgMatchCount = "match-count";
char const *kArgPlayer0ModelPath = "player0-model-path";
char const *kArgPlayer1ModelPath = "player1-model-path";

} // namespace

int main(int argc, char *argv[]) {
  boost::log::core::get()->add_global_attribute(
      "Function", boost::log::attributes::mutable_constant<std::string>(""));

  boost::program_options::options_description desc;
  desc.add_options()("help", "produce help message")(
      kArgLogGeneratorMode, boost::program_options::value<bool>(),
      "Sets the program to generate and log new matches.")(
      kArgMatchCount, boost::program_options::value<unsigned>(),
      "The number of matches to generate.")(
      kArgPlayer0ModelPath, boost::program_options::value<std::string>(),
      "Sets the path to the model used by player 0.")(
      kArgPlayer1ModelPath, boost::program_options::value<std::string>(),
      "Sets the path to the model used by player 1.");

  boost::program_options::variables_map args;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), args);
  boost::program_options::notify(args);

  std::unique_ptr<subbuteo::GameFlowInterface> game_flow;
  if (args.count(kArgLogGeneratorMode) &&
      args[kArgLogGeneratorMode].as<bool>()) {
    game_flow = std::make_unique<subbuteo::LogGeneratorGameFlow>();
  } else {
    game_flow = std::make_unique<subbuteo::InteractiveGameFlow>();
  }

  return game_flow->Run();
}