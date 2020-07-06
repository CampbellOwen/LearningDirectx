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

	std::vector<PixelData> LoadImage(LPCWSTR filename)
	{
		std::vector<PixelData> pixelData;


		IWICBitmapDecoder* pDecoder = nullptr;
		IWICBitmapFrameDecode* pFrame = nullptr;
		IWICBitmap* pBitmap = nullptr;
		IWICBitmapSource *pConvertedFrame = NULL;
		IWICBitmapLock* pLock = nullptr;
		HRESULT hr;
		UINT width, height;
		WICRect rcLock = { 0,0,0,0 };

		IWICImagingFactory* pWICImagingFactory = nullptr;
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pWICImagingFactory);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetLastErrorAsString(hr).c_str(), "Create imaging factory", MB_OK);
			goto cleanup;
		}

		hr = pWICImagingFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetLastErrorAsString(hr).c_str(), "Create Decoder from filename", MB_OK);
			goto cleanup;
		}

		hr = pDecoder->GetFrame(0, &pFrame);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetLastErrorAsString(hr).c_str(), "GetFrame", MB_OK);
			goto cleanup;
		}
		hr = WICConvertBitmapSource(GUID_WICPixelFormat32bppRGBA, pFrame, &pConvertedFrame);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetLastErrorAsString(hr).c_str(), "Convert Bitmap", MB_OK);
			goto cleanup;
		}

		hr = pWICImagingFactory->CreateBitmapFromSource(pConvertedFrame, WICBitmapCacheOnDemand, &pBitmap);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetLastErrorAsString(hr).c_str(), "Create Bitmap", MB_OK);
			goto cleanup;
		}

		pFrame->GetSize(&width,&height);
		rcLock.Width = width;
		rcLock.Height = height;
		hr = pBitmap->Lock(&rcLock, WICBitmapLockWrite, &pLock);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetLastErrorAsString(hr).c_str(), "Lock Bitmap", MB_OK);
			goto cleanup;
		}

		{
			UINT cbBufferSize = 0;
			BYTE* pv = nullptr;
			hr = pLock->GetDataPointer(&cbBufferSize, &pv);
			if (FAILED(hr)) {
				MessageBoxA(nullptr, Engine::Utils::GetLastErrorAsString(hr).c_str(), "Get Image Data Pointer", MB_OK);
				goto cleanup;
			}

			PixelData* pixels = reinterpret_cast<PixelData*>(pv);
			for (int i = 0; i < cbBufferSize / 4; i ++ ) {
				pixelData.push_back(pixels[i]);
			}
		}


cleanup:

		if (pLock) pLock->Release();
		if (pWICImagingFactory) pWICImagingFactory->Release();
		if (pDecoder) pDecoder->Release();
		if (pFrame) pFrame->Release();
		if (pBitmap) pBitmap->Release();
		if (pConvertedFrame) pConvertedFrame->Release();

		return pixelData;
	}
}
}

