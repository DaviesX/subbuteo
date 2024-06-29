#pragma once

#include <memory>

namespace subbuteo {

class PhysicsInterface {
  public:
    virtual std::unique_ptr<PhysicsInterface> Duplicate() = 0;
};

} // namespace subbuteo
