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
bool Skybox::loadSkyboxTexture(ID3D11Device* device, std::string filePath) {
	return this->loadTexture(device, filePath);
}
bool Skybox::renderSkybox(ID3D11DeviceContext* context) {
	UINT sizeOfVertex = sizeof(SkyboxVertex);
	UINT32 offset = 0;
	if (skyboxVerticies == nullptr || skyboxIndicies == nullptr) {
		return false;
	}
	context->IASetVertexBuffers(0, 1, &skyboxVerticies, &sizeOfVertex, &offset);
	context->IASetIndexBuffer(skyboxIndicies, DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(skyboxIndexCount, 0, 0);

	return true;
}

bool Skybox::InitializeSkyboxVerticies(ID3D11Device* device) {
	HRESULT hr;
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
		5, 3, 1, 6, 3, 5,
		7, 2, 3, 3, 6, 7,
		5, 4, 7, 7, 6, 5
	};

	D3D11_BUFFER_DESC vertexBufferDesc{ 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	D3D11_BUFFER_DESC indicesBufferDesc{ 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, 0 };
	D3D11_SUBRESOURCE_DATA indexSubresourceData{};
	skyboxIndexCount = 36;

	vertexBufferDesc.ByteWidth = (sizeof(SkyboxVertex)) * 4;
	vertexSubresourceData.pSysMem = verticies.data();
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &skyboxVerticies);
	if (FAILED(hr)) {
		return false;
	}
	indicesBufferDesc.ByteWidth = sizeof(UINT) * 6;
	indexSubresourceData.pSysMem = index.data();
	hr = device->CreateBuffer(&indicesBufferDesc, &indexSubresourceData, &skyboxIndicies);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}
bool Skybox::loadTexture(ID3D11Device* device, std::string filePath) {
	if (!skyboxTexture.LoadTextureFromFile(device, filePath, textureType::DIFFUSE)) {
		return false;
	}
	return true;
}