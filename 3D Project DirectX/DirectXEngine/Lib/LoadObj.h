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
class LoadObj
{
protected:
	ID3D11Device *device = nullptr;
	VertexBuffer<Vertex> vertexBuffer;
	vector<Texture> textures;
	std::vector<Vertex> indices;

	int numberOfTriangles = 0;
private:
	std::vector<XMFLOAT3> vertice;
	std::vector<XMFLOAT3> NormalArray;
	std::vector<XMFLOAT2> TexCoordArray;

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
};



#endif // !LOADOBJ_H
