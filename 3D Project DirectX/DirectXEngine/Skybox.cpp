#include"Skybox.h"


Skybox::Skybox()
{
	
}

Skybox::~Skybox()
{
	this->DestroySkybox();
}

void Skybox::DestroySkybox() {
	this->skyboxVerticies->Release();
	this->skyboxIndicies->Release();
}
bool Skybox::InitializeSkybox(ID3D11Device* device) {
	if (!InitializeSkyboxVerticies(device)) {
		return false;
	}
	return true;
}
bool Skybox::loadTexture(ID3D11Device* device, std::string filePath) {

}
bool Skybox::renderSkybox() {

}

bool Skybox::InitializeSkyboxVerticies(ID3D11Device* device) {
	std::vector<SkyboxVertex> verticies;
	verticies.resize(8);
	verticies[0] = { SKYBOX_WIDTH, SKYBOX_HEIGHT, SKYBOX_WIDTH, 1.0f };
	verticies[1] = { SKYBOX_WIDTH, -SKYBOX_HEIGHT, SKYBOX_WIDTH, 1.0f };
	verticies[2] = { -SKYBOX_WIDTH, SKYBOX_HEIGHT, SKYBOX_WIDTH, 1.0f };
	verticies[3] = { -SKYBOX_WIDTH, -SKYBOX_HEIGHT, SKYBOX_WIDTH, 1.0f };
	verticies[4] = { SKYBOX_WIDTH, SKYBOX_HEIGHT, -SKYBOX_WIDTH, 1.0f };
	verticies[5] = { SKYBOX_WIDTH, -SKYBOX_HEIGHT, -SKYBOX_WIDTH, 1.0f };
	verticies[6] = { -SKYBOX_WIDTH, -SKYBOX_HEIGHT, -SKYBOX_WIDTH, 1.0f };
	verticies[7] = { -SKYBOX_WIDTH, SKYBOX_HEIGHT, -SKYBOX_WIDTH, 1.0f };

	std::vector<UINT> index;
	index.resize(36);
	index = {
		0, 1, 3, 3, 2, 0,
		0, 4, 5, 5, 1, 0,
		0, 2, 7, 7, 4, 0,
		5, 4
		5, 4, 7, 7, 6, 5,
		7, 2, 3, 3, 6, 7
	}
}
bool Skybox::loadTexture(ID3D11Device* device, std::string filePath) {

}