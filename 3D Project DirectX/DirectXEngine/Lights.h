#include "Lib/LoadObj.h"
#include "Lib/ConstantBuffer.h"
#include "Lib/Transform.h"
enum LightType
{
	POINTLIGHT, 
	SPOTLIGHT
}; 

//struct Light {
//	LightType type;
//	LightPixelShader light;
//};

//
//
//class Light : public LoadObj, public Transform
//{
//private:
//	LightType type;
//
//public:
//	LightPixelShader light;
//public:
//
//	void Set(LightType type, ID3D11Device *device, ID3D11DeviceContext *deviceContext, string file);
//	LightType getType() {return type; }
//	void draw(ID3D11DeviceContext *deviceContext);
//	~Light(){}
//};

