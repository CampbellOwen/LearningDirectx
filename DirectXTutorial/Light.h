#pragma once
#include "Entity.h"

namespace Engine 
{

struct LightConstantBuffer
{
	DirectX::XMFLOAT4 position;
	int type;
	int padding[3];
};

class Light : public Entity
{
public:
	Light(std::string name) : Entity(name)
	{
	}
	void UpdateConstantBuffer(LightConstantBuffer* buffer);
	void DrawUI() override;
};

} // namespace Engine

