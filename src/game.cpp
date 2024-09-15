// IWYU pragma: no_include <__math/roots.h>
// IWYU pragma: no_include <__math/trigonometric_functions.h>
// IWYU pragma: no_include <SFML/System/Vector2.inl>

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

Scene::EntityId const kBallId = 1387;
Scene::EntityId const kFieldId = 387;
Scene::EntityId const kFieldBoundaryBaseId = 28;
Scene::EntityId const kSoccererBaseId = 9423;

unsigned const kFieldLayer = 0;
unsigned const kSoccererLayer = 1;
unsigned const kBallLayer = 1;

float const kFieldBounaryOverlap = 0.05f;

unsigned const kMinStableSteps = 5;

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

Game::Player PlayerOf(unsigned index) {
  if (index > 0) {
    return Game::Player::PLAYER0;
  } else {
    return Game::Player::PLAYER1;
  }
}

b2Vec2 ToB2Vec(sf::Vector2f const &v) { return b2Vec2(v.x, v.y); }

sf::Vector2f ToSfVec(b2Vec2 const &v) { return sf::Vector2f(v.x, v.y); }

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
                   unsigned player_params_index, Configuration const &config,
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

  CHECK_LT(player_params_index, config.AvailableSoccererTextures().size());
  const sf::Texture &texture =
      config.AvailableSoccererTextures()[player_params_index];

  unsigned soccerer_index = 0;
  for (auto const &[soccerer_type, soccerer_pos] : soccerer_positions) {
    Scene::EntityId id;
    Configuration::PhysicsParameters params;
    switch (soccerer_type) {
    case Configuration::SoccererType::FOOT_BALLER:
      id = FootBallerId(player, soccerer_index++);
      params = config.FootballerPhysicsParameters();
      break;
    case Configuration::SoccererType::GOAL_KEEPER:
      id = GoalKeeperId(player);
      params = config.GoalKeeperPhysicsParameters();
      break;
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
          body->SetLinearDamping(.2f);
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

void LoadBall(Configuration const &config, Scene *scene) {
  Configuration::PhysicsParameters const &params =
      config.BallPhysicsParameters();

  scene->AddEntity(
      kBallId, /*create_body_fn=*/
      [&params](b2World *world) {
        b2BodyDef body_def;
        body_def.type = b2_dynamicBody;
        body_def.position = b2Vec2(0, 0);

        b2CircleShape shape;
        shape.m_p = b2Vec2(0, 0);
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
      [&config, &params](DrawableWorld *world) {
        return world->CreateDrawable(
            /*position=*/sf::Vector2f(),
            sf::Vector2f(params.radius * 2, params.radius * 2), kBallLayer,
            /*angle=*/0, config.BallTexture());
      });
  config.AvailableSoccererTextures();
}

void LoadBoundary(sf::Vector2f const &start, sf::Vector2f const &stop,
                  unsigned boundary_index, Configuration const &config,
                  Scene *scene) {
  scene->AddEntity(
      kFieldBoundaryBaseId + boundary_index, /*create_body_fn=*/
      [&start, &stop, &config](b2World *world) {
        sf::Vector2f mid = 0.5f * (start + stop);

        b2BodyDef body_def;
        body_def.type = b2_staticBody;
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
               /*boundary_index=*/3, config, scene);
  // Top left bounary.
  LoadBoundary(/*start=*/sf::Vector2f(-goal_width / 2.f, dimension.y / 2.f),
               /*stop=*/sf::Vector2f(-dimension.x / 2.f, dimension.y / 2.f),
               /*boundary_index=*/4, config, scene);
  // Top right bounary.
  LoadBoundary(/*start=*/sf::Vector2f(dimension.x / 2.f, dimension.y / 2.f),
               /*stop=*/sf::Vector2f(goal_width / 2.f, dimension.y / 2.f),
               /*boundary_index=*/5, config, scene);
}

Game::Player GoalArea(WorldPosition const &ball_pos,
                      Configuration const &config) {
  if (ball_pos.y < -config.FieldDimension().y / 2.f) {
    return Game::Player::PLAYER0;
  } else if (ball_pos.y > config.FieldDimension().y / 2.f) {
    return Game::Player::PLAYER1;
  } else {
    return Game::Player::NONE;
  }
}

} // namespace

Game::Game(Scene *scene, Game::Player offense, Configuration const *config)
    : scene_(scene), current_state_(State::ONGOING), current_player_(offense),
      num_rounds_(0), config_(config) {}

Game::Game(Game &&other) : scene_(nullptr), num_rounds_(0) {
  std::swap(scene_, other.scene_);
  std::swap(current_state_, other.current_state_);
  std::swap(current_player_, other.current_player_);
  std::swap(num_rounds_, other.num_rounds_);
  std::swap(config_, other.config_);
}

bool Game::Launch(Move const &move) {
  if (PlayerOf(move.id) != this->CurrentPlayer()) {
    LOG(ERROR) << "Expected player of this turn is " << this->CurrentPlayer()
               << " but got " << PlayerOf(move.id) << " with id " << move.id;
    return false;
  }

  Scene::Entity soccerer = scene_->GetEntity(move.id);

  b2Vec2 dir(std::cos(move.angle), std::sin(move.angle));
  b2Vec2 force = move.power * config_->Launch().max_force * dir;
  soccerer.body->ApplyForceToCenter(force, /*wake=*/true);

  do {
    scene_->Step();

    WorldPosition ball_pos = this->CurrentBall();
    Player goal_area = GoalArea(ball_pos, *config_);
    switch (goal_area) {
    case Player::PLAYER0:
      current_state_ = State::PLAYER1_GOAL;
      break;
    case Player::PLAYER1:
      current_state_ = State::PLAYER0_GOAL;
      break;
    default:
      break;
    }
    if (current_state_ != State::ONGOING) {
      break;
    }
  } while (!scene_->Stable(kMinStableSteps));

  ++num_rounds_;
  return true;
}

unsigned Game::CurrentRound() const { return num_rounds_; }

Game::State Game::CurrentState() const { return current_state_; }

Game::Player Game::CurrentPlayer() const { return current_player_; }

std::vector<Game::Soccerer> Game::CurrentSoccerers() const {
  std::vector<Game::Soccerer> soccerers;
  soccerers.reserve(2 * config_->TeamSize());

  Scene::EntityId player0_goal_keeper_id = GoalKeeperId(Player::PLAYER0);
  Scene::EntityId player1_goal_keeper_id = GoalKeeperId(Player::PLAYER1);

  Scene::Entity player0_goal_keeper = scene_->GetEntity(player0_goal_keeper_id);
  Scene::Entity player1_goal_keeper = scene_->GetEntity(player1_goal_keeper_id);
  CHECK_NOTNULL(player0_goal_keeper.body);
  CHECK_NOTNULL(player1_goal_keeper.body);

  soccerers.push_back(Soccerer(player0_goal_keeper_id, Player::PLAYER0,
                               SoccererType::GoalKeeper,
                               ToSfVec(player0_goal_keeper.body->GetPosition()),
                               config_->GoalKeeperPhysicsParameters().radius));
  soccerers.push_back(Soccerer(player1_goal_keeper_id, Player::PLAYER1,
                               SoccererType::GoalKeeper,
                               ToSfVec(player1_goal_keeper.body->GetPosition()),
                               config_->GoalKeeperPhysicsParameters().radius));

  for (unsigned i = 0; i < config_->TeamSize() - 1; ++i) {
    Scene::EntityId player0_baller_id = FootBallerId(Player::PLAYER0, i);
    Scene::EntityId player1_baller_id = FootBallerId(Player::PLAYER1, i);

    Scene::Entity player0_baller = scene_->GetEntity(player0_baller_id);
    Scene::Entity player1_baller = scene_->GetEntity(player1_baller_id);
    CHECK_NOTNULL(player0_baller.body);
    CHECK_NOTNULL(player1_baller.body);

    soccerers.push_back(
        Soccerer(player0_baller_id, Player::PLAYER0, SoccererType::Baller,
                 ToSfVec(player0_baller.body->GetPosition()),
                 config_->FootballerPhysicsParameters().radius));
    soccerers.push_back(
        Soccerer(player1_baller_id, Player::PLAYER1, SoccererType::Baller,
                 ToSfVec(player1_baller.body->GetPosition()),
                 config_->FootballerPhysicsParameters().radius));
  }

  return soccerers;
}

WorldPosition Game::CurrentBall() const {
  Scene::Entity ball = scene_->GetEntity(kBallId);
  CHECK_NOTNULL(ball.body);
  b2Vec2 ball_pos = ball.body->GetPosition();
  return ToSfVec(ball_pos);
}

void LoadGameScene(Configuration const &config, Game::Player offense,
                   unsigned player_0_params_index,
                   unsigned player_1_params_index, Scene *scene) {
  LoadBall(config, scene);
  LoadField(config, scene);
  LoadSoccerers(Game::Player::PLAYER0, offense == Game::PLAYER0,
                player_0_params_index, config, scene);
  LoadSoccerers(Game::Player::PLAYER1, offense == Game::PLAYER1,
                player_1_params_index, config, scene);
}

std::ostream &operator<<(std::ostream &stream, Game::Move const &move) {
  stream << "{id:" << move.id << ",angle:" << move.angle
         << ",power:" << move.power << "}";
  return stream;
}

} // namespace subbuteo
