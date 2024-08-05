#include <box2d/b2_body.h>
#include <box2d/b2_math.h>
#include <box2d/b2_types.h>
#include <glog/logging.h>
#include <mutex>
#include <ostream>
#include <stddef.h>
#include <utility>

#include "scene.hpp"

namespace subbuteo {
namespace {

size_t const kExpectedNumEntities = 20;
float const kTimeStep = 1.f / 60.f;
int32 const kVelocityIterations = 6;
int32 const kPositionIterations = 2;

} // namespace

Scene::Scene(bool visualizable)
    : visualizable_(visualizable),
      physics_world_(std::make_unique<b2World>(b2Vec2(0, 0))),
      drawable_world_(std::make_unique<DrawableWorld>()) {
  entities_.reserve(kExpectedNumEntities);
}


bool Scene::Visualizable() const { return visualizable_; }

void Scene::AddEntity(EntityId id, CreateBodyFn const &create_body_fn,
                      CreateDrawableFn const &create_drawable_fn) {
  std::unique_lock lock(mu_);

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
  std::shared_lock lock(const_cast<Scene *>(this)->mu_);
  auto it = entities_.find(id);
  CHECK(it != entities_.end()) << "Entity with ID " << id << " doesn't exist.";
  return it->second;
}

std::unordered_map<Scene::EntityId, Scene::Entity> Scene::Entities() const {
  std::shared_lock lock(const_cast<Scene *>(this)->mu_);
  return entities_;
}

void Scene::Clear() {
  std::unique_lock lock(mu_);

  for (auto &[_, entity] : entities_) {
    if (entity.body != nullptr) {
      physics_world_->DestroyBody(entity.body);
    }
  }
  drawable_world_->ClearDrawables();
  entities_.clear();
}

void Scene::Step() {
  physics_world_->Step(kTimeStep, kVelocityIterations, kPositionIterations);
  if (!visualizable_) {
    return;
  }

  std::unique_lock lock(mu_);
  for (auto &[_, entity] : entities_) {
    if (entity.body != nullptr && entity.drawable != nullptr) {
      b2Vec2 pos = entity.body->GetPosition();
      float angle = entity.body->GetAngle();
      entity.drawable->position.x = pos.x;
      entity.drawable->position.y = pos.y;
      entity.drawable->angle = angle;
    }
  }
}

} // namespace subbuteo