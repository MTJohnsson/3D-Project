#ifndef Skybox_H
#define Skybox_H

#include"TGATexture.h"
#include <d3d11.h>
#include <directxmath.h>
#include<vector>
#include "Lib/ConstantBuffer.h"
#include <d3dcompiler.h>
#define SKYBOX_SIZE 1.0f

class Skybox
{
public:
	Skybox();
	~Skybox();
	void shutDown();
	bool initialize();
	bool setTexture(std::string file);
	bool render();
	bool setWorld(DirectX::XMMATRIX world);
	bool setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
	void setCB();
private:
	struct Vertex
	{
		float x, y, z, w;//16 
	};
	struct Matrices
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ConstantBuffer<Matrices> perFrameCB;
	ConstantBuffer<WorldMatrix> worldCB;
	ID3D11PixelShader *ps;
	ID3D11VertexShader *vs;
	ID3D11InputLayout *vertexLayout;
	ID3D11SamplerState* samplerState;
	ID3D11ShaderResourceView* cubeSRV;
	ID3D11Texture2D* cubeTex;
	TextureLoad textureLoad[6];
	std::vector<DWORD> indices;
	std::vector<Vertex> vertices;

	bool initializeShaders();
	bool initializeVertex();

};
#endif // !Skybox_H