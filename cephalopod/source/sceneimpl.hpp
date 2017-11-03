#pragma once

#include <memory>
#include "SFML/Graphics.hpp"
#include "../include/cephalopod/texture.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/game.hpp"

namespace ceph
{
	class SceneImpl
	{
	public:
		std::vector<std::shared_ptr<Actor>> stage_;
		std::vector<std::pair<Actor*, UpdateHandlerFunc>> updateHandlers_;
		std::shared_ptr<Texture> bkgd_tex_;
		sf::Sprite bkgd_;
		sf::Transform bkgd_mat_;
		ceph::CoordinateMapping coordinate_mapping_mode_;

		SceneImpl();
		void drawBackground(const DrawingContext& rt);
		void setBackground(const std::shared_ptr<Texture>& tex, ceph::CoordinateMapping mapping);
	};
}
