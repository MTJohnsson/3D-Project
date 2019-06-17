#ifndef SKYBOX_H
#define SKYBOX_H
#include"DirectXHelp.h"
#include"Lib/Texture.h"
#define SKYBOX_WIDTH 1.0f
#define SKYBOX_HEIGHT 1.0f


class Skybox
{
public:
	Skybox();
	~Skybox();
	void DestroySkybox();
	bool InitializeSkybox(ID3D11Device* device);
	bool loadSkyboxTexture(ID3D11Device* device, std::string filePath);
	bool renderSkybox(ID3D11DeviceContext* context);
private:
	struct SkyboxVertex {
		float x, y, z;
		float padding;
	};
	ID3D11Buffer* skyboxVerticies;
	ID3D11Buffer* skyboxIndicies;
	UINT skyboxIndexCount;

	ID3D11ShaderResourceView* skyBoxCubeTexture;
	ID3D11Texture2D* skyBoxTexture;
	
	Texture skyboxFront;
	Texture skyboxBack;
	Texture skyboxTop;
	Texture skyboxBottom;
	Texture skyboxLeft;
	Texture skyboxRight;

	bool InitializeSkyboxVerticies(ID3D11Device* device);
	bool loadTexture(ID3D11Device* device, std::string filePath, Texture &side);
	
};

#endif // !SKYBOX_H