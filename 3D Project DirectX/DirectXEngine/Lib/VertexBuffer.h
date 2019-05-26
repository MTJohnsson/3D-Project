#ifndef VertexBuffer_H
#define VertexBuffer_H

#include <d3d11.h>
#include <memory>
#include <wrl/client.h>
using namespace DirectX;
using Microsoft::WRL::ComPtr;

template<class T>
class VertexBuffer
{
private:
	ComPtr < ID3D11Buffer> buffer;
	UINT stride = sizeof(T);
	UINT vertexCount = 0;

public:
	VertexBuffer() {}
	~VertexBuffer() {
		/*buffer->Release();*/
		stride = NULL;
	}
	ID3D11Buffer* Get()const
	{
		return buffer;
	}

	ID3D11Buffer* const* GetAddressOf()const
	{
		return buffer.GetAddressOf();
	}

	UINT VertexCount() const
	{
		return this->vertexCount;
	}

	const UINT *Stride() const
	{
		return &this->stride;
	}
	HRESULT Initialize(ID3D11Device *device, T * data, UINT vertexCount)
	{
		if (buffer != nullptr)
			buffer = nullptr;

		this->vertexCount = vertexCount;

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = stride * vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = data;
HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, buffer.GetAddressOf());
		return hr;
	}
};

#endif // !1
