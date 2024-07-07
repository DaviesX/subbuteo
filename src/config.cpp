#include <SFML/Graphics.hpp>
#include <boost/json.hpp>
#include <eigen3/Eigen/Core>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "config.hpp"

namespace subbuteo {
namespace {

char const *kConfigFileName = "config.json";

boost::json::value ParseConfigFile(std::filesystem::path const &resource_path) {
  std::ifstream config_file(resource_path / kConfigFileName);
  std::stringstream buffer;
  buffer << config_file.rdbuf();
  return boost::json::parse(buffer.str());
}

} // namespace

Configuration::Configuration(std::filesystem::path const &resource_path) {
  boost::json::value json = ParseConfigFile(resource_path);
}

Configuration::~Configuration() = default;

std::string Configuration::Version() const { return version_; }

unsigned Configuration::TeamSize() const { return team_size_; }

Configuration::PhysicsParameters const &
Configuration::GoalKeeperPhysicsParameters() const {
  return goal_keeper_params_;
}

Configuration::PhysicsParameters const &
Configuration::FootballerPhysicsParameters() const {
  return foot_baller_params_;
}

std::vector<Eigen::Vector2f> const &Configuration::DefensePositions() const {
  return defense_positions_;
}

std::vector<Eigen::Vector2f> const &Configuration::OffensePositions() const {
  return offense_positions_;
}

std::vector<sf::Texture> const &Configuration::SoccererTextures() const {
  return soccerer_textures_;
}

} // namespace subbuteo
