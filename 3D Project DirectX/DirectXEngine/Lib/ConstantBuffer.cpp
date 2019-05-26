#include "ConstantBuffer.h"


bool ConstantBuffer::Initialize(ID3D11Device * device)
{
	gConstantBufferData = (CBData*)_aligned_malloc(sizeof(gConstantBufferData), 16);
	gConstantBufferData->colour[0] = 1.0f;
	gConstantBufferData->colour[1] = 0.0f;
	gConstantBufferData->colour[2] = 0.0f;
	gConstantBufferData->colour[3] = 1.0f;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(CBData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA pData;
	ZeroMemory(&pData, sizeof(pData));
	pData.pSysMem = gConstantBufferData;
	pData.SysMemPitch = 0;
	pData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = device->CreateBuffer(&desc, &pData, &this->constantBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateBuffer Failed.",
			"Create buffer Error", MB_OK);
		return false;
	}
	return true;
}

bool ConstantBuffer::updateConstantBuffer(ID3D11DeviceContext * deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	HRESULT hr = deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gConstantBufferData, sizeof(CBData));
	deviceContext->Unmap(constantBuffer, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, "mappedMemory Failed.",
			"mappedMemory Error", MB_OK);
		return false;
	}

	return true;
}

