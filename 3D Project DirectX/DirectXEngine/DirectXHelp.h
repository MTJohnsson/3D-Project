#ifndef HELPER_H
#define HELPER_H
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <DirectXMath.h>
#include <vector>

#include <wrl/client.h>
using namespace DirectX;
using Microsoft::WRL::ComPtr;//screen aspect

#define WIDTH  800
#define HEIGHT  600

struct LightPixelShader {
	float lightType;//4 nu 16
	XMFLOAT3 AmbientColor;
	float AmbientLight;
	XMFLOAT3 LightColor;
	float Strength;
	XMFLOAT3 position;	
};
struct LIGHT {
	LightPixelShader Light[2];
};

#endif // !HELPER_H