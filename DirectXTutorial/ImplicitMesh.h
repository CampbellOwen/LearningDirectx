#pragma once

#include "Mesh.h"

namespace Engine
{
class ImplicitMesh final : public Mesh
{
public:
	ImplicitMesh(size_t numIndices) {
		m_numIndices = numIndices;
	}
};
} // namespace Engine
