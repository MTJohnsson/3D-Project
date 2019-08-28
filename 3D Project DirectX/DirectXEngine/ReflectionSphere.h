#ifndef REFLECTIONSPHERE_H
#define REFLECTIONSPHERE_H

#include "Lib/Transform.h"
#include "Lib/VertexBuffer.h"
#include "Lib/LoadObj.h"
#include "Lib/Shaders.h"
#include "Lib/ConstantBuffer.h"

class ReflectionSphere :public Transform, public LoadObj
{
private:
	ID3D11Device *device = nullptr;
	Shader *shaderPtr = nullptr;
	ID3D11ShaderResourceView* cubeSRV = nullptr;
	ID3D11DeviceContext *deviceContext = nullptr;


public:
	void setCubeSRV(ID3D11ShaderResourceView* srv) { this->cubeSRV = srv; };
	bool InitializeObject(ID3D11Device *device, ID3D11DeviceContext *deviceContext, string file,
		Shader *shader);
	void draw(ConstantBuffer<PerFrameMatrices>& VPCP, ConstantBuffer<PerObjectMatrices>& world);
	~ReflectionSphere();
};



#endif // !REFLECTIONSPHERE_H
