#include <glog/logging.h>
#include <ostream>
#include <stddef.h>
#include <utility>

#include "scene.hpp"

class b2Body;

namespace subbuteo {
namespace {

size_t const kExpectedNumEntities = 20;

} // namespace

Scene::Scene(bool visualizable) : visualizable_(visualizable) {
  entities_.reserve(kExpectedNumEntities);
}

Scene::Scene(Scene const &other) : visualizable_(other.visualizable_) {}

bool Scene::Visualizable() const { return visualizable_; }

void Scene::AddEntity(EntityId id, CreateBodyFn const &create_body_fn,
                      CreateDrawableFn const &create_drawable_fn) {
  b2Body *body = nullptr;
  Drawable *drawable = nullptr;
  if (create_body_fn != nullptr) {
    CHECK_NOTNULL(physics_world_);
    body = create_body_fn(physics_world_.get());
  }
  if (visualizable_ && create_drawable_fn != nullptr) {
    CHECK_NOTNULL(drawable_world_);
    drawable = create_drawable_fn(drawable_world_.get());
  }

  bool inserted =
      entities_.insert(std::make_pair(id, Entity(body, drawable))).second;
  CHECK(inserted) << "Entity with ID " << id << " has already been added.";
}

Scene::Entity Scene::GetEntity(EntityId id) const {
  auto it = entities_.find(id);
  CHECK(it != entities_.end()) << "Entity with ID " << id << " doesn't exist.";
  return it->second;
}

std::unordered_map<Scene::EntityId, Scene::Entity> const &
Scene::Entities() const {
  return entities_;
}

void Scene::Step() {}

} // namespace subbuteo