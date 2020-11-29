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
#include "PagodaScene.h"
#include "PerspectiveMaterial.h"
#include "Scene.h"
#include "Utils.h"

// Direct 3D Library files
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

static Engine::UI::ImGuiInterface ui;

static Engine::GraphicsDevice* sp_graphicsDevice;
static Engine::GPUBuffer cameraConstantBuffer;

static Engine::Scene* pagodaScene;
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

	RECT wr = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };
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
		NULL
	);

	ShowWindow(hWnd, nCmdShow);

	InitD3D(hWnd);

	// Main Loop
	MSG msg;

	while (TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
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
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void InitUI(HWND hWnd)
{
	ui.Init(hWnd, sp_graphicsDevice->pDevice, sp_graphicsDevice->pImmediateContext);
	auto& cameraControlsState = ui.CameraState();
	auto& objectControlsState = ui.ObjectState();
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

	InitUI(hWnd);
	InitPipeline();
}


void CleanD3D()
{
	delete sp_graphicsDevice;
	delete pagodaScene;
}

struct CameraConstantBuffer
{
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT4 cameraPos;
};

void Update()
{
	auto objectControlsState = ui.ObjectState();

	Engine::Entity* mainPagoda = pagodaScene->GetEntity("Pagoda");
	mainPagoda->SetPosition(DirectX::XMFLOAT3(objectControlsState.x, objectControlsState.y, objectControlsState.z));
	mainPagoda->SetRotation(DirectX::XMFLOAT3(objectControlsState.rotx, objectControlsState.roty, objectControlsState.rotz));

	for (auto& entity : pagodaScene->GetEntities())
	{
		entity->Update();
	}
}


void RenderFrame(void)
{

	// Clear render targets
	Engine::ClearDepth(*sp_graphicsDevice, sp_graphicsDevice->pDepthStencilView);
	Engine::ClearRenderTarget(*sp_graphicsDevice, sp_graphicsDevice->backbufferRTV, ui.ObjectState().backgroundColor);

	// Update constant buffer
	auto cameraControlsState = ui.CameraState();
	Engine::MappedGPUBuffer mappedCameraBuffer = Engine::MapConstantBuffer(*sp_graphicsDevice, cameraConstantBuffer);
	{

		CameraConstantBuffer* pBuffer = reinterpret_cast<CameraConstantBuffer*>(mappedCameraBuffer.data);
		pBuffer->view = DirectX::XMMatrixTranslation(
			0.0f,
			0.0f,
			-20.0f
		);

		pBuffer->view = DirectX::XMMatrixInverse(nullptr, pBuffer->view);
		pBuffer->projection = DirectX::XMMatrixPerspectiveFovLH(
			cameraControlsState.fov,
			cameraControlsState.aspectRatio,
			cameraControlsState.nearZ,
			cameraControlsState.farZ);

		pBuffer->cameraPos = DirectX::XMFLOAT4(0.0f, 0.0f, -20.0f, 1.0f);
	}
	Engine::UnmapConstantBuffer(*sp_graphicsDevice, mappedCameraBuffer);

	Engine::BindConstantBuffer(*sp_graphicsDevice, cameraConstantBuffer, 0);

	// Render entities
	auto entities = pagodaScene->GetEntities();
	for (auto& entity : entities)
	{
		Engine::GPUBuffer& entityGPUBuffer = entity->GetMaterial()->GetGPUBuffer();
		Engine::MappedGPUBuffer entityConstantBuffer = Engine::MapConstantBuffer(*sp_graphicsDevice, entityGPUBuffer);
		{
			Engine::PerspectiveConstantBuffer* pBuffer = reinterpret_cast<Engine::PerspectiveConstantBuffer*>(entityConstantBuffer.data);
			pBuffer->worldTransform = entity->GetTransform();
		}
		Engine::UnmapConstantBuffer(*sp_graphicsDevice, entityConstantBuffer);

		Engine::BindConstantBuffer(*sp_graphicsDevice, entityGPUBuffer, 1);
		entity->Bind(*sp_graphicsDevice);
		sp_graphicsDevice->pImmediateContext->Draw(entity->GetMesh()->NumberVertices(), 0);
	}

	// Render UI last to draw on top of scene
	ui.Render();

	// Swap buffers
	sp_graphicsDevice->pSwapchain->Present(0, 0);
}

void InitPipeline(void)
{
	pagodaScene = new Game::PagodaScene();
	pagodaScene->Load(*sp_graphicsDevice);
	 
	//perspectiveConstantBuffer = Engine::CreateConstantBuffer(*sp_graphicsDevice, sizeof(Engine::PerspectiveConstantBuffer));
	cameraConstantBuffer = Engine::CreateConstantBuffer(*sp_graphicsDevice, sizeof(CameraConstantBuffer), nullptr);
}
