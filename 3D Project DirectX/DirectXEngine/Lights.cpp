#include "Lights.h"

//
//void Light::Set(LightType type, ID3D11Device * device, ID3D11DeviceContext * deviceContext, string file)
//{
//	light.lightType = type;
//	light.AmbientColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
//	light.LightColor = XMFLOAT3(0.60f, 0.50f, 0.6f);
//	light.Strength = 0.1;
//	light.position = this->getPosition();
//	this->Initialize(device, deviceContext, file);
//	//this->CreatePrimitive(CUBE);
//	//!LightBuffer.Initialize(device);
//	
//}
//
//void Light::draw(ID3D11DeviceContext * deviceContext)
//{
//	UINT stride = sizeof(Vertex);
//	UINT offset = 0;
//	for (int i = 0; i < textures.size(); i++)
//	{
//		/*if (DIFFUSE == textures[i].getType())
//			deviceContext->PSSetShaderResources(0, 1, textures[i].getTextureView());*/
//		if (DEFAULT == textures[i].getType())
//			deviceContext->PSSetShaderResources(0, 1, textures[i].getTextureView());
//	}
//	/*LightBuffer.data.lightType = type;
//	LightBuffer.data.AmbientColor = ambientColor;
//	LightBuffer.data.AmbientLight = ambientStrength;
//	LightBuffer.data.LightColor = color;
//	LightBuffer.data.Strength = strength;
//	LightBuffer.data.position = this->Position;
//	LightBuffer.updateConstantBuffer(deviceContext);
//	deviceContext->PSSetConstantBuffers(0, 1, LightBuffer.getConstantBuffer());*/
//	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.Stride(), &offset);
//	deviceContext->Draw(indices.size(), 0);
//}
