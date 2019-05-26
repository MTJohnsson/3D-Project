#ifndef PRIMITIVIES_H
#define PRIMITIVIES_H

#include "../DirectXHelp.h"
#include "../Lib/ConstantBuffer.h"
#include "../Lib/VertexBuffer.h"
#include "../Lib/Texture.h"

#include <vector>

enum PRIMITIVIES 
{
	CUBE, TRIANGLE
};
struct VertexPrimtives
{
	float x, y, z;
	float cx, cy, cz;
};
class Primitive
{
private:
	XMMATRIX world = XMMatrixIdentity();
	VertexBuffer<VertexPrimtives> vertexBuffer;
	ComPtr<ID3D11Buffer> indicesbuffer;
	std::vector<Texture> texture;
	int indice = 0;
public:
	bool translation(float x, float y, float z);
	XMMATRIX getWorld() { return world; }
	bool initializeCube(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	/*bool InitializeTriangle(ID3D11Device *device, ID3D11DeviceContext *deviceContext);*/
	void draw(ID3D11DeviceContext * deviceContext);
	~Primitive();
};

#endif // !OBJMODEL_H
