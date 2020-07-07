#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <wincodec.h>
#pragma comment (lib, "windowscodecs.lib")

#include "Loaders.h"
#include "Utils.h"

#include <iostream>
#include <memory>
#include <vector>

namespace Engine
{
namespace Loaders
{
	std::vector<Engine::VERTEX> LoadObj(const std::string& filename)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		std::string warn;
			
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());
		if (!warn.empty()) {
			MessageBoxA(nullptr, warn.c_str(), "tinyobj warning", MB_OK);
		}
		if (!err.empty()) {
			MessageBoxA(nullptr, err.c_str(), "tinyobj error", MB_OK);
		}
		if (!ret) {
			return {};
		}

		std::vector<Engine::VERTEX> vertices;

		for (auto shape : shapes)
		{
			size_t index_offset = 0;
			for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) 
			{
				int numFaceVertices = shape.mesh.num_face_vertices[f];
				for (size_t vIndex = 0; vIndex < numFaceVertices; vIndex++) 
				{
					tinyobj::index_t idx = shape.mesh.indices[index_offset + vIndex];
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];

					vertices.push_back({
						{vx, vy, vz},
						{nx, ny, nz},
						{tx, ty},
						{0.5, 0.5, 0.5, 1.0}
						});

				}

				index_offset += numFaceVertices;
			}
		}

		return vertices;
	}

	HRESULT LoadImage(
		ID3D11Device* device, 
		ID3D11DeviceContext* deviceContext, 
		LPCWSTR filename, 
		ID3D11Resource** texture)
	{
		IWICBitmapDecoder* pDecoder = nullptr;
		IWICBitmapFrameDecode* pFrame = nullptr;
		IWICBitmap* pBitmap = nullptr;
		IWICBitmapSource *pConvertedFrame = NULL;
		IWICBitmapLock* pLock = nullptr;
		HRESULT hr = S_OK;
		UINT width, height;
		WICRect rcLock = { 0,0,0,0 };

		IWICImagingFactory* pWICImagingFactory = nullptr;
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pWICImagingFactory);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create imaging factory", MB_OK);
			goto cleanup;
		}

		hr = pWICImagingFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create Decoder from filename", MB_OK);
			goto cleanup;
		}

		hr = pDecoder->GetFrame(0, &pFrame);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "GetFrame", MB_OK);
			goto cleanup;
		}
		hr = WICConvertBitmapSource(GUID_WICPixelFormat32bppRGBA, pFrame, &pConvertedFrame);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Convert Bitmap", MB_OK);
			goto cleanup;
		}

		pConvertedFrame->GetSize(&width,&height);
		rcLock.Width = width;
		rcLock.Height = height;

		{
			auto rawBuffer = std::unique_ptr<BYTE[]>(new BYTE[width*height*4]);

			hr = pConvertedFrame->CopyPixels(&rcLock, 4 * width, width * height * 4, rawBuffer.get());

			if (FAILED(hr)) {
				MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Copy Pixels", MB_OK);
				goto cleanup;
			}

			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
			initData.pSysMem = rawBuffer.get();
			initData.SysMemPitch = (width * 4);
			initData.SysMemSlicePitch = (width * height);

			ID3D11Texture2D* tex = nullptr;
			hr = device->CreateTexture2D(&desc, &initData, &tex);
			if (FAILED(hr)) {
				MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create Texure", MB_OK);
				goto cleanup;
			}

			*texture = tex;
		}

cleanup:
		if (pLock) pLock->Release();
		if (pWICImagingFactory) pWICImagingFactory->Release();
		if (pDecoder) pDecoder->Release();
		if (pFrame) pFrame->Release();
		if (pBitmap) pBitmap->Release();
		if (pConvertedFrame) pConvertedFrame->Release();

		return hr;
	}
}
}

