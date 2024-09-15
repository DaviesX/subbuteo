// IWYU pragma: no_include <__fwd/fstream.h>
// IWYU pragma: no_include <__fwd/sstream.h>
// IWYU pragma: no_include <SFML/System/Vector2.inl>

#include <filesystem>
#include <fstream>
#include <glog/logging.h>
#include <limits>
#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/encodings.h>
#include <rapidjson/rapidjson.h>
#include <regex>
#include <sstream>
#include <string>
#include <utility>

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
  CHECK(!result.HasParseError())
      << "Error parsing " << (resource_path / kConfigFileName).string();
  return result;
}

Configuration::SoccererType ToSoccererType(std::string const &type_str) {
  if (type_str == "foot_baller") {
    return Configuration::SoccererType::FOOT_BALLER;
  } else if (type_str == "goal_keeper") {
    return Configuration::SoccererType::GOAL_KEEPER;
  } else {
    CHECK(false) << "Unknown soccerer type " << type_str;
    return Configuration::SoccererType::INVALID;
  }
}

std::vector<Configuration::FieldPosition>
ToFieldPositions(rapidjson::Value const &pos_configs) {
  CHECK(pos_configs.IsArray());

  std::vector<Configuration::FieldPosition> result;
  result.reserve(pos_configs.Size());

  for (unsigned i = 0; i < pos_configs.Size(); ++i) {
    result.push_back(Configuration::FieldPosition(
        ToSoccererType(pos_configs[i]["type"].GetString()),
        pos_configs[i]["from_left"].GetFloat(),
        pos_configs[i]["from_central"].GetFloat()));
  }

  return result;
}

} // namespace

Configuration::PhysicsParameters::PhysicsParameters()
    : radius(std::numeric_limits<double>::quiet_NaN()),
      density(std::numeric_limits<double>::quiet_NaN()),
      friction(std::numeric_limits<double>::quiet_NaN()) {}

Configuration::PhysicsParameters::PhysicsParameters(float radius, float density,
                                                    float friction)
    : radius(radius), density(density), friction(friction) {}

Configuration::FieldPosition::FieldPosition()
    : soccerer_type(SoccererType::INVALID),
      from_left(std::numeric_limits<double>::quiet_NaN()),
      from_central(std::numeric_limits<double>::quiet_NaN()) {}

Configuration::FieldPosition::FieldPosition(SoccererType soccerer_type,
                                            float from_left, float from_central)
    : soccerer_type(soccerer_type), from_left(from_left),
      from_central(from_central) {}

Configuration::LaunchParameters::LaunchParameters()
    : min_force(0), max_force(0), uncertainty_force(0), uncertainty_angle(0) {}

Configuration::LaunchParameters::LaunchParameters(float min_force,
                                                  float max_force,
                                                  float uncertainty_force,
                                                  float uncertainty_angle)
    : min_force(min_force), max_force(max_force),
      uncertainty_force(uncertainty_force),
      uncertainty_angle(uncertainty_angle) {}

Configuration::Configuration(std::filesystem::path const &resource_path) {
  CHECK(std::filesystem::exists(resource_path));
  rapidjson::Document config = ParseConfigFile(resource_path);

  version_ = config["version"].GetString();
  team_size_ = config["team_size"].GetInt();

  rapidjson::Value launch_config = config["launch"].GetObject();
  launch_params_ =
      LaunchParameters(launch_config["min_force"].GetFloat(),
                       launch_config["max_force"].GetFloat(),
                       launch_config["uncertainty_force"].GetFloat(),
                       launch_config["uncertainty_angle"].GetFloat());

  rapidjson::Value ball_config = config["ball"].GetObject();
  ball_params_ =
      PhysicsParameters(/*radius=*/ball_config["radius"].GetFloat(),
                        /*density=*/ball_config["density"].GetFloat(),
                        /*friction=*/ball_config["friction"].GetFloat());
  sf::Texture ball_texture;
  CHECK(ball_texture.loadFromFile(
      (resource_path / ball_config["file"].GetString()).string()));
  ball_texture_ = std::move(ball_texture);

  rapidjson::Value field_config = config["field"].GetObject();
  field_dimension_ = sf::Vector2f(field_config["width"].GetFloat(),
                                  field_config["length"].GetFloat());
  field_params_ =
      PhysicsParameters(/*radius=*/0, /*density=*/0,
                        /*friction=*/field_config["floor_friction"].GetFloat());
  field_bounary_params_ = PhysicsParameters(
      /*radius=*/0, /*density=*/0,
      /*friction=*/field_config["boundary_friction"].GetFloat());
  CHECK(field_texture_.loadFromFile(
      (resource_path / field_config["file"].GetString()).string()));

  rapidjson::Value goal_config = config["goal"].GetObject();
  goal_width_ = goal_config["width"].GetFloat();

  rapidjson::Value goal_keeper_config = config["goal_keeper"].GetObject();
  goal_keeper_params_ =
      PhysicsParameters(goal_keeper_config["radius"].GetFloat(),
                        goal_keeper_config["density"].GetFloat(),
                        goal_keeper_config["friction"].GetFloat());
  rapidjson::Value foot_baller_config = config["foot_baller"].GetObject();
  foot_baller_params_ =
      PhysicsParameters(foot_baller_config["radius"].GetFloat(),
                        foot_baller_config["density"].GetFloat(),
                        foot_baller_config["friction"].GetFloat());
  std::regex soccerer_texture_re(foot_baller_config["file_glob"].GetString());
  for (std::filesystem::directory_entry const &entry :
       std::filesystem::directory_iterator(resource_path)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    if (!std::regex_match(entry.path().filename().string(),
                          soccerer_texture_re)) {
      continue;
    }
    sf::Texture soccerer_texture;
    CHECK(soccerer_texture.loadFromFile(entry.path().string()));
    available_soccerer_textures_.push_back(std::move(soccerer_texture));
  }

  offense_positions_ = ToFieldPositions(config["offense_positions"].GetArray());
  defense_positions_ = ToFieldPositions(config["defense_positions"].GetArray());
}

Configuration::~Configuration() = default;

std::string const &Configuration::Version() const { return version_; }

unsigned Configuration::TeamSize() const { return team_size_; }

Configuration::LaunchParameters const &Configuration::Launch() const {
  return launch_params_;
}

sf::Texture const &Configuration::BallTexture() const { return ball_texture_; }

sf::Vector2f const &Configuration::FieldDimension() const {
  return field_dimension_;
}

sf::Texture const &Configuration::FieldTexture() const {
  return field_texture_;
}

float Configuration::GoalWidth() const { return goal_width_; }

Configuration::PhysicsParameters const &
Configuration::BallPhysicsParameters() const {
  return ball_params_;
}

Configuration::PhysicsParameters const &
Configuration::FieldPhysicsParameters() const {
  return field_params_;
}

Configuration::PhysicsParameters const &
Configuration::FieldBoundaryPhysicsParameters() const {
  return field_bounary_params_;
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
