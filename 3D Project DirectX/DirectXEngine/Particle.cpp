#include "Particle.h"
#include "Graphics.h"

Particle::Particle()
{

	particle.Position = XMFLOAT3(15.f, 49.5f, 62.f);
//	std::vector<Vertex> verticesScreenQuad;
//	std::vector<DWORD> indices;
//
//	verticesScreenQuad.resize(6);
//	indices.resize(6);
//	//TOP
//	/*verticesScreenQuad[0] = { { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } };
//	verticesScreenQuad[1] = { { -1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f } };
//	verticesScreenQuad[2] = { { 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f } };
//	verticesScreenQuad[3] = { { 1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 1.0f } };
//*/
//	verticesScreenQuad[0] = { { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } };
//	verticesScreenQuad[1] = { { -1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f } };
//	verticesScreenQuad[2] = { { 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f } };
//	verticesScreenQuad[3] = { { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } };
//	verticesScreenQuad[4] = { { 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f } };
//	verticesScreenQuad[5] = { { 1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 1.0f } };
	/*indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;*/

	HRESULT hr = vertexBuffer.Initialize(Graphics::device, &this->particle, 1);
	if (FAILED(hr))
	{
			MessageBox(NULL, "index Failed.",
			"vertexBuffer Initialisation Error", MB_OK);
	}
	//D3D11_BUFFER_DESC indexdesc;
	////memset(&indexdesc, 0, sizeof(indexdesc));
	//ZeroMemory(&indexdesc, sizeof(D3D11_BUFFER_DESC));
	//indexdesc.Usage = D3D11_USAGE_DEFAULT;
	//indexdesc.ByteWidth = sizeof(DWORD) * 6;
	//indexdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexdesc.CPUAccessFlags = 0;
	//indexdesc.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA indexData;
	//memset(&indexData, 0, sizeof(indexData));
	//indexData.pSysMem = &indices;
	//HRESULT hr = device->CreateBuffer(&indexdesc, &indexData, &indexbuffer);
	//if (FAILED(hr)) {
	//	MessageBox(NULL, "index Failed.",
	//		"CreateBuffer Initialisation Error", MB_OK);
	//	//	return false;
	//}

	this->texture.LoadTextureFromFile(Graphics::device, "snowflake.png", DIFFUSE);
	camUp.Initialize(Graphics::device);
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = Graphics::device->CreateSamplerState(&desc, &this->SamplerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "index Failed.",
			"vertexBuffer Initialisation Error", MB_OK);
	}

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	blendDesc.IndependentBlendEnable = false;
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Graphics::device->CreateBlendState(&blendDesc, &_alphaBlendState);



}

Particle::~Particle()
{
}

void Particle::draw(ConstantBuffer<PerFrameMatrices> constantBuffer, ConstantBuffer<PerObjectMatrices> world)
{
	float blendFactor[4] = { 1.25f, 1.25f, 1.25f, 0.0f };
	Graphics::deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	Graphics::deviceContext->IASetInputLayout(Graphics::ParticlesShader->GetVertexShader()->GetInputLayout());
	Graphics::deviceContext->VSSetShader(Graphics::ParticlesShader->GetVertexShader()->GetShader(), NULL, 0);
	Graphics::deviceContext->GSSetShader(Graphics::ParticlesShader->GetGeometryShader()->GetShader(), nullptr, 0);
	//Graphics::deviceContext->GSSetShader(nullptr, nullptr, 0);
	Graphics::deviceContext->PSSetShader(Graphics::ParticlesShader->GetPixelShader()->GetShader(), NULL, 0);
	
	//XMFLOAT3 Position = XMFLOAT3(10.0f, 50.0f, 62.0f);
	//XMFLOAT3 CP = (Position - camPos);
	//XMFLOAT3 normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	////float angle = atan2(Position.x - camPos.x, Position.z - camPos.z) /** (180.0 / XM_2PI)*/;
	//XMFLOAT3 axis = cross(normal, CP);
	//normalize(CP);
	//normalize(normal);
	//normalize(axis);
	//float angle = dot(normal, CP);
	//float radiant = acos(angle);// *180 / (XM_PI);
	//XMVECTOR rotaxis = XMLoadFloat3(&axis);//XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	//XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, radiant);
	//XMMATRIX Translation = XMMatrixTranslation(Position.x, Position.y, Position.z);
	//world = Rotation * Translation;
	//XMFLOAT3 Position(10.f, 46.1f, 62.f);
	//// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	//float angle = atan2(Position.x - camPos.x, Position.z - camPos.z) /** (180.0 / XM_2PI)*/;
	//XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, angle);
	//XMMATRIX Translation = XMMatrixTranslation(Position.x, Position.y, Position.z);
	//world = Rotation *Translation;

	UINT stride = sizeof(ParticleData);
	UINT offset = 0;
	/*buffer.data.camPos = Graphics::camera->GetPositionFloat3();
	buffer.data.world = world;
	buffer.updateConstantBuffer(deviceContext);
	this->deviceContext->GSSetConstantBuffers(0, 1, buffer.getConstantBuffer());*/

	XMFLOAT3 camup = XMFLOAT3(0, 1.0f, 0);// -Graphics::camera->GetPositionFloat3();
	//normalize(camup);
	this->camUp.data.Up = camup;
	this->camUp.updateConstantBuffer(Graphics::deviceContext);


	world.data.world = XMMatrixIdentity();
	world.updateConstantBuffer(Graphics::deviceContext);
	Graphics::deviceContext->GSSetConstantBuffers(0, 1, constantBuffer.getConstantBuffer());
	Graphics::deviceContext->GSSetConstantBuffers(1, 1, camUp.getConstantBuffer());
	Graphics::deviceContext->GSSetConstantBuffers(2, 1, constantBuffer.getConstantBuffer());

	Graphics::deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	Graphics::deviceContext->PSSetShaderResources(0, 1, &this->cubeSRV);
	//Graphics::deviceContext->PSSetSamplers(0, 1, &SamplerState);
	Graphics::deviceContext->Draw(1, 0);

	
}
