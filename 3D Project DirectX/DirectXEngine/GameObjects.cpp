#include "GameObjects.h"


bool GameObjects::InitializeGameObjects(ID3D11Device* device, ID3D11DeviceContext * deviceContext, Shader *shader, Shader *shader2)
{
	this->device = device;
	this->deviceContext = deviceContext;

	Mesh sphere;
	sphere.InitializeObject(device, deviceContext, "ObjFiles/earth.obj", shader, shader2);
	sphere.setScale(1, 1, 1);
	meshes.push_back(sphere);
	meshes[0].setPosition(10.f, 46.1f, 62.f);

	terrain = new Terrain(device, deviceContext);

	//CreatePrimitive(CUBE);

	if (!constantBuffer.Initialize(device))
		return false;

	particles = new Particle();
	

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
float GameObjects::render(XMMATRIX view, XMMATRIX projection, XMFLOAT3 camPos, float dt)
{
	// This function calculates how much the camera should move in y-axis depending on terrain height beneath it
	float heightDifferance = terrain->getHeight(camPos);
	// return this float to Graphics

	gIncrement += dt*speed;

	constantBuffer.data.camPos = camPos;
	constantBuffer.data.view = view;
	constantBuffer.data.projection = projection;

	for (int i = 0; i < meshes.size(); i++)
	{
		if (meshes[i].IsHit() == true)
		{
			meshes[i].setRotation(0.0f, 1.0, 0.0f, gIncrement * 0.2);
		}
		constantBuffer.data.world = meshes[i].getWorld();
		constantBuffer.updateConstantBuffer(deviceContext);
		deviceContext->GSSetConstantBuffers(0, 1, constantBuffer.getConstantBuffer());
		meshes[i].draw();
	}

	// Terrain

	constantBuffer.data.world = terrain->getWorld();
	constantBuffer.updateConstantBuffer(deviceContext);
	this->deviceContext->GSSetConstantBuffers(0, 1, constantBuffer.getConstantBuffer());
	this->terrain->draw();

	particles->draw(constantBuffer);

	return heightDifferance;
}

GameObjects::~GameObjects()
{
	device = nullptr;
	deviceContext = nullptr;
	meshes.clear();
	delete this->terrain;
	primitives.clear();
	this->constantBuffer.release();
	delete this->particles;
}