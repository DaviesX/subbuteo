// IWYU pragma: no_include <__math/roots.h>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_edge_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <cmath>
#include <glog/logging.h>
#include <numbers>
#include <ostream>
#include <utility>

#include "drawable.hpp"
#include "game.hpp"

namespace subbuteo {
namespace {

Scene::EntityId const kFieldId = 387;
Scene::EntityId const kFieldBoundaryBaseId = 28;
Scene::EntityId const kSoccererBaseId = 9423;

unsigned const kFieldLayer = 0;
unsigned const kSoccererLayer = 1;

float const kFieldBounaryOverlap = 0.05f;

Scene::EntityId GoalKeeperId(Game::Player player) {
  switch (player) {
  case Game::Player::PLAYER0:
    return kSoccererBaseId;
  case Game::Player::PLAYER1:
    return -kSoccererBaseId;
  default:
    CHECK(false) << "Unknown player " << player;
    return 0;
  }
}

Scene::EntityId FootBallerId(Game::Player player, unsigned index) {
  switch (player) {
  case Game::Player::PLAYER0:
    return kSoccererBaseId + 1 + index;
  case Game::Player::PLAYER1:
    return -(kSoccererBaseId + 1 + index);
  default:
    CHECK(false) << "Unknown player " << player;
    return 0;
  }
}

b2Vec2 ToB2Vec(const sf::Vector2f &v) { return b2Vec2(v.x, v.y); }

std::vector<std::pair<Configuration::SoccererType, WorldPosition>>
ToWorldPositions(
    std::vector<Configuration::FieldPosition> const &relative_positions,
    sf::Vector2f const &field_dimension, Game::Player player) {
  std::vector<std::pair<Configuration::SoccererType, WorldPosition>> result;
  result.reserve(relative_positions.size());

  for (auto const &pos : relative_positions) {
    float sign;
    switch (player) {
    case Game::Player::PLAYER0:
      sign = 1.0f;
      break;
    case Game::Player::PLAYER1:
      sign = -1.0f;
      break;
    default:
      CHECK(false) << "Unknown player " << player;
      sign = 0;
    }

    float x = (pos.from_left - 0.5) * field_dimension.x;
    float y = sign * pos.from_central * field_dimension.y / 2.0f;
    result.push_back(std::make_pair(pos.soccerer_type, WorldPosition(x, y)));
  }

  return result;
}

void LoadSoccerers(Game::Player player, bool offense,
                   unsigned player_texture_index, Configuration const &config,
                   Scene *scene) {
  std::vector<std::pair<Configuration::SoccererType, WorldPosition>>
      soccerer_positions;
  soccerer_positions.reserve(config.TeamSize());
  if (offense) {
    soccerer_positions = ToWorldPositions(config.OffensePositions(),
                                          config.FieldDimension(), player);
  } else {
    soccerer_positions = ToWorldPositions(config.DefensePositions(),
                                          config.FieldDimension(), player);
  }

  CHECK_LT(player_texture_index, config.AvailableSoccererTextures().size());
  const sf::Texture &texture =
      config.AvailableSoccererTextures()[player_texture_index];

  unsigned soccerer_index = 0;
  for (auto const &[soccerer_type, soccerer_pos] : soccerer_positions) {
    Scene::EntityId id;
    Configuration::PhysicsParameters params;
    switch (soccerer_type) {
    case Configuration::SoccererType::FOOT_BALLER:
      id = FootBallerId(player, soccerer_index++);
      params = config.FootballerPhysicsParameters();
    case Configuration::SoccererType::GOAL_KEEPER:
      id = GoalKeeperId(player);
      params = config.GoalKeeperPhysicsParameters();
    default:
      CHECK(false) << "Unknown soccerer type " << soccerer_type;
      id = 0;
    }

    float angle;
    switch (player) {
    case Game::Player::PLAYER0:
      angle = std::numbers::pi;
      break;
    case Game::Player::PLAYER1:
      angle = 0;
      break;
    default:
      CHECK(false) << "Unknown player " << player;
      angle = 0;
    }

    scene->AddEntity(
        id,
        /*create_body_fn=*/
        [&soccerer_pos = soccerer_pos, &params](b2World *world) {
          b2BodyDef body_def;
          body_def.type = b2_dynamicBody;
          body_def.position = ToB2Vec(soccerer_pos);

          b2CircleShape shape;
          shape.m_p = ToB2Vec(soccerer_pos);
          shape.m_radius = params.radius;

          b2FixtureDef fixture_def;
          fixture_def.shape = &shape;
          fixture_def.density = params.density;
          fixture_def.friction = params.friction;

          b2Body *body = world->CreateBody(&body_def);
          body->CreateFixture(&fixture_def);

          return body;
        },
        /*create_drawable_fn=*/
        [&soccerer_pos = soccerer_pos, &params, angle,
         &texture](DrawableWorld *world) {
          return world->CreateDrawable(
              soccerer_pos, sf::Vector2f(params.radius * 2, params.radius * 2),
              kSoccererLayer, angle, texture);
        });
  }
}

sf::Vector2f ComputeNormalizedDirection(sf::Vector2f const &start,
                                        sf::Vector2f const &stop) {
  sf::Vector2f dir = stop - start;
  float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
  return 1.0f / length * dir;
}

void LoadBoundary(sf::Vector2f const &start, sf::Vector2f const &stop,
                  unsigned boundary_index, Configuration const &config,
                  Scene *scene) {
  scene->AddEntity(
      kFieldBoundaryBaseId + boundary_index, /*create_body_fn=*/
      [&start, &stop, &config](b2World *world) {
        sf::Vector2f mid = 0.5f * (start + stop);

        b2BodyDef body_def;
        body_def.position = ToB2Vec(mid);

        b2EdgeShape shape;
        sf::Vector2f dir = ComputeNormalizedDirection(start, stop);
        sf::Vector2f pre_start = start - kFieldBounaryOverlap * dir;
        sf::Vector2f post_stop = stop + kFieldBounaryOverlap * dir;
        shape.SetOneSided(ToB2Vec(pre_start), ToB2Vec(start), ToB2Vec(stop),
                          ToB2Vec(post_stop));

        b2FixtureDef fixture_def;
        fixture_def.shape = &shape;
        fixture_def.friction = config.FieldBoundaryPhysicsParameters().friction;

        b2Body *body = world->CreateBody(&body_def);
        body->CreateFixture(&fixture_def);

        return body;
      },
      /*create_drawable_fn=*/nullptr);
}

void LoadField(Configuration const &config, Scene *scene) {
  scene->AddEntity(kFieldId, /*create_body_fn=*/nullptr,
                   /*create_drawable_fn=*/[&config](DrawableWorld *world) {
                     return world->CreateDrawable(
                         /*position=*/sf::Vector2f(), config.FieldDimension(),
                         kFieldLayer,
                         /*angle=*/0, config.FieldTexture());
                   });

  sf::Vector2f const &dimension = config.FieldDimension();
  float goal_width = config.GoalWidth();

  // Left boundary.
  LoadBoundary(/*start=*/sf::Vector2f(-dimension.x / 2.f, -dimension.y / 2.f),
               /*stop=*/sf::Vector2f(-dimension.x / 2.f, dimension.y / 2.f),
               /*boundary_index=*/0, config, scene);
  // Right boundary.
  LoadBoundary(/*start=*/sf::Vector2f(dimension.x / 2.f, dimension.y / 2.f),
               /*stop=*/sf::Vector2f(dimension.x / 2.f, -dimension.y / 2.f),
               /*boundary_index=*/1, config, scene);
  // Bottom left bounary.
  LoadBoundary(/*start=*/sf::Vector2f(-goal_width / 2.f, -dimension.y / 2.f),
               /*stop=*/sf::Vector2f(-dimension.x / 2.f, -dimension.y / 2.f),
               /*boundary_index=*/2, config, scene);
  // Bottom right bounary.
  LoadBoundary(/*start=*/sf::Vector2f(dimension.x / 2.f, -dimension.y / 2.f),
               /*stop=*/sf::Vector2f(goal_width / 2.f, -dimension.y / 2.f),
               /*boundary_index=*/2, config, scene);
  // Top left bounary.
  LoadBoundary(/*start=*/sf::Vector2f(-goal_width / 2.f, dimension.y / 2.f),
               /*stop=*/sf::Vector2f(-dimension.x / 2.f, dimension.y / 2.f),
               /*boundary_index=*/2, config, scene);
  // Top right bounary.
  LoadBoundary(/*start=*/sf::Vector2f(dimension.x / 2.f, dimension.y / 2.f),
               /*stop=*/sf::Vector2f(goal_width / 2.f, dimension.y / 2.f),
               /*boundary_index=*/2, config, scene);
}

} // namespace

Game::Game(Scene const *scene) : scene_(scene) {}

Game::Game(Game &&other) : scene_(nullptr) { std::swap(scene_, other.scene_); }

void LoadGame(Configuration const &config, Game::Player offense,
              unsigned player_0_texture_index, unsigned player_1_texture_index,
              Scene *scene) {
  LoadField(config, scene);
  LoadSoccerers(Game::Player::PLAYER0, offense == Game::PLAYER0,
                player_0_texture_index, config, scene);
  LoadSoccerers(Game::Player::PLAYER1, offense == Game::PLAYER1,
                player_1_texture_index, config, scene);
}

} // namespace subbuteo
