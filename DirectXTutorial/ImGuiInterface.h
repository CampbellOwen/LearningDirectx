#pragma once

#include <d3d11.h>
#include "ImGUI\imgui.h"
#include "ImGUI\imgui_impl_win32.h"
#include "ImGUI\imgui_impl_dx11.h"
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

	float backgroundColor[4] = { 0.122f, 0.133f, 0.149f, 1.0f };

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

struct SceneControls : Controls
{
	float lightx;
	float lighty;
	float lightz;

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
	SceneControls& SceneState();

private:
	ObjectControls m_ObjectControls;
	CameraControls m_CameraControls;
	SceneControls m_SceneControls;
};

} // namespace UI
} // namespace Engine
