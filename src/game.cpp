#include "game.hpp"

namespace subbuteo {

Game::Game(Configuration const &config, std::shared_ptr<Scene> const &scene)
    : scene_(scene) {}

Game::Game(Game const &other)
    : scene_(std::make_shared<Scene>(*other.scene_)) {}

} // namespace subbuteo
