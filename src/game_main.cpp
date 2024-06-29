#include <SFML/Graphics.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <string>

#include "draw.hpp"

namespace {

unsigned const kWindowWidth = 505u;
unsigned const kWindowHeight = 672u;
char const *kWindowTitle = "Subbuteo";

} // namespace

int main(int argc, char *argv[]) {
    boost::program_options::options_description desc;
    desc.add_options()("help", "produce help message")(
        "log-generator-mode", boost::program_options::value<bool>(),
        "Sets the program to generate and log new matches.")(
        "match-count", boost::program_options::value<unsigned>(),
        "The number of matches to generate.")("player0-model-path",
                                              boost::program_options::value<std::string>(),
                                              "Sets the path to the model used by player 0.")(
        "player1-model-path", boost::program_options::value<std::string>(),
        "Sets the path to the model used by player 1.");

    boost::program_options::variables_map args;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc),
                                  args);
    boost::program_options::notify(args);

    sf::RenderWindow window({kWindowWidth, kWindowHeight}, kWindowTitle);
    subbuteo::Loop();
    return 0;
}