#ifndef SKYBOX_H
#define SKYBOX_H
#include"DirectXHelp.h"
#include"TGATexture.h"
#include"Lib/VertexBuffer.h"
#include"Lib/ConstantBuffer.h"
#define SKYBOX_WIDTH 1.0f
#define SKYBOX_HEIGHT 1.0f

struct SkyboxMatricies {
	XMMATRIX view;
	XMMATRIX projection;
	XMFLOAT3 camPos;
	float padding;	//4 nu 16
};
struct SkyboxWorldMatrix {
	XMMATRIX world;
};

class Skybox
{
public:
	Skybox();
	~Skybox();
	void shutDown();
	ID3D11ShaderResourceView* getTextureCube() const { return this->cubeSRV; };
	bool initialize(ID3D11Device* device);
	bool setTexture(ID3D11Device* device, ID3D11DeviceContext* context, std::string file);
	bool renderSkybox(ID3D11DeviceContext* context);
	bool setWorldMatrix(XMMATRIX world, ID3D11DeviceContext* context);
	bool setViewProjMatrix(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, ID3D11DeviceContext* context);
	void setCB(ID3D11DeviceContext* context);
private:
	struct Vertex
	{
		float x, y, z, w;//16 
	};
	VertexBuffer<Vertex> vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ConstantBuffer<SkyboxMatricies> matrixConstBuffer;
	ConstantBuffer<SkyboxWorldMatrix> worldConstBuffer;
	ID3D11PixelShader *skyboxPS;
	ID3D11VertexShader *skyboxVS;
	ID3D11InputLayout *vertexLayout;
	ID3D11SamplerState* samplerState;
	ID3D11ShaderResourceView* cubeSRV;
	ID3D11Texture2D* cubeTexture;
	TGATexture tgaTextures[6];
	std::vector<DWORD> indices;
	std::vector<Vertex> vertices;

	bool initializeShaders(ID3D11Device* device);

};

#endif // !SKYBOX_H
