#include <random>
#include "AsteroidsScene.hpp"
#include "cephalopod/texture.hpp"
#include "cephalopod/game.hpp"
#include "cephalopod/actions.hpp"
#include "cephalopod/actionconstraints.hpp"
#include "cephalopod/easingactions.hpp"
#include "cephalopod/group.hpp"
#include "cephalopod/sprite.hpp"
#include "Ship.hpp"

std::random_device rd; // obtain a random number from hardware
std::mt19937 eng( rd()  );

void Asteroids::initialize()
{
	sprite_sheet_ = ceph::SpriteSheet::create(
		".\\data\\zarquon.png",
		".\\data\\zarquon.json" 
	);
	setBackgroundColor(ceph::ColorRGB(10, 30, 90));
	
	addActors({
		CreateStarLayer( -256.0f, 0.5f ),
		CreateStarLayer( -128.0f, 0.25f ),
		CreateStarLayer( -64.0f, 0.125f ),
		CreateStarLayer( -32.0f, 0.0625f )
	});

	addActor(
		bkgd_layer_ = ceph::Actor::create<ceph::Group>()
	);

	auto ship = ceph::Actor::create<Ship>(sprite_sheet_);

	ship->setPosition(700, 300);
	ship->getActions().applyConstraint(
		std::make_shared<ceph::WrapTorroidally>( 40.0f, 40.0f )
	);
	addActor(ship);
	ship->connect(updateEvent, &Ship::update);

	addAsteroid( "medium_asteroid_", 24 );
}

std::shared_ptr<ceph::Group> Asteroids::getBkgdLayer() const
{
	return bkgd_layer_;
}

std::shared_ptr<ceph::Actor> Asteroids::CreateStarLayer(float horz_speed, float alpha)
{
	auto bounds = ceph::Game::getInstance().getLogicalRect();

	std::uniform_int_distribution<> star_distr(1, 7);
	std::uniform_real_distribution<> x_distr(bounds.x, bounds.x2() - horz_speed);
	std::uniform_real_distribution<> y_distr(bounds.y, bounds.y2());

	auto layer = ceph::Actor::create<ceph::Group>();
	for (int i = 0; i < 45; i++) {
		auto star_id = "star_" + std::to_string(star_distr(eng));
		auto star = ceph::Actor::create<ceph::Sprite>(sprite_sheet_, star_id);
		auto& star_actions = star->getActions();
		star->setAnchorPt(0.5f, 0.5f);
		star->setPosition( static_cast<float>(x_distr( eng )), static_cast<float>(y_distr( eng )) );
		star->setAlpha(alpha);
		star_actions.applyAction(
			ceph::createMoveByAction(2.0f, horz_speed, 0), true
		);
		star_actions.applyAction(
			ceph::createRotateByAction(1.0f, 1.0f), true
		);
		star_actions.applyConstraint(
			std::make_shared<ceph::WrapTorroidally>(10.0f, 0.0f)
		);
		layer->addChild(star);
	}

	return layer;
}

void Asteroids::addAsteroid(const std::string& frame_prefix, int n )
{
	auto asteroid = ceph::Actor::create<ceph::Sprite>(sprite_sheet_, frame_prefix + "0");
	asteroid->getActions().applyAction(
		ceph::createMoveByAction(1.0f, ceph::Vec2D<float>(100.0f, 100.0f)),
		true
	);

	std::vector<std::string> frames;
	for (int i = 0; i < n; i++)
		frames.push_back(frame_prefix + std::to_string(i));

	asteroid->getActions().applyAction(
		ceph::createAnimationAction(0.025f, frames.begin(), frames.end()),
		true
	);
	
	asteroid->getActions().applyConstraint(
		std::make_shared<ceph::WrapTorroidally>(100.0f, 100.0f)
	);
	addActor(asteroid);
}
