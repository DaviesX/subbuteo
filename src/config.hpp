#pragma once

#include <SFML/Graphics.hpp>
#include <eigen3/Eigen/Core>
#include <filesystem>
#include <vector>

namespace subbuteo {

class Configuration {
  public:
    struct PhysicsParameters {
        float radius;
        float mass;
    };

    Configuration(std::filesystem::path const &resource_path);
    ~Configuration();

    unsigned TeamSize() const;
    PhysicsParameters const &GoalKeeperPhysicsParameters() const;
    PhysicsParameters const &FootballerPhysicsParameters() const;
    std::vector<Eigen::Vector2f> const &DefensePositions() const;
    std::vector<Eigen::Vector2f> const &OffensePositions() const;
    std::vector<sf::Texture> const &SoccererTextures() const;

  private:
    unsigned team_size_;
    PhysicsParameters goal_keeper_params_;
    PhysicsParameters foot_baller_params_;
    std::vector<Eigen::Vector2f> defense_positions_;
    std::vector<Eigen::Vector2f> offense_positions_;
    std::vector<sf::Texture> soccerer_textures_;
};

} // namespace subbuteo
