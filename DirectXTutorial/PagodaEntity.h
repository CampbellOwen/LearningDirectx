#pragma once
#include "Entity.h"

namespace Engine
{

class PagodaEntity :
   public Entity
{
public:
   void Update() override;
   void DrawUI() override;
private:
   struct UIState
   {
      float x, y, z;
      float rotx, roty, rotz;
   };

private:
   UIState m_uiState;
};

} // namespace Engine

