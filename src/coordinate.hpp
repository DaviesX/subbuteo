#pragma once

#include <SFML/System.hpp>
#include <eigen3/Eigen/Core>

namespace subbuteo {

using WindowPosition = Eigen::Vector2i;
using ScreenPosition = Eigen::Vector2f;
using MatchPosition = Eigen::Vector2f;

inline ScreenPosition CreateScreenPosition(unsigned x, unsigned y,
                                           sf::Vector2u const &win_size) {
  return ScreenPosition(static_cast<ScreenPosition::value_type>(x) / win_size.x,
                        static_cast<ScreenPosition::value_type>(y) /
                            win_size.y);
}

} // namespace subbuteo
