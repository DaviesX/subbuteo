// IWYU pragma: no_include <__fwd/fstream.h>
// IWYU pragma: no_include <__fwd/sstream.h>
// IWYU pragma: no_include <SFML/System/Vector2.inl>

#include <filesystem>
#include <fstream>
#include <glog/logging.h>
#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/encodings.h>
#include <rapidjson/rapidjson.h>
#include <sstream>
#include <string>

#include "config.hpp"

namespace subbuteo {
namespace {

char const *kConfigFileName = "config.json";

rapidjson::Document
ParseConfigFile(std::filesystem::path const &resource_path) {
  std::ifstream config_file(resource_path / kConfigFileName);
  CHECK(config_file.is_open())
      << "Error opening " << (resource_path / kConfigFileName).string();
  std::stringstream buffer;
  buffer << config_file.rdbuf();

  rapidjson::Document result;
  result.Parse<0>(buffer.str().c_str());
  CHECK(!result.HasParseError()) << "Error parsing " << kConfigFileName;
  return result;
}

} // namespace

Configuration::Configuration(std::filesystem::path const &resource_path) {
  rapidjson::Document config = ParseConfigFile(resource_path);
}

Configuration::~Configuration() = default;

std::string const &Configuration::Version() const { return version_; }

unsigned Configuration::TeamSize() const { return team_size_; }

sf::Vector2f const &Configuration::FieldDimension() const {
  return field_dimension_;
}

Configuration::PhysicsParameters const &
Configuration::GoalKeeperPhysicsParameters() const {
  return goal_keeper_params_;
}

Configuration::PhysicsParameters const &
Configuration::FootballerPhysicsParameters() const {
  return foot_baller_params_;
}

std::vector<Configuration::FieldPosition> const &
Configuration::DefensePositions() const {
  return defense_positions_;
}

std::vector<Configuration::FieldPosition> const &
Configuration::OffensePositions() const {
  return offense_positions_;
}

std::vector<sf::Texture> const &
Configuration::AvailableSoccererTextures() const {
  return available_soccerer_textures_;
}

} // namespace subbuteo
