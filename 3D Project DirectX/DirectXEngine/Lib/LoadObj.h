#ifndef LOADOBJ_H
#define LOADOBJ_H

#include "../DirectXHelp.h"
#include <iostream>
#include <vector>
#include <string>
#include "VertexBuffer.h"
#include "Texture.h"

using namespace std;
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TextCoord;
	XMFLOAT3 Color;
};
struct Sphere
{
	float radius;
	XMFLOAT2 Max;
	XMFLOAT2 Min;
	void calculateRadius();
};


class LoadObj
{
protected:
	ID3D11Device *device = nullptr;
	VertexBuffer<Vertex> vertexBuffer;
	vector<Texture> textures;
	std::vector<Vertex> vertices;

	int numberOfTriangles = 0;

	std::vector<XMFLOAT3> position;
	std::vector<XMFLOAT3> Normal;
	std::vector<XMFLOAT2> TexCoord;
private:


	//loader functions
	bool readMtlFile(string mtlName);
	XMFLOAT3 ConvertString(string& f1, string& f2, string& f3);
	XMFLOAT2 ConvertString(string& f1, string& f2);
	Vertex setFaces(std::string f);
	Vertex setFaces2(std::string f);
	bool setTextureType(textureType type, std::string filepath);
public:
	bool loadOBJ(std::string& file);
	bool Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, string file);
	~LoadObj();
	Sphere sphere;
};



#endif // !LOADOBJ_H
