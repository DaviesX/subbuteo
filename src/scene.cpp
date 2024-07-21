#include <glog/logging.h>

#include "scene.hpp"

namespace subbuteo {

Scene::Scene(bool visualizable) : visualizable_(visualizable) {}

Scene::Scene(Scene const &other) : visualizable_(other.visualizable_) {}

Scene::EntityId Scene::AddEntity(b2Body *body, Drawable &&drawable) {}

Scene::Entity const &Scene::GetEntity(EntityId id) const {
  CHECK_GE(entities_.count(id), 0);
  return entities_.at(id);
}

std::unordered_map<Scene::EntityId, Scene::Entity> const &
Scene::Entities() const {
  return entities_;
}

void Scene::Step() {}

} // namespace subbuteo