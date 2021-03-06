#include <windows.h>
#include <windowsx.h>

#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "Entity.h"
#include "GraphicsDevice.h"
#include "ImGuiInterface.h"
#include "Loaders.h"
#include "Material.h"
#include "Mesh.h"
#include "RenderNormalsMaterial.h"
#include "RenderPass.h"
#include "RenderTexture.h"
#include "Scene.h"
#include "Skybox.h"
#include "ThreeTextureMaterial.h"
#include "UIRenderPass.h"
#include "Utils.h"

#include "PagodaScene.h"
#include "SphereScene.h"

// Direct 3D Library files
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

static Engine::UI::ImGuiInterface ui;

static Engine::GraphicsDevice *sp_graphicsDevice;
static Engine::GPUBuffer cameraConstantBuffer;

static Engine::RenderPass* forwardRenderPass;
static Engine::RenderPass* normalsPass;
static Engine::UIRenderPass* uiPass;

static Engine::RenderTexture* screenSpaceNormals;
static Engine::RenderNormalsMaterial* normalsMaterial;

static Engine::Scene *scene;
static DirectX::XMMATRIX perspectiveMatrix;

void InitD3D(HWND hWnd);
void CleanD3D(void);

void Update(void);
void RenderFrame(void);
void InitPipeline(void);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // No background by default
	wc.lpszClassName = L"WindowClass1";

	RegisterClassEx(&wc);

	RECT wr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(
		NULL,
		L"WindowClass1",
		L"First Windowed Program",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	InitD3D(hWnd);

	// Main Loop
	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}

		Update();
		RenderFrame();
	}

	CleanD3D();

	return msg.wParam;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void InitUI(HWND hWnd)
{
	ui.Init(hWnd, sp_graphicsDevice->pDevice, sp_graphicsDevice->Context());
	auto &cameraControlsState = ui.CameraState();
	auto &objectControlsState = ui.ObjectState();
	cameraControlsState.aspectRatio = (SCREEN_WIDTH * 1.0f) / SCREEN_HEIGHT;
	cameraControlsState.fov = 0.785398f;
	cameraControlsState.nearZ = 1.0f;
	cameraControlsState.farZ = 300.0f;

	// Nice and aesthetic default positioning :)
	objectControlsState.x = -6.878f;
	objectControlsState.y = -8.341;
	objectControlsState.z = 8.0;
	objectControlsState.roty = 2.421f;
}

void InitD3D(HWND hWnd)
{
	sp_graphicsDevice = new Engine::GraphicsDevice(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	std::vector<Engine::RenderTexture*> target { &sp_graphicsDevice->backbufferRenderTexture };
	forwardRenderPass = new Engine::RenderPass(nullptr, target);

	screenSpaceNormals = new Engine::RenderTexture(
		*sp_graphicsDevice,
		sp_graphicsDevice->backbufferRenderTexture.m_width,
		sp_graphicsDevice->backbufferRenderTexture.m_height
	);

	std::vector<Engine::RenderTexture*> normalsPassTargets { screenSpaceNormals };
	normalsMaterial = new Engine::RenderNormalsMaterial();
	normalsMaterial->Init(*sp_graphicsDevice);

	normalsPass = new Engine::RenderPass(normalsMaterial, normalsPassTargets);

	uiPass = new Engine::UIRenderPass(target);

	InitUI(hWnd);
	InitPipeline();
}

void CleanD3D()
{
	delete sp_graphicsDevice;
	delete scene;
}

struct CameraConstantBuffer
{
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT4 cameraPos;
	DirectX::XMFLOAT4 lightPos;
};

void Update()
{
	for (auto &entity : scene->GetEntities())
	{
		entity->Update(*sp_graphicsDevice);
	}
}

void RenderFrame(void)
{
	// Clear render targets
	Engine::ClearDepth(*sp_graphicsDevice, sp_graphicsDevice->pDepthStencilView);
	//Engine::ClearRenderTarget(*sp_graphicsDevice, sp_graphicsDevice->backbufferRenderTexture, ui.ObjectState().backgroundColor);
	forwardRenderPass->SetClearColour(ui.ObjectState().backgroundColor);


    scene->BindCameras(*sp_graphicsDevice);
    scene->BindLights(*sp_graphicsDevice);


	// Update constant buffer
	const auto &cameraControlsState = ui.CameraState();
	const auto &sceneControlsState = ui.SceneState();
	//Engine::MappedGPUBuffer mappedCameraBuffer = Engine::MapConstantBuffer(*sp_graphicsDevice, cameraConstantBuffer);
	//{

	//	CameraConstantBuffer *pBuffer = reinterpret_cast<CameraConstantBuffer *>(mappedCameraBuffer.data);
	//	pBuffer->view = DirectX::XMMatrixTranslation(
	//		0.0f,
	//		0.0f,
	//		-20.0f);

	//	pBuffer->view = DirectX::XMMatrixInverse(nullptr, pBuffer->view);
	//	pBuffer->projection = DirectX::XMMatrixPerspectiveFovLH(
	//		cameraControlsState.fov,
	//		cameraControlsState.aspectRatio,
	//		cameraControlsState.nearZ,
	//		cameraControlsState.farZ);

	//	pBuffer->cameraPos = DirectX::XMFLOAT4(0.0f, 0.0f, -20.0f, 1.0f);
	//	pBuffer->lightPos = DirectX::XMFLOAT4(
	//		sceneControlsState.lightx,
	//		sceneControlsState.lighty,
	//		sceneControlsState.lightz,
	//		1.0f);
	//}
	//Engine::UnmapConstantBuffer(*sp_graphicsDevice, mappedCameraBuffer);

	//Engine::BindConstantBuffer(*sp_graphicsDevice, cameraConstantBuffer, 0);

	std::vector<Engine::Entity*> sceneEntities = scene->GetEntities();

	normalsPass->Render(*sp_graphicsDevice, sceneEntities);

	forwardRenderPass->EnableClearOutput();
	forwardRenderPass->Render(*sp_graphicsDevice, sceneEntities);

	Engine::Camera* sceneCamera = scene->GetCamera();
	if (sceneCamera)
	{
		Engine::Skybox* skybox = sceneCamera->GetSkybox();
		if (skybox)
		{
			forwardRenderPass->DisableClearOutput();
			forwardRenderPass->Render(*sp_graphicsDevice, { skybox });
		}
	}

	uiPass->Render(*sp_graphicsDevice, sceneEntities);


	// Render UI last to draw on top of scene
	//ui.Render();

	// Swap buffers
	sp_graphicsDevice->pSwapchain->Present(0, 0);
}

void InitPipeline(void)
{
	//scene = new Game::PagodaScene(*sp_graphicsDevice);
	//scene->Load(*sp_graphicsDevice);

	//scene->GetEntity("Floor")->GetMaterial()->AddTexture(screenSpaceNormals);

	scene = new Game::SphereScene(*sp_graphicsDevice);
	scene->Load(*sp_graphicsDevice);

	//perspectiveConstantBuffer = Engine::CreateConstantBuffer(*sp_graphicsDevice, sizeof(Engine::PerspectiveConstantBuffer));
	cameraConstantBuffer = Engine::CreateConstantBuffer(*sp_graphicsDevice, sizeof(CameraConstantBuffer), nullptr);
}
