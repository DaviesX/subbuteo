#include <boost/log/trivial.hpp>

#include "scene.hpp"

namespace subbuteo {

Scene::Scene(bool visualizable) : visualizable_(visualizable) {}

Scene::Scene(Scene const &other) : visualizable_(other.visualizable_) {}

Scene::Entity const &Scene::GetEntity(EntityId id) const {
  BOOST_ASSERT(entities_.count(id) > 0);
  return entities_.at(id);
}

std::unordered_map<Scene::EntityId, Scene::Entity> const &
Scene::Entities() const {
  return entities_;
}

void Scene::Step() {}

} // namespace subbuteo