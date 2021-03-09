#pragma once
#include "Scene.h"

namespace Game {

class SphereScene : public Engine::Scene
{
public:
	SphereScene(const Engine::GraphicsDevice& device) : Scene(device)
	{
	}
	void Load(const Engine::GraphicsDevice& device) override;
};

} // namespace Game

