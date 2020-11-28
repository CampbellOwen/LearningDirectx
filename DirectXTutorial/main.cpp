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
static Engine::GPUBuffer perspectiveConstantBuffer;

static Engine::Scene* pagodaScene;
static DirectX::XMMATRIX perspectiveMatrix;

void InitD3D(HWND hWnd);
void CleanD3D(void);

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
	cameraControlsState.farZ = 50.0;

	// Nice and aesthetic default positioning :)
	objectControlsState.x = -6.878f;
	objectControlsState.y = -8.341;
	objectControlsState.z = 8.0;
	objectControlsState.roty = 0.889f;
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

void RenderFrame(void)
{
	auto cameraControlsState = ui.CameraState();
	auto objectControlsState = ui.ObjectState();

	// Clear render targets
	Engine::ClearDepth(*sp_graphicsDevice, sp_graphicsDevice->pDepthStencilView);
	Engine::ClearRenderTarget(*sp_graphicsDevice, sp_graphicsDevice->backbufferRTV, objectControlsState.backgroundColor);

	// Update constant buffer
	Engine::MappedGPUBuffer mappedBuffer = Engine::MapConstantBuffer(*sp_graphicsDevice, &perspectiveConstantBuffer);
	{
		Engine::PerspectiveConstantBuffer* pBuffer = reinterpret_cast<Engine::PerspectiveConstantBuffer*>(mappedBuffer.data);

		pBuffer->worldTransform = DirectX::XMMatrixRotationRollPitchYaw(
			objectControlsState.rotx, 
			objectControlsState.roty, 
			objectControlsState.rotz);
		pBuffer->worldTransform *= DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);
		pBuffer->worldTransform *= DirectX::XMMatrixTranslation(
			objectControlsState.x,
			objectControlsState.y,
			objectControlsState.z);

		pBuffer->cameraTransform = DirectX::XMMatrixTranslation(
			0.0f,
			0.0f,
			-20.0f
		);

		pBuffer->cameraTransform = DirectX::XMMatrixInverse(nullptr, pBuffer->cameraTransform);
		pBuffer->cameraTransform *= DirectX::XMMatrixPerspectiveFovLH(
			cameraControlsState.fov, 
			cameraControlsState.aspectRatio, 
			cameraControlsState.nearZ, 
			cameraControlsState.farZ);
	}
	Engine::UnmapConstantBuffer(*sp_graphicsDevice, mappedBuffer);

	Engine::BindConstantBuffer(*sp_graphicsDevice, perspectiveConstantBuffer, 0);

	// Render entities
	auto entities = pagodaScene->GetEntities();
	for (auto& entity : entities)
	{
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
	 
	perspectiveConstantBuffer = Engine::CreateConstantBuffer(*sp_graphicsDevice, sizeof(Engine::PerspectiveConstantBuffer));
}
