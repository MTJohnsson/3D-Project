#include"SkyDome.h"


SkyDome::SkyDome()
{
}

SkyDome::~SkyDome()
{
	this->sphereVerticies->Release();
	this->sphereIndicies->Release();
	if (sdrv != nullptr) {
		this->sdrv->Release();
	}
	this->DSLessEqual->Release();
	this->RSCullNone->Release();
}

void SkyDome::InitializeSphere(ID3D11Device* device, int LatLines, int LongLines)
{
	HRESULT hr;
	NrSphereVerticies = ((LatLines - 2) * LongLines) + 2;
	NrSphereFaces = ((LatLines - 3)*(LongLines) * 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<skyDomeVertex> vertices(NrSphereVerticies);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for (DWORD i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
		rotationX = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28 / (LongLines));
			rotationY = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (rotationX * rotationY));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i*LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
			vertices[i*LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
			vertices[i*LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NrSphereVerticies - 1].pos.x = 0.0f;
	vertices[NrSphereVerticies - 1].pos.y = 0.0f;
	vertices[NrSphereVerticies - 1].pos.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(skyDomeVertex) * NrSphereVerticies;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVerticies);


	std::vector<DWORD> indices(NrSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < LongLines - 1; ++j)
		{
			indices[k] = i * LongLines + j + 1;
			indices[k + 1] = i * LongLines + j + 2;
			indices[k + 2] = (i + 1)*LongLines + j + 1;

			indices[k + 3] = (i + 1)*LongLines + j + 1;
			indices[k + 4] = i * LongLines + j + 2;
			indices[k + 5] = (i + 1)*LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i*LongLines) + LongLines;
		indices[k + 1] = (i*LongLines) + 1;
		indices[k + 2] = ((i + 1)*LongLines) + LongLines;

		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
		indices[k + 4] = (i*LongLines) + 1;
		indices[k + 5] = ((i + 1)*LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = NrSphereVerticies - 1;
		indices[k + 1] = (NrSphereVerticies - 1) - (l + 1);
		indices[k + 2] = (NrSphereVerticies - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NrSphereVerticies - 1;
	indices[k + 1] = (NrSphereVerticies - 1) - LongLines;
	indices[k + 2] = NrSphereVerticies - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NrSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndicies);

}

bool SkyDome::loadCubeMap(ID3D11Device* device, std::wstring fileName) {
	HRESULT hr;
	
	std::wstring wide_string(fileName.begin(), fileName.end());
	ID3D11Texture2D* SMTexture = 0;
	hr = DirectX::CreateWICTextureFromFile(device, fileName.c_str(), (ID3D11Resource**)&SMTexture, NULL);
	//(d3d11Device, L"skymap.dds",&loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);

	if (FAILED(hr)) {
		return false;
	}

	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(SMTexture, &SMViewDesc, &sdrv);
	if (FAILED(hr)) {
		return false;
	}

	return true;
	
}

void SkyDome::updateWorld(XMFLOAT3 camPos) {
	sphereWorldMatrix = XMMatrixIdentity();
	XMMATRIX scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	XMMATRIX translation = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);
	sphereWorldMatrix = scale * translation;
}
void SkyDome::setupRenderStates(ID3D11Device* device) {
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	device->CreateRasterizerState(&rasterizerDesc, &RSCullNone);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	
	device->CreateDepthStencilState(&dssDesc, &DSLessEqual);
}
void SkyDome::drawSkydome(ID3D11DeviceContext* context) {
	UINT stride = sizeof(skyDomeVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &sphereVerticies, &stride, &offset);
	context->IASetIndexBuffer(sphereIndicies, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(skyDomeShader.GetVertexShader()->GetInputLayout());
	
	context->VSSetShader(skyDomeShader.GetVertexShader()->GetShader(), nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(skyDomeShader.GetPixelShader()->GetShader(), nullptr, 0);
	context->PSSetShaderResources(0, 1, &sdrv);
	context->OMSetDepthStencilState(DSLessEqual, 0);
	context->RSSetState(RSCullNone);
	context->DrawIndexed(NrSphereFaces * 3, 0, 0);

	context->OMSetDepthStencilState(nullptr, 0);
}