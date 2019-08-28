#ifndef GameObjects_H
#define GameObjects_H

#include "Mesh.h"
#include "Mesh/Primitive.h"
#include "Lights.h"
#include "Mesh/Terrain.h"
#include "Particle.h"
#include "Skybox.h"
#include "ReflectionSphere.h"

#include <SimpleMath.h>
#include "DirectXHelp.h"
using namespace DirectX::SimpleMath;

//Separate into per frame and per object
class GameObjects
{
private:
	vector<Primitive> primitives;
	Terrain* terrain;

	Particle* particles;

	//Skybox
	Skybox skybox;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	float gIncrement = 0;
	ConstantBuffer<PerFrameMatrices> constantBufferPerFrame;
	ConstantBuffer<PerObjectMatrices> constantBufferWorld;

	void normalize(XMFLOAT3& vector)
	{
		// Normalize ~~ | a | = sqrt((ax * ax) + (ay * ay) + (az * az))
		float factor = sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
		vector = XMFLOAT3(vector.x / factor, vector.y / factor, vector.z / factor);
	}

	XMFLOAT3 cross(const XMFLOAT3 & vector1, const XMFLOAT3 & vector2)
	{
		// Cross Product ~~ A x B = (a2b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1)
		return XMFLOAT3(vector1.y * vector2.z - vector1.z * vector2.y, vector1.z * vector2.x - vector1.x * vector2.z, vector1.x * vector2.y - vector1.y * vector2.x);
	}

	float dot(const XMFLOAT3 & vector1, const XMFLOAT3 & vector2)
	{
		// Dot Product ~~ A • B = a1b1 + a2b2 + a3b3
		//return (vector1.x*vector2.x + vector1.y*vector2.y + vector1.z*vector2.z);
		return (vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z);
	}

	float length(const XMFLOAT3& vector)
	{
		//float f = vector.x * vector.x;
		//float f2 = vector.y * vector.y;
		//float f3 = vector.z * vector.z;
		//f = f + f2 + f3;
		//f = sqrtf(f);
		return sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	}
	float magnitude(XMFLOAT3 v) { return  std::sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }
public:
	//front to back rendering
	bool RenderBackToFront();
	float LengthFromCamera(XMFLOAT3 p);
	XMFLOAT3 DirectionLength(XMFLOAT3 v, XMFLOAT3 v2) { return XMFLOAT3(v.x - v2.x, v.y - v2.y, v.z - v2.z); }



	vector<Mesh> meshes;
	ReflectionSphere shinyBall;
	//GameObjects();
	bool InitializeGameObjects(ID3D11Device* device, ID3D11DeviceContext * deviceContext, Shader *shader, Shader *shader2);
	bool InitializeReflectionBall(Shader* shinyShader);
	bool CreatePrimitive(PRIMITIVIES primitivies);
	float render(XMMATRIX view, XMMATRIX projection, XMFLOAT3 camPos, float dt);
	void renderSkybox(XMMATRIX view, XMMATRIX projection, XMFLOAT3 camPos);
	~GameObjects();
};
#endif // !OBJMODEL_H