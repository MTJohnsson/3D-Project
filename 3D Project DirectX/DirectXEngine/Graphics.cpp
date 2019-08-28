#include "Graphics.h"
#include "wrl/client.h"

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment (lib, "d3dcompiler.lib")


Shader* Graphics::ParticlesShader = nullptr;
Shader* Graphics::deferredShaders = nullptr;

ID3D11Device* Graphics::device = nullptr;
ID3D11DeviceContext* Graphics::deviceContext = nullptr;
Camera* Graphics::camera = nullptr;
bool Graphics::FrontToBack = false;
int Graphics::FrontTobackMeshDraw = 0;

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	if (!InitializeDirectX(hwnd, width, height))
		return false;
	if (!InitializeGraphicsBuffer())
		return false;
	if (!bufferDisplayBuffer.Initialize(device))
		return false;
	/*if (!MatrixBuffer.Initialize(device))
		return false;*/
	if (!InitializeShaders())
		return false;
	if (!InitializeScene())
		return false;
	InitializeScreenQuad();
	//objects.InitializeGameObjects(device, deviceContext, shader, shader2);
	//objects.InitializeGameObjects(device, deviceContext, &deferredShaders, &deferredShadersNormalMapping);
	objects.InitializeGameObjects(device, deviceContext, &deferredShadersNormalMapping, deferredShaders);
	objects.InitializeReflectionBall(&reflectionShaders);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);
	ImGui::StyleColorsDark();

	return true;
}

