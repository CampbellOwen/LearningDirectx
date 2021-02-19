#include "Light.h"

namespace Engine 
{


void Light::UpdateConstantBuffer(LightConstantBuffer* buffer)
{
   buffer->position = DirectX::XMFLOAT4(
      m_position.x,
      m_position.y,
      m_position.z,
      1.0f
   );
   buffer->type = 0;
}

void Light::DrawUI()
{
   Entity::DrawUI();
}

} // namespace Engine
