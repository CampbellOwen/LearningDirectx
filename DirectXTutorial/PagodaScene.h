#pragma once

#include "Scene.h"

namespace Game
{

class PagodaScene : public Engine::Scene
{
	void Load(const Engine::GraphicsDevice& device) override;
};

} // namespace Game
