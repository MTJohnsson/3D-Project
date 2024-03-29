
#ifndef  PARTICLE_H
#define PARTICLE_H

#include "DirectXHelp.h"
#include "Lib/ConstantBuffer.h"
#include "Lib/VertexBuffer.h"
#include "Lib/Texture.h"
#include "Lib/Transform.h"
struct ParticleData
{
	XMFLOAT3 Position;
};
struct CamUpVector
{
	XMFLOAT3 Up;
};
class Particle
{
private:

	VertexBuffer<ParticleData> vertexBuffer;
	ConstantBuffer<CamUpVector> camUp;
	ID3D11DeviceContext* deviceContext;
//	XMMATRIX world = XMMatrixIdentity();
	ParticleData particle;
	Texture texture;
	ID3D11SamplerState* SamplerState;
	ID3D11BlendState* _alphaBlendState;

public:
	Particle();
	~Particle();
	void draw(ConstantBuffer<PerFrameMatrices> constantBuffer);
};

#endif // ! PARTICLE_H
