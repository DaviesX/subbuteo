#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace subbuteo {

class Configuration {
public:
  enum SoccererType {
    INVALID,
    FOOT_BALLER,
    GOAL_KEEPER,
  };

  struct PhysicsParameters {
    PhysicsParameters();
    PhysicsParameters(float radius, float density, float friction);

    float radius;
    float density;
    float friction;
  };

  struct FieldPosition {
    FieldPosition();
    FieldPosition(SoccererType soccerer_type, float from_left,
                  float from_central);

    SoccererType soccerer_type;
    float from_left;
    float from_central;
  };

  struct LaunchParameters {
    LaunchParameters();
    LaunchParameters(float min_impulse, float max_impulse,
                     float uncertainty_force, float uncertainty_angle);

    float min_impulse;
    float max_impulse;
    float uncertainty_force;
    float uncertainty_angle;
  };

  Configuration(std::filesystem::path const &resource_path);
  ~Configuration();

  std::string const &Version() const;
  unsigned TeamSize() const;
  sf::Texture const &ScoreBoardTexture() const;
  LaunchParameters const &Launch() const;
  sf::Texture const &BallTexture() const;
  sf::Vector2f const &FieldDimension() const;
  float FieldLinearDamping() const;
  sf::Texture const &FieldTexture() const;
  sf::Vector2f const &GoalDimension() const;
  PhysicsParameters const &BallPhysicsParameters() const;
  PhysicsParameters const &FieldBoundaryPhysicsParameters() const;
  PhysicsParameters const &GoalKeeperPhysicsParameters() const;
  PhysicsParameters const &FootballerPhysicsParameters() const;
  std::vector<FieldPosition> const &DefensePositions() const;
  std::vector<FieldPosition> const &OffensePositions() const;
  std::vector<sf::Texture> const &AvailableSoccererTextures() const;

private:
  std::string version_;
  unsigned team_size_;
  sf::Texture score_board_texture_;
  LaunchParameters launch_params_;
  sf::Texture ball_texture_;
  sf::Vector2f field_dimension_;
  float field_linear_damping_;
  sf::Texture field_texture_;
  sf::Vector2f goal_dimension_;
  PhysicsParameters ball_params_;
  PhysicsParameters field_bounary_params_;
  PhysicsParameters goal_keeper_params_;
  PhysicsParameters foot_baller_params_;
  std::vector<FieldPosition> defense_positions_;
  std::vector<FieldPosition> offense_positions_;
  std::vector<sf::Texture> available_soccerer_textures_;
};

} // namespace subbuteo
