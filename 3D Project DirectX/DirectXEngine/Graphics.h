#ifndef Graphics_H
#define Graphics_H

//#include "Shaders.h"
//#include "Vertex.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "Input/camera.h"
#include "GameObjects.h"

#include "Mouse.h"

//Colours
static const float cyan[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
static const float magenta[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
static const float yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
static const float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
static const float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

struct BufferDisplay {
	float display;
	XMFLOAT3 padding;
};
struct RenderTarget {
	ID3D11Texture2D* textureTarget = nullptr;
	ID3D11RenderTargetView* renderTarget = nullptr;
	ID3D11ShaderResourceView* shaderResource = nullptr;
};
const UINT BUFFERS = 4;

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame(float dt, float x, float y);
	~Graphics();

	//Deferred Render Functions
	void FirstRender();
	void LastRender();
	void DrawPass(float dt, float x, float y);

	bool InitializeGraphicsBuffer();
	void DestroyGraphicsBuffer();
	void InitializeScreenQuad();
	void setViewPort();

	//picking
	void Picking(float x, float y);
	bool RaySphereIntersect(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection, XMFLOAT3 pos, float radius);
	float RayTriangle(XMVECTOR pickRayInWorldSpacePos,
		XMVECTOR pickRayInWorldSpaceDir,
		XMMATRIX& worldSpace, int index);

	bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point);
	//static Shader* shader;		// outputs texture
	static Shader* ParticlesShader;
	static Shader* deferredShaders;
	static ID3D11Device* device;// = nullptr;
	static ID3D11DeviceContext* deviceContext;// = nullptr;
	static Camera* camera;

	static int FrontTobackMeshDraw;
	static bool FrontToBack;


	bool CullBackFace = false;
	XMFLOAT3 CullCamera;
private:
	void updateImguie(float dt);
	GameObjects objects;

	//ID3D11Device* device = nullptr;
	//ID3D11DeviceContext* deviceContext = nullptr;

	ConstantBuffer<LIGHT> LightBuffer;

	//Deferred Render
	RenderTarget renderBuffers[BUFFERS];
	ID3D11DepthStencilView* renderDepthStencil = nullptr;
	//Screen-quad for last pass
	ID3D11Buffer* sqVerticesBuffer;
	ID3D11Buffer* sqIndicesBuffer;
	UINT sqIndicesCountForBuffer;

	int deferredBufferDisplay;
	ConstantBuffer<BufferDisplay> bufferDisplayBuffer;

private:
	bool InitializeDirectX(HWND hwmd, int width, int height);
	bool InitializeShaders();
	bool InitializeScene();
	bool ResetShaders();
	//ID3D11Device* device; //create buffers
	//ID3D11DeviceContext* deviceContext; //set diffrent resources to renderting
	//we set them to diffrent textures
	IDXGISwapChain* swapchain; //swap out our frames when we render it, 
	ID3D11RenderTargetView* renderTargetView; // for our window

	//Shader *shader;		// outputs texture
	//Shader *shader2;
	ID3D11RasterizerState* rasterizerState;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;
	//om både befinner sig på samma zeta ifrån kammran vill man se den röda trianglen istället för gröna i exemplet
	//ID3D11DepthStencilState* depthStencilState;

	//sampler state for texturing
	ID3D11SamplerState*samplerState;

	//Deferred Render Shaders
	Shader lastPassShaders;
	Shader deferredShadersNormalMapping;




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

	float length(const XMFLOAT3 & vector)
	{
		//float f = vector.x * vector.x;
		//float f2 = vector.y * vector.y;
		//float f3 = vector.z * vector.z;
		//f = f + f2 + f3;
		//f = sqrtf(f);
		return sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	}
};
#endif // !OBJMODEL_H
