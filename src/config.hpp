#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <filesystem>
#include <string>
#include <vector>

#include "coordinate.hpp"

namespace subbuteo {

class Configuration {
public:
  struct PhysicsParameters {
    float radius;
    float mass;
  };

  Configuration(std::filesystem::path const &resource_path);
  ~Configuration();

  std::string Version() const;
  unsigned TeamSize() const;
  PhysicsParameters const &GoalKeeperPhysicsParameters() const;
  PhysicsParameters const &FootballerPhysicsParameters() const;
  std::vector<WorldPosition> const &DefensePositions() const;
  std::vector<WorldPosition> const &OffensePositions() const;
  std::vector<sf::Texture> const &SoccererTextures() const;

private:
  std::string version_;
  unsigned team_size_;
  PhysicsParameters goal_keeper_params_;
  PhysicsParameters foot_baller_params_;
  std::vector<WorldPosition> defense_positions_;
  std::vector<WorldPosition> offense_positions_;
  std::vector<sf::Texture> soccerer_textures_;
};

} // namespace subbuteo
