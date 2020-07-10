#include <windows.h>
#include <windowsx.h>

#include <iostream>
#include <vector>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Loaders.h"
#include "Material.h"
#include "PerspectiveMaterial.h"
#include "Mesh.h"
#include "Utils.h"

// Direct 3D Library files
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define CHECKHR(hr) ()


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768


static IDXGISwapChain* swapchain;
static ID3D11Device* dev;
static ID3D11DeviceContext* devCon;
static ID3D11RenderTargetView* backbuffer;
static ID3D11DepthStencilView* pDepthStencilView;
static ID3D11Texture2D* depthStencilBuffer;
static ID3D11DepthStencilState* pDepthStencilState;

static ID3D11VertexShader* pVS;
static ID3D11PixelShader* pPS;
static ID3D11Buffer* pVBuffer;
static ID3D11InputLayout* pLayout;

static Engine::Mesh triangle;
//static Engine::Material basicMaterial;
static Engine::PerspectiveMaterial perspectiveMaterial;

static DirectX::XMMATRIX perspectiveMatrix;

void InitD3D(HWND hWnd);
void CleanD3D(void);

void RenderFrame(void);
void InitPipeline(void);
void InitGraphics(void);

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

void InitDearImGUI(HWND hWnd)
{
	// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(dev, devCon);
}

void InitD3D(HWND hWnd)
{
	// Initialize Direct3D
	
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(
		NULL, 
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL, 
		D3D11_CREATE_DEVICE_DEBUG, 
		NULL, 
		NULL, 
		D3D11_SDK_VERSION, 
		&scd, 
		&swapchain, 
		&dev, 
		NULL, 
		&devCon
	);

	// Setup Render Target
	ID3D11Texture2D* pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// Create render target with backbuffer address
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// Setup Depth Buffer

	ID3D11Texture2D* pDepthStencil = nullptr;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = SCREEN_WIDTH;
	descDepth.Height = SCREEN_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDepth.SampleDesc.Count = 4;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = dev->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create Depth Texture", MB_OK);
	}

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	// Depth test paraemeters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	
	// Stencil test paremeters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ID3D11DepthStencilState* pDSState;
	dev->CreateDepthStencilState(&dsDesc, &pDSState);

	// Bind depth stencil state
	devCon->OMSetDepthStencilState(pDSState, 1);
	pDSState->Release();

	// Bind depth-stencil resource using a view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	hr = dev->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create Depth Stencil View", MB_OK);
	}

	pDepthStencil->Release();

	devCon->OMSetRenderTargets(1, &backbuffer, pDepthStencilView);


	// Setup the Viewport

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	devCon->RSSetViewports(1, &viewport);

	InitDearImGUI(hWnd);
	InitPipeline();
	InitGraphics();
}

void CleanD3D()
{
	 ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

	swapchain->SetFullscreenState(FALSE, NULL);

	swapchain->Release();
	backbuffer->Release();
	pDepthStencilView->Release();
	dev->Release();
	devCon->Release();
}

void RenderFrame(void)
{
	// Clear back buffer
	static float cornflowerBlue[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		static float x = 0.0f;
		static float y = 0.0f;
		static float z = 0.0f;

		Engine::PerspectiveConstantBuffer cBuffer;
	cBuffer.worldTransform = DirectX::XMMatrixRotationY(0.785398f);
	cBuffer.worldTransform *= DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);
	cBuffer.worldTransform *= DirectX::XMMatrixTranslation(x,y,z);
	//cBuffer.cameraTransform = DirectX::XMMatrixPerspectiveLH(2.0f, 1.0f, 0.1f, 50.0f);
	cBuffer.cameraTransform = DirectX::XMMatrixPerspectiveFovLH(0.785398f, 1.7f, 0.1f, 100.0f);
		perspectiveMaterial.UpdateConstantBuffer(devCon, cBuffer);

	devCon->ClearRenderTargetView(backbuffer, cornflowerBlue);
	devCon->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Render 3D

	perspectiveMaterial.Activate(devCon);
	triangle.Render(devCon);
	devCon->Draw(triangle.NumberVertices(), 0);

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	{

		ImGui::Begin("Object Controls");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("Position");

		ImGui::SliderFloat("X", &x, -30.0f, 30.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("Y", &y, -30.0f, 30.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("Z", &z, -30.0f, 30.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

		ImGui::Text("Clear Colour");
		ImGui::ColorEdit3("clear color", (float*)&cornflowerBlue); // Edit 3 floats representing a color

		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Swap buffers
	swapchain->Present(0, 0);
}

void InitPipeline(void)
{
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const Engine::ShaderInfo vertexShader{L"shaders.hlsl", "VShader"};
	const Engine::ShaderInfo pixelShader{ L"shaders.hlsl", "PShader" };
	//basicMaterial.Init(dev, vertexShader, pixelShader, ied, 4);
	perspectiveMaterial.Init(dev, vertexShader, pixelShader, ied, 4);

	ID3D11Resource* texture;
	HRESULT hr = Engine::Loaders::LoadImageW(dev, devCon, L"F:\\Models\\JapaneseTemple\\Textures\\albedo.png", &texture);
	perspectiveMaterial.AddTexture(dev, texture);
	
	Engine::PerspectiveConstantBuffer cBuffer;
	cBuffer.worldTransform = DirectX::XMMatrixRotationY(0.785398f);
	cBuffer.worldTransform *= DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);
	cBuffer.worldTransform *= DirectX::XMMatrixTranslation(0, -5, 35);
	//cBuffer.cameraTransform = DirectX::XMMatrixPerspectiveLH(2.0f, 1.0f, 0.1f, 50.0f);
	cBuffer.cameraTransform = DirectX::XMMatrixPerspectiveFovLH(0.785398f, 1.7f, 0.1f, 100.0f);
	perspectiveMaterial.UpdateConstantBuffer(devCon, cBuffer);
}

void InitGraphics(void)
{
	const std::vector<Engine::VERTEX> temple = Engine::Loaders::LoadObj("F:\\Models\\JapaneseTemple\\model.obj");
	triangle.Init(dev, devCon, temple);
}