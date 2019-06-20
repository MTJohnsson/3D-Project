#ifndef SKYDOME_H
#define SKYDOME_H
#include"DirectXHelp.h"
#include"Lib/Shaders.h"
#include<WICTextureLoader.h>
#include<vector>
#include<string>
struct skyDomeVertex {
	XMFLOAT3 pos;
};
class SkyDome
{
public:
	SkyDome();
	~SkyDome();

	void InitializeSphere(ID3D11Device* device ,int latitude, int longitude);
	bool loadCubeMap(ID3D11Device* device, std::wstring fileName);
	void updateWorld(XMFLOAT3 camPos);
	void setupRenderStates(ID3D11Device* device);
	void drawSkydome(ID3D11DeviceContext* context);
	XMMATRIX getWorld() const { return this->sphereWorldMatrix; };
	Shader skyDomeShader;
private:
	ID3D11Buffer* sphereVerticies;
	ID3D11Buffer* sphereIndicies;

	ID3D11ShaderResourceView* sdrv;
	ID3D11Resource* srcTex[6];

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	int NrSphereVerticies, NrSphereFaces;

	XMMATRIX sphereWorldMatrix;
	XMMATRIX rotationX;
	XMMATRIX rotationY;
};

#endif // !SKYDOME_H

