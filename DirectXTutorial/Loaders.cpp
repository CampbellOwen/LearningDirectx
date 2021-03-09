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
	std::pair<std::vector<Engine::VERTEX>, std::vector<uint32_t>> LoadObj(const std::string& filename)
	{
		std::string err;
		std::string warn;

		/*
		    tinyobj::ObjReader reader;
    reader.ParseFromFile(nvh::findFile("scenes/CornellBox-Original-Merged.obj", searchPaths));
    assert(reader.Valid());

    const std::vector<tinyobj::real_t> objVertices = reader.GetAttrib().GetVertices();
    const std::vector<tinyobj::shape_t>& objShapes = reader.GetShapes();
    assert(objShapes.size() == 1);
    const tinyobj::shape_t& objShape = objShapes[0];

    std::vector<uint32_t> objIndices;
    objIndices.reserve(objShape.mesh.indices.size());
    for (const tinyobj::index_t& index : objShape.mesh.indices)
    {
        objIndices.push_back(index.vertex_index);
    }

		*/

		//tinyobj::ObjReader reader;
		//reader.ParseFromFile(filename);
		//assert(reader.Valid());
		//const std::vector<tinyobj::real_t> objVertices = reader.GetAttrib().GetVertices();
		//const auto objNormals = reader.GetAttrib().normals;
		//const auto objUV = reader.GetAttrib().texcoords;
		//assert(objVertices.size() == objNormals.size() && objNormals.size() == objUV.size());

		//const auto shapes = reader.GetShapes();
		//assert(shapes.size() > 1);
		//const auto objIndices = shapes[0].mesh.indices;

		//assert(shapes[0].mesh.num_face_vertices[0] == 3);

		//std::vector<Engine::VERTEX> vertices;
		//for (int i = 0, uv = 0; i < objVertices.size(), uv < objUV.size(); i += 3, uv += 2)
		//{
		//	vertices.emplace_back(
		//		DirectX::XMFLOAT3(objVertices[i], objVertices[i+1], objVertices[i+2]),
		//		DirectX::XMFLOAT3(objNormals[i], objNormals[i+1], objNormals[i+2]),
		//		DirectX::XMFLOAT2(objUV[uv], objUV[uv+1]),
		//		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)
		//	);
		//}

		//std::vector<uint32_t> indices;
		//for (const auto& index : objIndices)
		//{
		//	indices.push_back(static_cast<uint32_t>(index));
		//}
			
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());
		if (!warn.empty()) {
			//MessageBoxA(nullptr, warn.c_str(), "tinyobj warning", MB_OK);
		}
		if (!err.empty()) {
			MessageBoxA(nullptr, err.c_str(), "tinyobj error", MB_OK);
		}
		if (!ret) {
			return {};
		}

		std::vector<uint32_t> indices;
		std::vector<VERTEX> vertices;

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

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attrib.texcoords.size() > 0)
					{
						tx = attrib.texcoords[2 * idx.texcoord_index + 0];
						ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					}

					vertices.push_back({
						{vx, vy, vz},
						{nx, ny, nz},
						{tx, ty},
						{0.5, 0.5, 0.5, 1.0}
						});

					indices.push_back(index_offset + vIndex);

				}

				index_offset += numFaceVertices;
			}
		}

		return std::make_pair(vertices, indices);
	}

	HRESULT LoadImage( LPCWSTR filename, DiskImage* imageOut)
	{
		IWICBitmapDecoder* pDecoder = nullptr;
		IWICBitmapFrameDecode* pFrame = nullptr;
		IWICBitmap* pBitmap = nullptr;
		IWICBitmapSource *pConvertedFrame = nullptr;
		IWICBitmapLock* pLock = nullptr;
		IWICBitmapFlipRotator* pBitmapFlipRotator = nullptr;
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

		// PNG are top down, flip image so 0,0 is bottom left
		hr = pWICImagingFactory->CreateBitmapFlipRotator(&pBitmapFlipRotator);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Create bitmap flip rotator", MB_OK);
			goto cleanup;
		}

		hr = pBitmapFlipRotator->Initialize(pConvertedFrame, WICBitmapTransformFlipVertical);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Flip pixels", MB_OK);
			goto cleanup;
		}

		pBitmapFlipRotator->GetSize(&width,&height);
		rcLock.Width = width;
		rcLock.Height = height;

		{
			auto rawBuffer = std::unique_ptr<BYTE[]>(new BYTE[width*height*4]);

			hr = pBitmapFlipRotator->CopyPixels(&rcLock, 4 * width, width * height * 4, rawBuffer.get());


			if (FAILED(hr)) {
				MessageBoxA(nullptr, Engine::Utils::GetHRErrorString(hr).c_str(), "Copy Pixels", MB_OK);
				goto cleanup;
			}

			imageOut->height = height;
			imageOut->width = width;
			imageOut->spBuffer = std::move(rawBuffer);

		}

cleanup:
		if (pBitmapFlipRotator) pBitmapFlipRotator->Release();
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

