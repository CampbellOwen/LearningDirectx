#pragma once

#include "Mesh.h"

namespace Engine
{
class ImplicitMesh final : public Mesh
{
public:
	ImplicitMesh(size_t numVertices) {
		m_numVertices = numVertices;
	}
};
} // namespace Engine
