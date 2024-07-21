#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <algorithm>
#include <glog/logging.h>
#include <memory>
#include <ostream>
#include <unordered_map>
#include <vector>

#include "draw.hpp"
#include "drawable.hpp"
#include "scene.hpp"

namespace subbuteo {
namespace {

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

void DrawScene(std::shared_ptr<Scene const> const &scene,
               sf::RenderWindow *window) {
  LOG(INFO) << "DrawScene: started";

  while (window->isOpen()) {
    window->clear(sf::Color::White);

    std::vector<Drawable const *> drawables = ToSortedDrawables(*scene);
    for (auto drawable : drawables) {
      window->draw(drawable->sprite);
      if (drawable->material != nullptr) {
        sf::Sprite material = drawable->material->Sprite(drawable->sprite);
        window->draw(material);
      }
    }

    window->display();
  }

  LOG(INFO) << "DrawScene: exiting...";
}

} // namespace subbuteo
