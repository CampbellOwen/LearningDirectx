#pragma once

#include <d3d11.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <windows.h>

#include "Interface.h"

namespace Engine
{
namespace UI 
{

struct ObjectControls : Controls
{
	 float x;
	 float y;
	 float z;

	 float rotx;
	 float roty;
	 float rotz;

	 // Default cornflower blue :)
	 float backgroundColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };

	 void Render() override;
};

struct CameraControls : Controls
{
	 float fov;
	 float aspectRatio;
	 float nearZ;
	 float farZ;

	 void Render() override;
};

class ImGuiInterface : Interface
{
public:
	 void Init(HWND hWnd, ID3D11Device* dev, ID3D11DeviceContext* devCon);
	 ~ImGuiInterface();

	 void Render() override;

	 ObjectControls& ObjectState();
	 CameraControls& CameraState();

private:
	 ObjectControls m_ObjectControls;
	 CameraControls m_CameraControls;
};

} // namespace UI
} // namespace Engine
