#include "ReflectionSphere.h"

bool ReflectionSphere::InitializeObject(ID3D11Device * device, ID3D11DeviceContext * deviceContext, string file, Shader * shader)
{
	this->device = device;
	this->deviceContext = deviceContext;
	Initialize(device, deviceContext, file);

	shaderPtr = shader;
	
	return true;
}

void ReflectionSphere::draw(ConstantBuffer<PerFrameMatrices>& VPCP, ConstantBuffer<PerObjectMatrices>& world)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	world.data.world = this->getWorld();
	world.updateConstantBuffer(deviceContext);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(shaderPtr->GetVertexShader()->GetInputLayout());
	deviceContext->VSSetShader(shaderPtr->GetVertexShader()->GetShader(), NULL, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(shaderPtr->GetPixelShader()->GetShader(), NULL, 0);

	deviceContext->VSSetConstantBuffers(0, 1, VPCP.getConstantBuffer());
	deviceContext->VSSetConstantBuffers(1, 1, world.getConstantBuffer());

	deviceContext->PSSetShaderResources(0, 1, &cubeSRV);
	
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.Stride(), &offset);
	deviceContext->Draw(vertices.size(), 0);

}

ReflectionSphere::~ReflectionSphere()
{
	this->shaderPtr = nullptr;
	delete this->shaderPtr;
}
