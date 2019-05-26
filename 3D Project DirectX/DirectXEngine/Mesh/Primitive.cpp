#include "Primitive.h"
#include <WICTextureLoader.h>

bool Primitive::translation(float x, float y, float z)
{
	XMMATRIX traspos = DirectX::XMMatrixTranslation(x, y,z);
	//traspos = DirectX::XMMatrixTranspose(traspos);
	world = traspos;
	world = DirectX::XMMatrixTranspose(world);

	return true;
}

bool Primitive::initializeCube(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	VertexPrimtives v[]{ 
		0.0f, 0.5f, 0.0f,	//v0 pos
		1.f, 0.0f, 0.0f,

		0.5f, -0.5f, 0.0f,	//v1
		1.f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.0f, //v2
		1.f, 0.0f, 0.0f
		 // -0.5f, 0.5f, 0.0f,	//v0 frontTopLeft
		 //1.f, 0.0f, 0.0f,
		 // 0.f, 0.0f, 0.0f,

		 // 0.5f, -0.5f, 0.0f,	//v1 frontBottomRight
		 //0.f, 1.0f, 0.0f,	//v1 color
		 //0.f, 0.0f, 0.0f,

		 // -0.5f, -0.5f, 0.0f, //v2 frontBottomleft
		 //0.f, 1.0f, 0.0f,	//v1 color
		 //0.f, 0.0f, 0.0f,

		 // 0.5f,0.5f, 0.0f,		//v3 frontTopRight
		 //  0.f, 1.0f, 0.0f,	//v1 color
		 //0.f, 0.0f, 0.0f,

		 // -0.5f, 0.5f,-1.0f,	//w4  backTopLeft
		 //0.f, 1.0f, 0.0f,	//v1 color
		 //0.f, 0.0f, 0.0f,

		 // 0.5f, -0.5f,-1.0f,	//w5  backBottomRight
		 //0.f, 1.0f, 0.0f,	//v1 color
		 //0.f, 0.0f, 0.0f,

		 // -0.5f, -0.5f,-1.0f, //w6  backBottomleft
		 //0.f, 1.0f, 0.0f,	//v1 color
		 //0.f, 0.0f, 0.0f,

		 // 0.5f,0.5f, -1.0f,	//w7  backTopRight
		 //  0.f, 1.0f, 0.0f,	//v1 color
		 //0.f, 0.0f, 0.0f,
	};
	
	//DWORD indices[]{
	//	0,3,2,	//side 1
	//	3,1,2,
	//	4,0,6, //sida 2
	//	6,0,2,
	//	5,7,6,	//sida 3
	//	6,7,4,
	//	1,3,5,   // side 4
	//	5,3,7,
	//	4,7,0, //sida 5
	//	0,7,3,
	//	1,5,2, // side 6
	//	2,5,6,
	//};

	/*indice = sizeof(indices)/sizeof(indices[0]);*/
	vertexBuffer.Initialize(device, &v[0], sizeof(*v));

	//D3D11_BUFFER_DESC indexdesc;
	//memset(&indexdesc, 0, sizeof(indexdesc));
	//indexdesc.Usage = D3D11_USAGE_DEFAULT;
	//indexdesc.ByteWidth = sizeof(DWORD) * sizeof(indices);
	//indexdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexdesc.CPUAccessFlags = 0;
	//indexdesc.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA indexData;
	//memset(&indexData, 0, sizeof(indexData));
	//indexData.pSysMem = &indices[0];
	//HRESULT hr = device->CreateBuffer(&indexdesc, &indexData, indicesbuffer.GetAddressOf());
	//if (FAILED(hr)) {
	//	MessageBox(NULL, "index Failed.",
	//		"CreateBuffer Initialisation Error", MB_OK);
	//	return false;
	//}

	/*hr = DirectX::CreateWICTextureFromFile(device, L"default_texture.png", nullptr, myTexture.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(NULL, "Textures Failed.",
			"Textures Error", MB_OK);
		return false;
	}*/
	Texture tex;
	tex.LoadDefaultTexture(device, DEFAULT);
	this->texture.push_back(tex);
	return true;
}

void Primitive::draw(ID3D11DeviceContext * deviceContext)
{
	// Bind the vertex buffer
	UINT stride = sizeof(VertexPrimtives);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	//deviceContext->PSSetShaderResources(0, 1, texture[0].getTextureView());
	//deviceContext->IASetIndexBuffer(indicesbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//deviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);
	deviceContext->Draw(3, 0);	
}

Primitive::~Primitive()
{
}
