#pragma once

#include "Scene.h"

namespace Game
{

class PagodaScene : public Engine::Scene
{
public:

	PagodaScene(const Engine::GraphicsDevice& device) : Scene(device)
	{
	}
	void Load(const Engine::GraphicsDevice& device) override;
};

} // namespace Game
