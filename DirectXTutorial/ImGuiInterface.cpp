#include "ImGuiInterface.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Engine
{
namespace UI 
{

void CameraControls::Render()
{
	ImGui::Begin("Camera Controls");
	ImGui::SliderFloat("FOV", &fov, 0.1f, M_PI);
	ImGui::SliderFloat("Aspect Ratio", &aspectRatio, 0.0f, 3.0f);
	ImGui::SliderFloat("Near Z", &nearZ, 0.001f, 10.0f);
	ImGui::SliderFloat("Far Z", &farZ, 0, 100.0f);
	ImGui::End();
}

void ObjectControls::Render()
{

	ImGui::Begin("Object Controls");

	ImGui::Text("Position");
	ImGui::SliderFloat("X", &x, -30.0f, 30.0f);
	ImGui::SliderFloat("Y", &y, -30.0f, 30.0f);
	ImGui::SliderFloat("Z", &z, -30.0f, 30.0f);

	ImGui::Text("Rotation");
	ImGui::SliderFloat("Rotation X", &rotx, 0.0f, M_PI * 2.0f);
	ImGui::SliderFloat("Rotation Y", &roty, 0.0f, M_PI * 2.0f);
	ImGui::SliderFloat("Rotation Z", &rotz, 0.0f, M_PI * 2.0f);

	ImGui::Text("Clear Colour");
	ImGui::ColorEdit3("clear color", (float*)&backgroundColor);

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void ImGuiInterface::Init(HWND hWnd, ID3D11Device* dev, ID3D11DeviceContext* devCon)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(dev, devCon);
}

ImGuiInterface::~ImGuiInterface()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiInterface::Render()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	m_CameraControls.Render();
	m_ObjectControls.Render();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

CameraControls& ImGuiInterface::CameraState()
{
	return m_CameraControls;
}

ObjectControls& ImGuiInterface::ObjectState()
{
	return m_ObjectControls;
}

} // namespace UI
} // namespace Engine