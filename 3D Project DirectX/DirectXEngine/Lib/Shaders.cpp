#include "Shaders.h"


bool VertexShader::CreatVertexShader(ID3D11Device* device, std::wstring fileName, D3D11_INPUT_ELEMENT_DESC * layout, UINT elements)
{
	HRESULT hr = D3DReadFileToBlob(fileName.c_str(), this->buffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateVertexShader(this->buffer->GetBufferPointer(), this->buffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
	
		return false;
	}

	hr = device->CreateInputLayout(layout, elements, this->buffer->GetBufferPointer(), this->buffer->GetBufferSize(), this->inputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool PixelShader::CreatPixelShader(ID3D11Device * device, std::wstring fileName)
{
	if (this->shader != nullptr || this->buffer != nullptr)
	{
		this->shader = nullptr;
		this->buffer = nullptr;
	}
	HRESULT hr = D3DReadFileToBlob(fileName.c_str(), this->buffer.GetAddressOf());
	if (FAILED(hr))
	{
		//std::wstring f = L"PixelShaderDefault.hlsl";
		//hr = D3DCompileFromFile(f.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0_level_9_3", D3D10_SHADER_ENABLE_STRICTNESS, 0, this->buffer.GetAddressOf(), NULL);
		//
		return false;
	}
	hr = device->CreatePixelShader(this->buffer.Get()->GetBufferPointer(), this->buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

Shader::Shader()
{
}

bool Shader::CreatGeometryShader(ID3D11Device * device, std::wstring fileName)
{
	if (!this->geometryShader.CreatGeometryShader(device, fileName))
		return false;
	return true;
}

bool Shader::CreatVertexShader(ID3D11Device * device, std::wstring fileName, D3D11_INPUT_ELEMENT_DESC * layout, UINT elements)
{
	if (!this->vertexShader.CreatVertexShader(device, fileName, layout, elements))
		return false;
	return true;
}

bool Shader::CreatPixelShader(ID3D11Device * device, std::wstring fileName)
{
	if (!this->pixdelShader.CreatPixelShader(device, fileName))
		return false;
	return true;
}

Shader::Shader(const Shader & other)
{
	this->vertexShader = other.vertexShader;
	this->geometryShader = other.geometryShader;
	this->pixdelShader = other.pixdelShader;
}

Shader Shader::operator=(const Shader & other)
{
	if (this != &other)
	{
		this->vertexShader = other.vertexShader;
		this->geometryShader = other.geometryShader;
		this->pixdelShader = other.pixdelShader;
	}
	return *this;
}

bool GeometryShader::CreatGeometryShader(ID3D11Device * device, std::wstring fileName)
{
	HRESULT hr = D3DReadFileToBlob(fileName.c_str(), this->buffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	hr = device->CreateGeometryShader(this->buffer.Get()->GetBufferPointer(), this->buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}