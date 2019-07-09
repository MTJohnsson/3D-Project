#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../DirectXHelp.h"

#include <iostream>
#include <fstream>
#include <string>
#include <WICTextureLoader.h>
#include <wrl/client.h>
#include "../Lib/Transform.h"
#include "../Lib/LoadObj.h"
#include "../Lib/Shaders.h"
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

using namespace DirectX;

struct Vertex3
{
	float x, y, z;
	float xn, yn, zn;	// normals
	float coordU, coordV;
	float r, g, b;
};

class Terrain :public Transform/*, public LoadObj*/
{
public:
	Terrain(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	virtual ~Terrain();
	
	void draw();
	void release() {
		if (indicesbuffer != nullptr) {
			indicesbuffer->Release();
		}
		if (vertexBuffer != nullptr) {
			vertexBuffer->Release();
		}
	}

	float getHeight(XMFLOAT3 camPos);

private:
	ID3D11Device * device = nullptr;
	ID3D11DeviceContext *deviceContext = nullptr;
	Shader *shaderPtr = nullptr;

	void generateVertex();
	void generateIndex();
	void initiateVertex();
	bool readFromFile();

	void normalize(XMFLOAT3 &vector);
	XMFLOAT3 cross(const XMFLOAT3 &vector1, const XMFLOAT3 &vector2);
	float dot(const XMFLOAT3 &vector1, const XMFLOAT3 &vector2);

	const int TERR_WIDTH = 100;
	const int TERR_LENGTH = 100;

	int verCount = TERR_WIDTH * TERR_LENGTH;
	int indCount = (TERR_WIDTH - 1) * (TERR_LENGTH - 1) * 6;

	ID3D11Buffer* indicesbuffer = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;

	// Generate and Initiate Vertex
	std::vector<Vertex3> indices;
	std::vector<int> vertices;
	std::vector<Vertex3> vertex;
	std::vector<int> yValues;

	// Texture
	ComPtr<ID3D11ShaderResourceView> texture;

	// Ground Intersection
	void findNearestVertecies(XMFLOAT3 camPos);
	float baryCentric(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, XMFLOAT2 pos);

	std::vector<Vertex3> nearestVertecies;

};
#endif