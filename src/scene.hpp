#pragma once

#include <box2d/b2_world.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <shared_mutex>
#include <unordered_map>

#include "drawable.hpp"

class b2Body;

namespace subbuteo {

class Scene {
public:
  using EntityId = int64_t;

  using CreateBodyFn = std::function<b2Body *(b2World *world)>;
  using CreateDrawableFn = std::function<Drawable *(DrawableWorld *world)>;

  struct Entity {
    Entity(b2Body *body,  Drawable *drawable)
        : body(body), drawable(drawable) {}

    b2Body *body;
    Drawable *drawable;
  };

  Scene(bool visualizable);
  Scene(Scene const &) = delete;

  bool Visualizable() const;
  void AddEntity(EntityId id, CreateBodyFn const &create_body_fn,
                 CreateDrawableFn const &create_drawable_fn);
  Entity GetEntity(EntityId id) const;
  std::unordered_map<EntityId, Entity> Entities() const;
  void Clear();
  void Step();

private:
  std::shared_mutex mu_;
  bool const visualizable_;
  std::unordered_map<EntityId, Entity> entities_;
  std::unique_ptr<b2World> physics_world_;
  std::unique_ptr<DrawableWorld> drawable_world_;
};

} // namespace subbuteo
