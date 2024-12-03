// IWYU pragma: no_include <SFML/System/Vector2.inl>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <glog/logging.h>
#include <ostream>
#include <unordered_map>
#include <vector>

#include "camera.hpp"
#include "draw.hpp"
#include "drawable.hpp"
#include "scene.hpp"

namespace subbuteo {
namespace {

unsigned const kFrameRateLimit = 30;

std::vector<Drawable const *> ToSortedDrawables(const Scene &scene) {
  std::vector<Drawable const *> result;
  result.reserve(scene.Entities().size());
  for (auto const &[_, entity] : scene.Entities()) {
    if (entity.drawable == nullptr) {
      continue;
    }
    result.push_back(entity.drawable);
  }

  std::sort(
      result.begin(), result.end(),
      [](Drawable const *a, Drawable const *b) { return a->layer < b->layer; });
  return result;
}

} // namespace

void DrawScene(Scene const &scene, Camera const &camera,
               bool const *close_event, sf::RenderWindow *window) {
  LOG(INFO) << "DrawScene started";

  if (!scene.Visualizable()) {
    LOG(INFO) << "Scene is not visualizable. DrawScene exiting...";
    return;
  }

  window->setFramerateLimit(kFrameRateLimit);
  while (!*close_event) {
    window->clear(sf::Color::Transparent);

    std::vector<Drawable const *> drawables = ToSortedDrawables(scene);
    for (auto drawable : drawables) {
      sf::Sprite sprite(drawable->texture);

      sf::Vector2f scale = ComputeWindowScale(camera, drawable->dimension,
                                              drawable->texture.getSize());
      sf::Vector2f center = ComputeWindowPosition(camera, drawable->position);
      sprite.setOrigin(drawable->texture.getSize().x / 2.f,
                       drawable->texture.getSize().y / 2.f);
      sprite.setPosition(center);
      sprite.setScale(scale);
      sprite.setRotation(drawable->angle);
      window->draw(sprite);

      if (drawable->material != nullptr) {
        sf::Sprite material_sprite = drawable->material->Sprite(sprite);
        window->draw(material_sprite);
      }
    }

    window->display();
  }

  LOG(INFO) << "DrawScene exiting...";
}

} // namespace subbuteo
