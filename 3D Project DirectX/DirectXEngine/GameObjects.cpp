#include "GameObjects.h"


bool GameObjects::InitializeGameObjects(ID3D11Device* device, ID3D11DeviceContext * deviceContext, Shader *shader, Shader *shader2)
{
	//this->data = data;
	this->device = device;
	this->deviceContext = deviceContext;

	Mesh sphere;
	sphere.InitializeObject(device, deviceContext, "ObjFiles/earth.obj", shader, shader2);
	sphere.setScale(1, 1, 1);
	meshes.push_back(sphere);
	meshes[0].setPosition(10.f, 46.1f, 62.f);

	terrain = new Terrain(device, deviceContext, shader, shader2);

	//CreatePrimitive(CUBE);

	if (!constantBuffer.Initialize(device))
		return false;
	if (!skybox.initialize(this->device)) {
		return false;
	}
	if (!skybox.setTexture(this->device, this->deviceContext, "blue")) {
		return false;
	}
	return true;
}

bool GameObjects::CreatePrimitive(PRIMITIVIES primitivies)
{
	if (primitivies == CUBE) {
		Primitive cube;
		cube.initializeCube(device, deviceContext);
		primitives.push_back(cube);
		
	}
	return true;
}	
float speed = 0.001f;
float GameObjects::render(XMMATRIX view, XMMATRIX projection, XMFLOAT3 camPos, float dt, std::vector<XMFLOAT4> mousePickInfo)
{
	// This function calculates how much the camera should move in y-axis depending on terrain height beneath it
	float heightDifferance = terrain->getHeight(camPos);
	// return this float to Graphics

	gIncrement += dt*speed;
	constantBuffer.data.camPos = XMFLOAT3(camPos);
	constantBuffer.data.view = view;
	constantBuffer.data.projection = projection;
	
	//LightBuffer->updateConstantBuffer(deviceContext);
	//deviceContext->UpdateSubresource( *LightBuffer->getConstantBuffer(), 0, nullptr, LightBuffer, 0, 0);
	//deviceContext->PSSetConstantBuffers(0, 1, LightBuffer->getConstantBuffer());
	
	for (int i = 0; i < meshes.size(); i++)
	{
		// collision control - if hit -> rotate
		if (mousePickInfo.size() == 2)
		{
			bool hit = true;
			// LOCAL SPACE	(previous calculations in Engine.cpp, from line 40)
			XMMATRIX invertedWorld = XMMatrixInverse(NULL, meshes[i].getWorld()); // W^-1

			Vector4 rayOriginL = XMVector4Transform(Vector4(mousePickInfo[0].x, mousePickInfo[0].y, mousePickInfo[0].z, mousePickInfo[0].w), invertedWorld);
			Vector4 rayDirL = XMVector4Transform(Vector4(mousePickInfo[1].x, mousePickInfo[1].y, mousePickInfo[1].z, mousePickInfo[1].w), invertedWorld);

			// Shere intersection (from 3D lab 1)
			XMFLOAT3 l = XMFLOAT3(0.f, 1.32f, 1.f) - XMFLOAT3(rayOriginL.x, rayOriginL.y, rayOriginL.z);
			float s = dot(l, XMFLOAT3(rayDirL.x, rayDirL.y, rayDirL.z));
			float m = sqrtf(abs(length(l) * length(l) - s * s));
			
			if (s < 0 && length(l) < 1.f)
				hit = false;
			if (m > 1.f)
				hit = false;

			// Stop rotating when clicked on again
			//if (meshes[i].IsHit() == true)
			//	hit = false;
			
			meshes[i].Hit(hit);
		}
		if (meshes[i].IsHit() == true)
		{
			meshes[i].setRotation(0.0f, 1.0, 0.0f, gIncrement * 0.2);
		}
		constantBuffer.data.world = meshes[i].getWorld();
		constantBuffer.data.camPos = camPos;
		constantBuffer.updateConstantBuffer(deviceContext);
		deviceContext->GSSetConstantBuffers(0, 1, constantBuffer.getConstantBuffer());
		meshes[i].draw();
	}

	// Terrain
	constantBuffer.data.camPos = camPos;
	constantBuffer.data.world = terrain->getWorld();
	constantBuffer.data.view = view;
	constantBuffer.data.projection = projection;
	constantBuffer.updateConstantBuffer(deviceContext);
	this->deviceContext->GSSetConstantBuffers(0, 1, constantBuffer.getConstantBuffer());
	this->terrain->draw();

	return heightDifferance;
}
void GameObjects::renderSkybox(XMMATRIX view, XMMATRIX projection, XMFLOAT3 camPos) {
	XMMATRIX skyWorld = XMMatrixIdentity();
	XMMATRIX scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	XMMATRIX translation = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);
	skyWorld = scale * translation;
	this->skybox.setWorld(skyWorld, this->deviceContext);
	this->skybox.setViewProj(view, projection, this->deviceContext);
	this->skybox.setCB(this->deviceContext);

	this->skybox.render(this->deviceContext);
}
GameObjects::~GameObjects()
{
	device = nullptr;
	deviceContext = nullptr;
	meshes.clear();
	delete this->terrain;
	primitives.clear();
	this->constantBuffer.release();
}