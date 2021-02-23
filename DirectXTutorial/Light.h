#pragma once
#include "Entity.h"

namespace Engine 
{

struct LightConstantBuffer
{
	int type;
	DirectX::XMFLOAT3 padding;
	DirectX::XMFLOAT4 position;
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

