#include "GameObjects.h"
#include "Graphics.h"

bool GameObjects::RenderBackToFront()
{
	int minIndex;
	for (int i = 0; i < meshes.size() - 1; i++)
	{

		minIndex = i;
		for (int j = i + 1; j < meshes.size(); j++)
		{
			if (LengthFromCamera(meshes[j].getPosition()) <
				LengthFromCamera(meshes[minIndex].getPosition()))
			{
				minIndex = j;
			}
			
		}
		Mesh Temp = meshes[minIndex];
		meshes[minIndex] = meshes[i];
		meshes[i] = Temp;

	}
	return true;
}

float GameObjects::LengthFromCamera(XMFLOAT3 p)
{
	XMFLOAT3 direction = DirectionLength(Graphics::camera->GetPositionFloat3(), p);
	return magnitude(direction);
}

bool GameObjects::InitializeGameObjects(ID3D11Device* device, ID3D11DeviceContext * deviceContext, Shader *shader, Shader *shader2)
{
	this->device = device;
	this->deviceContext = deviceContext;

	Mesh sphere;
	sphere.InitializeObject(device, deviceContext, "ObjFiles/earth.obj", shader, shader2);
	sphere.setScale(1, 1, 1);
	sphere.setPosition(10.f, 46.1f, 62.f);
	meshes.push_back(sphere);


	Mesh sphere2;
	sphere2.InitializeObject(device, deviceContext, "ObjFiles/earth.obj", shader, shader2);
	sphere2.setScale(1, 1, 1);
	sphere2.setPosition(10.f, 46.1f, 68.f);
	meshes.push_back(sphere2);

	Mesh sphere3;
	sphere3.InitializeObject(device, deviceContext, "ObjFiles/earth.obj", shader, shader2);
	sphere3.setScale(1, 1, 1);
	sphere3.setPosition(15.f, 46.1f, 72.f);
	meshes.push_back(sphere3);

	terrain = new Terrain(device, deviceContext);

	//CreatePrimitive(CUBE);

	if (!constantBuffer.Initialize(device))
		return false;
	if (!skybox.initialize(this->device)) {
		return false;
	}
	if (!skybox.setTexture(this->device, this->deviceContext, "blue")) {
		return false;
	}

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

	//render back to front
	if (Graphics::FrontToBack) 
	{
		RenderBackToFront();
		int index = Graphics::FrontTobackMeshDraw;
		
		if (meshes[index].IsHit() == true)
		{
			meshes[index].setRotation(0.0f, 1.0, 0.0f, gIncrement * 0.2);
		}
		constantBuffer.data.world = meshes[index].getWorld();
		constantBuffer.updateConstantBuffer(deviceContext);
		deviceContext->GSSetConstantBuffers(0, 1, constantBuffer.getConstantBuffer());
		meshes[index].draw();
		
	}
	else
	{
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
	}


	constantBuffer.data.world = terrain->getWorld();
	constantBuffer.updateConstantBuffer(deviceContext);
	
	this->deviceContext->GSSetConstantBuffers(0, 1, constantBuffer.getConstantBuffer());
	this->terrain->draw();

	particles->draw(constantBuffer);

	return heightDifferance;
}
void GameObjects::renderSkybox(XMMATRIX view, XMMATRIX projection, XMFLOAT3 camPos) {
	XMMATRIX skyWorld = XMMatrixIdentity();
	XMMATRIX scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	XMMATRIX translation = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);
	skyWorld = scale * translation;
	this->skybox.setWorldMatrix(skyWorld, this->deviceContext);
	this->skybox.setViewProjMatrix(view, projection, this->deviceContext);
	this->skybox.setCB(this->deviceContext);

	this->skybox.renderSkybox(this->deviceContext);
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