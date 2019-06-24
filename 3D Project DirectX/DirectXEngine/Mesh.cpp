#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <iomanip>  
#include <sstream>

bool Mesh::InitializeObject(ID3D11Device *device, ID3D11DeviceContext *deviceContext, string file, Shader *shader, Shader *shader2)
{
	this->device = device;
	this->deviceContext = deviceContext;
	Initialize(device, deviceContext, file);
	this->IndexCount = this->vertices.size();

	for (int i = 0; i < textures.size(); i++)
	{
		if (DIFFUSE == textures[i].getType())
		{
			deviceContext->PSSetShaderResources(0, 1, textures[i].getTextureView());
		}
		if (NORMAL == textures[i].getType())
		{
			deviceContext->PSSetShaderResources(1, 1, textures[i].getTextureView());
		}
	}

	shaderPtr = shader;
	if (textures.size() == 0)
	{
		shaderPtr = shader2;
	}

	return true;
}

void Mesh::draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;


	deviceContext->VSSetShader(shaderPtr->GetVertexShader()->GetShader(), NULL, 0);
	deviceContext->GSSetShader(shaderPtr->GetGeometryShader()->GetShader(), nullptr, 0);
	deviceContext->PSSetShader(shaderPtr->GetPixelShader()->GetShader(), NULL, 0);

	for (int i = 0; i < textures.size(); i++)
	{
		if (DIFFUSE == textures[i].getType())
		{
			deviceContext->PSSetShaderResources(0, 1, textures[i].getTextureView());
		}
		if (NORMAL == textures[i].getType())
		{
			deviceContext->PSSetShaderResources(1, 1, textures[i].getTextureView());
		}
	}

	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.Stride(), &offset);
	deviceContext->Draw(vertices.size(), 0);


	//deviceContext->PSSetShaderResources(1, 1,nullptr);
	//deviceContext->PSSetShaderResources(0, 1, nullptr);
}

Mesh::~Mesh()
{
	//Possible memory leak
	this->shaderPtr = nullptr;
	delete this->shaderPtr;
}