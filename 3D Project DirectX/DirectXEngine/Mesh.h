#ifndef MESH_H
#define MESH_H

#include "Lib/Transform.h"
#include "Lib/VertexBuffer.h"
#include "Lib/LoadObj.h"
#include "Lib/Shaders.h"

class Mesh:public Transform, public LoadObj
{
private:
	ID3D11Device *device = nullptr;
	Shader *shaderPtr = nullptr;
	ID3D11DeviceContext *deviceContext = nullptr;
	bool hit = false;
	
public:
	UINT IndexCount = 0;
	vector<Vertex> vertic() { return this->vertices; }
	bool InitializeObject(ID3D11Device *device, ID3D11DeviceContext *deviceContext, string file,
		Shader *shader, Shader *shader2);
	void draw();
	void Hit(bool isHit) { hit = isHit; }
	const bool IsHit() { return hit; }
	~Mesh();
};
#endif // !MESH_H
