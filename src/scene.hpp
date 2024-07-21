#pragma once

#include <box2d/b2_world.h>
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "drawable.hpp"

class b2Body;

namespace subbuteo {

class Scene {
public:
  using EntityId = int64_t;

  struct Entity {
    b2Body *body;
    Drawable *drawable;
  };

  Scene(bool visualizable);
  Scene(Scene const &other);

  EntityId AddEntity(b2Body *body, Drawable &&drawable);
  Entity const &GetEntity(EntityId id) const;
  std::unordered_map<EntityId, Entity> const &Entities() const;
  void Step();

private:
  bool const visualizable_;
  std::unordered_map<EntityId, Entity> entities_;
  std::unique_ptr<b2World> physics_world_;
  std::unique_ptr<DrawableWorld> drawable_world_;
};

} // namespace subbuteo
