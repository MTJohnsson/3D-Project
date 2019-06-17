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
	this->skyBoxTexture->Release();
	this->skyBoxCubeTexture->Release();
}
bool Skybox::InitializeSkybox(ID3D11Device* device) {
	if (!InitializeSkyboxVerticies(device)) {
		return false;
	}
	return true;
}
bool Skybox::loadSkyboxTexture(ID3D11Device* device, std::string filePath) {
	if (!loadTexture(device, filePath, skyboxRight)) {
		return false;
	}
	if (!loadTexture(device, filePath, skyboxLeft)) {
		return false;
	}
	if (!loadTexture(device, filePath, skyboxTop)) {
		return false;
	}
	if (!loadTexture(device, filePath, skyboxBottom)) {
		return false;
	}
	if (!loadTexture(device, filePath, skyboxBack)) {
		return false;
	}
	if (!loadTexture(device, filePath, skyboxFront)) {
		return false;
	}
	HRESULT hr;


	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 6;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	/*textureDesc.Width = 1;
	textureDesc.Height = 1;*/

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	shaderResourceViewDesc.TextureCube.MipLevels = textureDesc.MipLevels;
	shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;

	ID3D11ShaderResourceView* views[6];
	views[0] = *skyboxRight.getTextureView();
	views[1] = *skyboxLeft.getTextureView();
	views[2] = *skyboxTop.getTextureView();
	views[3] = *skyboxBottom.getTextureView();
	views[4] = *skyboxBack.getTextureView();
	views[5] = *skyboxFront.getTextureView();

	D3D11_SUBRESOURCE_DATA texData[6];
	for (int i = 0; i < 6; i++) {
		texData[i].pSysMem = &views[i];
		texData[i].SysMemSlicePitch = 0;
		texData[i].SysMemPitch = sizeof(views[i]);
	}
	hr = device->CreateTexture2D(&textureDesc, texData, &skyBoxTexture);
	if (FAILED(hr)) {
		return false;
	}
	hr = device->CreateShaderResourceView(skyBoxTexture, &shaderResourceViewDesc, &skyBoxCubeTexture);
	if (FAILED(hr)) {
		return false;
	}

	
	return true;
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
bool Skybox::loadTexture(ID3D11Device* device, std::string filePath, Texture &side) {
	if (!side.LoadTextureFromFile(device, filePath, textureType::DIFFUSE)) {
		return false;
	}
	return true;
}