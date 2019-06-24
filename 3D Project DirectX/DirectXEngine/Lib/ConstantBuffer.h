#pragma once
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <wrl/client.h>
using namespace DirectX;
using Microsoft::WRL::ComPtr;//screen aspect

template<typename  T>
class ConstantBuffer
{
private:
	ComPtr < ID3D11Buffer> constantBuffer = nullptr;
public:
	T data;
	bool InitializeMultiple(ID3D11Device * device, int NUM_LIGHTS) {


		D3D11_BUFFER_DESC lbDesc;
		lbDesc.ByteWidth = sizeof(T) * NUM_LIGHTS;
		lbDesc.Usage = D3D11_USAGE_DYNAMIC;
		lbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lbDesc.MiscFlags = 0;
		lbDesc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&lbDesc, nullptr, this->constantBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			MessageBox(NULL, "CreateBuffer Failed.",
				"Create buffer Error", MB_OK);
			return false;
		}
		return true;
	}
	bool Initialize(ID3D11Device * device)
	{
		if (constantBuffer.Get() != nullptr)
			constantBuffer = nullptr;
		D3D11_BUFFER_DESC constantBufferdesc;
		ZeroMemory(&constantBufferdesc, sizeof(constantBufferdesc));
		constantBufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferdesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));// sizeof(T);
		constantBufferdesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr;
		hr = device->CreateBuffer(&constantBufferdesc, 0, this->constantBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			MessageBox(NULL, "CreateBuffer Failed.",
				"Create buffer Error", MB_OK);
			return false;
		}
		return true;
	};
	bool updateConstantBufferMultiple(ID3D11DeviceContext * deviceContext)
	{
		D3D11_MAPPED_SUBRESOURCE mappedMemory;
		HRESULT hr = deviceContext->Map(constantBuffer.GetAddressOf(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, &data, sizeof(T));
		deviceContext->Unmap(constantBuffer.Get(), 0);
		if (FAILED(hr))
		{
			MessageBox(NULL, "mappedMemory Failed.",
				"mappedMemory Error", MB_OK);
			return false;
		}
		return true;
	}
	bool updateConstantBuffer(ID3D11DeviceContext * deviceContext)
	{
		D3D11_MAPPED_SUBRESOURCE mappedMemory;
		HRESULT hr = deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, &data, sizeof(T));
		deviceContext->Unmap(constantBuffer.Get(), 0);
		if (FAILED(hr))
		{
			MessageBox(NULL, "mappedMemory Failed.",
				"mappedMemory Error", MB_OK);
			return false;
		}
		return true;
	}
	ID3D11Buffer*const* getConstantBuffer()const { return this->constantBuffer.GetAddressOf(); }
	//ID3D11Bufferconst &get()const { return this->constantBuffer.Get(); }
	bool release() {
		if (constantBuffer.Get() != nullptr) {
			constantBuffer = nullptr;
		}
		return true;
	}
};