POINT cursor;
bool my_tool_active = true;
float dist = 0.0f;
float distZ = 0.0f;
float heightDifferance = 0;
void Graphics::RenderFrame(float dt, float x, float y)
{
	FirstRender();
	DrawPass(dt, x,y);
	LastRender();
	//deviceContext->PSSetConstantBuffers(0, 1, objects.LightBuffer->getConstantBuffer());

	//float clearColor[] = { 0.06f, 0.06f,0.06f, 1.0f };
	//deviceContext->ClearRenderTargetView(this->renderTargetView, clearColor);
	////clear dethstencil view
	//deviceContext->ClearDepthStencilView(depthStencilView,
	//	D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	////this->deviceContext->PSSetConstantBuffers(0, 1, &constBuffer.getConstantBuffer());
	////draw
	//deviceContext->IASetInputLayout(shader->GetVertexShader()->GetInputLayout());
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	////after setting Topplogy we need to set rasterizer state
	//deviceContext->RSSetState(rasterizerState);

	////set depthStencil state
	//deviceContext->OMSetDepthStencilState(nullptr, 0);
	//deviceContext->PSSetSamplers(0, 1, &samplerState);
	//deviceContext->HSSetShader(nullptr, nullptr, 0);
	//deviceContext->DSSetShader(nullptr, nullptr, 0);
	//deviceContext->PSSetConstantBuffers(0, 1, LightBuffer.getConstantBuffer());


	//heightDifferance = objects.render(camera->GetViewMatrix(), camera->GetProjectionMatrix(),
	//	camera->GetPositionFloat3(), dt, mousePickInfo);	// take mouse ray to the objects in order to calculate find local space
	//
	//// Move camera depending on the height of the ground beneath, use float that is returned from objects.render()
	//camera->SetPosition(camera->GetPositionFloat3().x, camera->GetPositionFloat3().y - heightDifferance, camera->GetPositionFloat3().z);

	////draw objects
	//deviceContext->GSSetShader(nullptr, nullptr, 0);
	updateImguie(dt);
	this->swapchain->Present(0, NULL);
}
void Graphics::FirstRender() {

	deviceContext->IASetInputLayout(deferredShaders->GetVertexShader()->GetInputLayout());
	deviceContext->OMSetDepthStencilState(nullptr, 0);
	deviceContext->VSSetShader(deferredShaders->GetVertexShader()->GetShader(), NULL, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(deferredShaders->GetGeometryShader()->GetShader(), nullptr, 0);
	deviceContext->PSSetShader(deferredShaders->GetPixelShader()->GetShader(), NULL, 0);
	deviceContext->PSSetSamplers(0, 1, &samplerState);


	ID3D11RenderTargetView* renderTargetViews[] = {
		renderBuffers[0].renderTarget,
		renderBuffers[1].renderTarget,
		renderBuffers[2].renderTarget
	};
	deviceContext->OMSetRenderTargets(BUFFERS, renderTargetViews, renderDepthStencil);

	deviceContext->ClearRenderTargetView(renderBuffers[0].renderTarget, cyan);
	deviceContext->ClearRenderTargetView(renderBuffers[1].renderTarget, black);
	deviceContext->ClearRenderTargetView(renderBuffers[2].renderTarget, yellow);
	deviceContext->ClearDepthStencilView(renderDepthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->RSSetState(NULL);
	//deviceContext->RSSetState(NULL);
}
void Graphics::LastRender() {
	ID3D11RenderTargetView* nullRtv[] = { nullptr };
	deviceContext->OMSetRenderTargets(1, nullRtv, nullptr);

	deviceContext->IASetInputLayout(lastPassShaders.GetVertexShader()->GetInputLayout());
	deviceContext->VSSetShader(lastPassShaders.GetVertexShader()->GetShader(), NULL, 0);
	deviceContext->GSSetShader(lastPassShaders.GetGeometryShader()->GetShader(), NULL, 0);
	deviceContext->PSSetShader(lastPassShaders.GetPixelShader()->GetShader(), NULL, 0);
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	deviceContext->PSSetShaderResources(2, 1, &renderBuffers[0].shaderResource);
	deviceContext->PSSetShaderResources(3, 1, &renderBuffers[1].shaderResource);
	deviceContext->PSSetShaderResources(4, 1, &renderBuffers[2].shaderResource);


	bufferDisplayBuffer.data.display = deferredBufferDisplay;
	bufferDisplayBuffer.data.camPos = camera->GetPositionFloat3();
	bufferDisplayBuffer.updateConstantBuffer(deviceContext); //Fail
	deviceContext->GSSetConstantBuffers(0, 1, bufferDisplayBuffer.getConstantBuffer());
	deviceContext->PSSetConstantBuffers(1, 1, bufferDisplayBuffer.getConstantBuffer());

	deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
	deviceContext->ClearRenderTargetView(renderTargetView, cyan);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	const UINT offset = 0;
	const UINT stride = sizeof(Vertex);
	deviceContext->IASetVertexBuffers(0, 1, &sqVerticesBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(sqIndicesBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(sqIndicesCountForBuffer, 0, 0);

	ID3D11ShaderResourceView* nullsrv = nullptr;

	deviceContext->PSSetShaderResources(2, 1, &nullsrv);
	deviceContext->PSSetShaderResources(3, 1, &nullsrv);
	deviceContext->PSSetShaderResources(4, 1, &nullsrv);
	deviceContext->GSSetShader(NULL, NULL, 0);
}

void Graphics::DrawPass(float dt, float x, float y) {
	
	//set depthStencil state
	//deviceContext->OMSetDepthStencilState(nullptr, 0);
	//deviceContext->PSSetSamplers(0, 1, &samplerState);
	//deviceContext->HSSetShader(nullptr, nullptr, 0);
	//deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetConstantBuffers(0, 1, LightBuffer.getConstantBuffer());

	//setViewPort();
	if (!CullBackFace)
		CullCamera = camera->GetPositionFloat3();
	heightDifferance = objects.render(camera->GetViewMatrix(), camera->GetProjectionMatrix(),CullCamera, dt);

	// Move camera depending on the height of the ground beneath, use float that is returned from objects.render()
	camera->SetPosition(camera->GetPositionFloat3().x, camera->GetPositionFloat3().y - heightDifferance, camera->GetPositionFloat3().z);
	
	Picking(x, y);
	
	deviceContext->OMSetDepthStencilState(depthStencilState, 0);
	objects.renderSkybox(camera->GetViewMatrix(), camera->GetProjectionMatrix(), camera->GetPositionFloat3());

	//draw objects
	deviceContext->GSSetShader(nullptr, nullptr, 0);
}

bool Graphics::InitializeGraphicsBuffer() {
	HRESULT hr;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = WIDTH;
	textureDesc.Height = HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	for (UINT i = 0; i < BUFFERS; i++) {
		hr = (device->CreateTexture2D(&textureDesc, NULL, &renderBuffers[i].textureTarget));
		if (FAILED(hr))
		{
			MessageBox(NULL, ("Create 2D Texture Failed!"),
				("Didn't Create 2D Texture!"), MB_OK);
			return false;
		}
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	for (UINT i = 0; i < BUFFERS; i++) {
		hr = (device->CreateRenderTargetView(renderBuffers[i].textureTarget, &renderTargetViewDesc, &renderBuffers[i].renderTarget));
		if (FAILED(hr))
		{
			MessageBox(NULL, ("Create Render Target View Failed!"),
				("Didn't Create Render Target View!"), MB_OK);
			return false;
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (UINT i = 0; i < BUFFERS; i++) {
		hr = (device->CreateShaderResourceView(renderBuffers[i].textureTarget, &shaderResourceViewDesc, &renderBuffers[i].shaderResource));
		if (FAILED(hr))
		{
			MessageBox(NULL, ("Create Shader Resource View Failed!"),
				("Didn't Create Shader Resource View!"), MB_OK);
			return false;
		}
	}

	ID3D11Texture2D* depthStencilTexture = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilBufferDesc.Width = WIDTH;
	depthStencilBufferDesc.Height = HEIGHT;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = (device->CreateTexture2D(&depthStencilBufferDesc, NULL, &depthStencilTexture));
	if (FAILED(hr))
	{
		MessageBox(NULL, "Create Depth Stencil 2D Texture Failed.",
			"Didn't Create Depth Stencil 2D Texture", MB_OK);
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = depthStencilBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	hr = (device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &renderDepthStencil));
	if (FAILED(hr))
	{
		MessageBox(NULL, "Create Depth Stencil View Failed.",
			"Didn't Create Depth Stencil View", MB_OK);
		return false;
	}

	depthStencilTexture->Release();
	return true;
} //Initialize the buffers
void Graphics::DestroyGraphicsBuffer() {
	renderDepthStencil->Release();

	for (UINT i = 0; i < BUFFERS; i++) {
		renderBuffers[i].textureTarget->Release();
		renderBuffers[i].renderTarget->Release();
		renderBuffers[i].shaderResource->Release();
	}
} //Releases the Graphics Buffers
void Graphics::InitializeScreenQuad() {
	std::vector<Vertex> verticesScreenQuad;
	std::vector<UINT> indices;

	verticesScreenQuad.resize(4);
	indices.resize(6);

	verticesScreenQuad[0] = { { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } };
	verticesScreenQuad[1] = { { -1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f } };
	verticesScreenQuad[2] = { { 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f } };
	verticesScreenQuad[3] = { { 1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 1.0f } };

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	D3D11_BUFFER_DESC vertexBufferDesc{ 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	D3D11_BUFFER_DESC indicesBufferDesc{ 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, 0 };
	D3D11_SUBRESOURCE_DATA indexSubresourceData{};
	sqIndicesCountForBuffer = 6;

	vertexBufferDesc.ByteWidth = (sizeof(Vertex)) * 4;
	vertexSubresourceData.pSysMem = verticesScreenQuad.data();
	device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &sqVerticesBuffer);

	indicesBufferDesc.ByteWidth = sizeof(UINT) * 6;
	indexSubresourceData.pSysMem = indices.data();
	device->CreateBuffer(&indicesBufferDesc, &indexSubresourceData, &sqIndicesBuffer);
} //Creates a quad to render the buffers to.

void Graphics::setViewPort()
{
	D3D11_VIEWPORT vp;
	vp.Height = (float)WIDTH;
	vp.Width = (float)HEIGHT;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	deviceContext->RSSetViewports(1, &vp);
}
XMFLOAT2 MousePosition;
void Graphics::Picking(float x, float y)
{
	if (x == -1 || y ==-1)
		return;
	
	XMFLOAT4X4 P;
	XMStoreFloat4x4(&P, camera->GetProjectionMatrix());
	/*
	// Compute picking ray in view space.
	float vx = (+2.0f * x / 1280 - 1.0f) / P(0, 0);
	float vy = (-2.0f * y / 1024 + 1.0f) / P(1, 1);
	MousePosition.x = vx;
	MousePosition.y = vy;
	// Ray definition in view space.
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	XMMATRIX V = camera->GetViewMatrix();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);


	//check objects
	XMMATRIX W = objects.meshes[0].getWorld();
	
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);
	// Tranform ray to vi space of Mesh.
	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	// Make the ray direction unit length for the intersection tests.
	rayDir = XMVector3Normalize(rayDir);

	XMFLOAT3 o =objects.meshes[0].getPosition();
	XMVECTOR object = XMVectorSet(o.x, o.y+1, o.z, 1.0f);
	
	rayOrigin = XMVectorSubtract(object, rayOrigin);
	
	XMFLOAT4 rOrigin;
	XMStoreFloat4(&rOrigin, rayOrigin);
	XMFLOAT4 rDir;
	XMStoreFloat4(&rDir, rayDir);
	
	float radius = objects.meshes[0].sphere.radius;
	bool hit = RaySphereIntersect(rOrigin, rDir, o, radius);
	if (hit)
	{
		objects.meshes[0].Hit(true);
	}
	else
		objects.meshes[0].Hit(false);*/


	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float PRVecX, PRVecY, PRVecZ;
	//Transform 2D pick position on screen space to 3D ray in View space

	PRVecX = (+2.0f * x / 1280 - 1.0f) / P(0, 0);
	PRVecY = (-2.0f * y / 1024 + 1.0f) / P(1, 1);
	MousePosition.x = PRVecX;
	MousePosition.y = PRVecY;

	PRVecZ = 1.0f;  //View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

	pickRayInViewSpaceDir = XMVectorSet(PRVecX, PRVecY, PRVecZ, 0.0f);

	//Uncomment this line if you want to use the center of the screen (client area)
	//to be the point that creates the picking ray (eg. first person shooter)
	//pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Transform 3D Ray from View space to 3D ray in World space
	XMMATRIX pickRayToWorldSpaceMatrix;
	XMVECTOR matInvDeter;   //We don't use this, but the xna matrix inverse function requires the first parameter to not be null

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, camera->GetViewMatrix());   //Inverse of View Space matrix is World space matrix

	XMVECTOR pickRayInWorldSpacePos = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
	XMVECTOR pickRayInWorldSpaceDir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
	pickRayInWorldSpaceDir = XMVector3Normalize(pickRayInWorldSpaceDir);

	for (int i = 0; i < objects.meshes.size(); i++)
	{
		int distanse = RayTriangle(pickRayInWorldSpacePos, pickRayInWorldSpaceDir, objects.meshes[i].getWorld(), i);
		if (distanse > 0)
		{
			objects.meshes[i].Hit(true);
		}
		else {
			objects.meshes[i].Hit(false);
		}
	}
}

bool Graphics::RaySphereIntersect(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection, XMFLOAT3 pos, float radius)
{
	XMFLOAT3 oc = XMFLOAT3(rayOrigin.x, rayOrigin.y, rayOrigin.z)- objects.meshes[0].getPosition();
	float b = dot(XMFLOAT3(rayDirection.x, rayDirection.y, rayDirection.z), oc);

	float c = dot(oc, oc)- (radius*radius);
	if ((b * b) - c <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}
	return false;
	return true;
}

float Graphics::RayTriangle(XMVECTOR pickRayInWorldSpacePos, XMVECTOR pickRayInWorldSpaceDir, XMMATRIX& worldSpace, int index)
{
	//pick vertices
	//auto vertices = (Vertex*)objects.meshes[0].vBuffer().GetAddressOf();
	auto vertices = objects.meshes[index].vertic();
	UINT triCount = objects.meshes[index].IndexCount;
	float tMin = 0.0f;
	for (UINT i = 0; i < triCount / 3; i++)
	{
		UINT i0 = i * 3 + 0;
		UINT i1 = i * 3 + 1;
		UINT i2 = i * 3 + 2;

		// Vertices for this triangle.
		XMVECTOR v0 = XMLoadFloat3(&vertices[i0].Position);
		XMVECTOR v1 = XMLoadFloat3(&vertices[i1].Position);
		XMVECTOR v2 = XMLoadFloat3(&vertices[i2].Position);

		//Transform the vertices to world space
		v0 = XMVector3TransformCoord(v0, worldSpace);
		v1 = XMVector3TransformCoord(v1, worldSpace);
		v2 = XMVector3TransformCoord(v2, worldSpace);


		//Find the normal using U, V coordinates (two edges)
		XMVECTOR U = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR V = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR faceNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		U = v1 - v0;
		V = v2 - v0;

		//Compute face normal by crossing U, V
		faceNormal = XMVector3Cross(U, V);

		faceNormal = XMVector3Normalize(faceNormal);

		//Calculate a point on the triangle for the plane equation
		XMVECTOR triPoint = v0;

		//Get plane equation ("Ax + By + Cz + D = 0") Variables
		float tri1A = XMVectorGetX(faceNormal);
		float tri1B = XMVectorGetY(faceNormal);
		float tri1C = XMVectorGetZ(faceNormal);
		float tri1D = (-tri1A * XMVectorGetX(triPoint) - tri1B * XMVectorGetY(triPoint) - tri1C * XMVectorGetZ(triPoint));

		//Now we find where (on the ray) the ray intersects with the triangles plane
		float ep1, ep2, t = 0.0f;
		float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;
		XMVECTOR pointInPlane = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		ep1 = (XMVectorGetX(pickRayInWorldSpacePos) * tri1A) + (XMVectorGetY(pickRayInWorldSpacePos) * tri1B) + (XMVectorGetZ(pickRayInWorldSpacePos) * tri1C);
		ep2 = (XMVectorGetX(pickRayInWorldSpaceDir) * tri1A) + (XMVectorGetY(pickRayInWorldSpaceDir) * tri1B) + (XMVectorGetZ(pickRayInWorldSpaceDir) * tri1C);

		//Make sure there are no divide-by-zeros
		if (ep2 != 0.0f)
			t = -(ep1 + tri1D) / (ep2);

		if (t > 0.0f)    //Make sure you don't pick objects behind the camera
		{
			//Get the point on the plane
			planeIntersectX = XMVectorGetX(pickRayInWorldSpacePos) + XMVectorGetX(pickRayInWorldSpaceDir) * t;
			planeIntersectY = XMVectorGetY(pickRayInWorldSpacePos) + XMVectorGetY(pickRayInWorldSpaceDir) * t;
			planeIntersectZ = XMVectorGetZ(pickRayInWorldSpacePos) + XMVectorGetZ(pickRayInWorldSpaceDir) * t;

			pointInPlane = XMVectorSet(planeIntersectX, planeIntersectY, planeIntersectZ, 0.0f);

			//Call function to check if point is in the triangle
			if (PointInTriangle(v0, v1, v2, pointInPlane))
			{
				//Return the distance to the hit, so you can check all the other pickable objects in your scene
				//and choose whichever object is closest to the camera
				return t / 2.0f;
			}
		}
	}
	return -1;
}

bool Graphics::PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point)
{
	//To find out if the point is inside the triangle, we will check to see if the point
	//is on the correct side of each of the triangles edges.

	XMVECTOR cp1 = XMVector3Cross((triV3 - triV2), (point - triV2));
	XMVECTOR cp2 = XMVector3Cross((triV3 - triV2), (triV1 - triV2));
	if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
	{
		cp1 = XMVector3Cross((triV3 - triV1), (point - triV1));
		cp2 = XMVector3Cross((triV3 - triV1), (triV2 - triV1));
		if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
		{
			cp1 = XMVector3Cross((triV2 - triV1), (point - triV1));
			cp2 = XMVector3Cross((triV2 - triV1), (triV3 - triV1));
			if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
			{
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}


Graphics::~Graphics()
{
	//classer som vi includerar

	this->device->Release();
	this->deviceContext->Release();
	this->swapchain->Release();
	this->renderTargetView->Release();

	this->samplerState->Release();
	this->rasterizerState->Release();
	this->depthStencilBuffer->Release();
	this->depthStencilView->Release();
	this->depthStencilState->Release();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	delete ParticlesShader;
	delete deferredShaders;
	delete camera;
	DestroyGraphicsBuffer();
}


void Graphics::updateImguie(float dt)
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Project!", &my_tool_active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("frame (%.1f FPS)", 1000.0f / dt);
	//ImGui::SliderFloat("Dist", (float*)&dist, -10.05f, 10.05f);
	//ImGui::SliderFloat("Up/Down ", (float*)&distZ, -1.05f, 1.05f);
	switch (deferredBufferDisplay) {
	case 0:
		ImGui::Text("[ Diffuse + Ambient + Specular ]");
		break;
	case 1:
		ImGui::Text("[ Normal ]");
		break;
	case 2:
		ImGui::Text("[ Depth ]");
		break;
	case 3:
		ImGui::Text("[ Diffuse + Ambient ]");
		break;
	case 4:
		ImGui::Text("[ Ambient + Specular ]");
		break;
	case 5:
		ImGui::Text("[ Diffuse + Specular ]");
		break;
	}
	ImGui::SliderInt("Load Buffer: ", &deferredBufferDisplay, 0, 5);
	ImGui::Checkbox("BackFaceCull", &CullBackFace);
	ImGui::Text("MousePosition ( %f , %f )", MousePosition.x, MousePosition.y);

	ImGui::Checkbox("FrontToBack", &FrontToBack);
	ImGui::SliderInt("Mesh render: ", &FrontTobackMeshDraw, 0, 2);


	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
	
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;
	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hwnd;                           // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,//this->swapchain.GetAddressOf(),
		&device,//this->device.GetAddressOf(),
		NULL,
		&deviceContext);//this->deviceContext.GetAddressOf());

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
		pBackBuffer->Release();

	}
	if (FAILED(hr))
	{
		MessageBox(NULL,"D3D11CreateDevice Failed.", "Failed to create device and swapchain", MB_OK);
		return false;
	}
	//eget för depthStencilDesc;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	hr = device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateTexture2D Failed.", "Failed to create depthStencilDesc", MB_OK);
		return false;
	}

	hr = device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateDepthStencilView Failed.", "Failed to create depthStencilBuffer", MB_OK);
		return false;
	}

	// set the render target as the back buffer
	deviceContext->OMSetRenderTargets(1, &renderTargetView, this->depthStencilView);

	//create the viewposrt rasterizer
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	//eget för depthstencil
	vp.MinDepth = 0.0f; //closest to camera as possible
	vp.MaxDepth = 1.0f; //futherest from camera as possible

	deviceContext->RSSetViewports(1, &vp);

	//Create Rasterize state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	hr = device->CreateRasterizerState(&rasterizerDesc,&rasterizerState);
	if (FAILED(hr)) //If error occurred
	{
		MessageBox(NULL, "Failed to create rasterizer state.",
			"D3D11 Initialisation Error", MB_OK);
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = device->CreateDepthStencilState(&dssDesc, &depthStencilState);
	if(FAILED(hr))
	{
		MessageBox(NULL, "Failed to create depth stencil state.",
			"D3D11 Initialisation Error", MB_OK);
		return false;
	}

	return true;
}

bool Graphics::InitializeShaders()
{

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC layout2[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  }
	};

	UINT numElements = ARRAYSIZE(layout);
	UINT numElements2 = ARRAYSIZE(layout2);
	std::wstring shaderfolder = L"..\\x64\\Debug\\";


	/*shader = new Shader();
	shader->CreatVertexShader(device, shaderfolder + L"VertexShader.cso", layout, numElements);
	shader->CreatGeometryShader(device, shaderfolder + L"GeometryShader.cso");
	shader->CreatPixelShader(device, shaderfolder + L"PixelShader.cso");


	shader2 = new Shader();
	shader2->CreatVertexShader(device, shaderfolder + L"VertexShader.cso", layout, numElements);
	shader2->CreatGeometryShader(device, shaderfolder + L"GeometryShader.cso");
	shader2->CreatPixelShader(device, shaderfolder + L"PixelShaderDefault.cso");*/

	lastPassShaders.CreatVertexShader(device, shaderfolder + L"LastPassVertexShader.cso", layout2, numElements2);
	lastPassShaders.CreatGeometryShader(device, shaderfolder + L"LastPassGeometryShader.cso");
	lastPassShaders.CreatPixelShader(device, shaderfolder + L"LastPassPixelShader.cso");
	
	deferredShaders = new Shader();
	deferredShaders->CreatVertexShader(device, shaderfolder + L"DeferredVertexShader.cso", layout2, numElements2);
	deferredShaders->CreatGeometryShader(device, shaderfolder + L"DeferredGeometryShader.cso");
	deferredShaders->CreatPixelShader(device, shaderfolder + L"DeferredPixelShader.cso");

	deferredShadersNormalMapping.CreatVertexShader(device, shaderfolder + L"DeferredVertexShader.cso", layout2, numElements2);
	deferredShadersNormalMapping.CreatPixelShader(device, shaderfolder + L"DeferredPixelShaderNormalMapping.cso");
	deferredShadersNormalMapping.CreatGeometryShader(device, shaderfolder + L"DeferredGeometryShader.cso");

	reflectionShaders.CreatVertexShader(device, shaderfolder + L"ReflectionVS.cso", layout2, numElements2);
	reflectionShaders.CreatPixelShader(device, shaderfolder + L"ReflectionPS.cso");

	this->ParticlesShader = new Shader();
	D3D11_INPUT_ELEMENT_DESC layout3[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};
	UINT numElements3 = ARRAYSIZE(layout3);

	bool completed = true;
	completed = ParticlesShader->CreatVertexShader(device, shaderfolder + L"ParticleVS.cso", layout3, numElements3);
	completed = ParticlesShader->CreatPixelShader(device, shaderfolder + L"ParticlePS.cso");
	completed = ParticlesShader->CreatGeometryShader(device, shaderfolder + L"ParticleGS.cso");

	if (!completed)
	{
		MessageBox(NULL, "ParticlesShader Failed.",
			"CreateVertexBuffer Error", MB_OK);
		return false;
	}
	//Create sampler description for sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = device->CreateSamplerState(&sampDesc, &this->samplerState); //Create sampler state
	if (FAILED(hr))
	{
		MessageBox(NULL, "D3D11CreateDevice Failed.",
			"CreateVertexBuffer Error", MB_OK);
		return false;
	}

	LightBuffer.Initialize(device);
	
	LightBuffer.data.Light[0].lightType = SPOTLIGHT;
	LightBuffer.data.Light[0].AmbientColor = XMFLOAT3(0.2, 0.2, 0.2);
	LightBuffer.data.Light[0].AmbientLight = 1.f;
	LightBuffer.data.Light[0].LightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	LightBuffer.data.Light[0].Strength = 0.1;
	LightBuffer.data.Light[0].position = XMFLOAT3(0.0f, 10.0f, 0.0f);

	LightBuffer.data.Light[1].lightType = POINTLIGHT;
	LightBuffer.data.Light[1].AmbientColor = XMFLOAT3(0.2, 0.2, 0.2);
	LightBuffer.data.Light[1].AmbientLight = 10.f;
	LightBuffer.data.Light[1].LightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	LightBuffer.data.Light[1].Strength = 0.7f;
	LightBuffer.data.Light[1].position = XMFLOAT3(20.0f, 1000.0f, 20.0f);
	
	LightBuffer.updateConstantBuffer(deviceContext);

	return true;
}
bool Graphics::InitializeScene()
{

	camera = new Camera();
	XMFLOAT3 pos(10.f, 60.0f, 50.f);
	camera->SetPosition(pos);
	
	return true;
}

bool Graphics::ResetShaders()
{
	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->GSSetShader(NULL, nullptr, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
	return false;
}

