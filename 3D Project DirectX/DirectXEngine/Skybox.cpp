#include"Skybox.h"

//Public
Skybox::Skybox()
{
	Vertex cubeTemp[] =
	{ //Width for x/z, Height for y
		-SKYBOX_WIDTH,  SKYBOX_HEIGHT, -SKYBOX_WIDTH,1.0f, 
		-SKYBOX_WIDTH, -SKYBOX_HEIGHT, -SKYBOX_WIDTH,1.0f,
		 SKYBOX_WIDTH, -SKYBOX_HEIGHT, -SKYBOX_WIDTH,1.0f,
		 SKYBOX_WIDTH,  SKYBOX_HEIGHT, -SKYBOX_WIDTH,1.0f,
		-SKYBOX_WIDTH, -SKYBOX_HEIGHT,  SKYBOX_WIDTH,1.0f,
		-SKYBOX_WIDTH,  SKYBOX_HEIGHT,  SKYBOX_WIDTH,1.0f,
		 SKYBOX_WIDTH, -SKYBOX_HEIGHT,  SKYBOX_WIDTH,1.0f,
		 SKYBOX_WIDTH,  SKYBOX_HEIGHT,  SKYBOX_WIDTH,1.0f
	};

	DWORD index[] = {
		0, 1, 2, 2, 3, 0,
		4, 1, 0, 0, 5, 4,
		2, 6, 7, 7, 3, 2,
		4, 5, 7, 7, 6, 4,
		0, 3, 7, 7, 5, 0,
		1, 4, 2, 2, 4, 6
	};

	for (int i = 0; i < 8; i++)
	{
		this->vertices.push_back(cubeTemp[i]);
	}

	for (int i = 0; i < 36; i++)
	{
		this->indices.push_back(index[i]);
	}
	this->cubeSRV = nullptr;
	this->cubeTexture = nullptr;
}

Skybox::~Skybox()
{
	this->shutDown();
}

void Skybox::shutDown()
{
	for (int i = 0; i < 6; i++)
	{
		this->tgaTextures[i].Shutdown();
	}
	this->cubeTexture->Release();
	this->cubeSRV->Release();

	this->skyboxPS->Release();
	this->skyboxVS->Release();
	this->samplerState->Release();
	this->vertexLayout->Release();
	this->indexBuffer->Release();
}

bool Skybox::initialize(ID3D11Device* device)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 36;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices.data();

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer)))
	{
		return false;
	}
	if (FAILED(vertexBuffer.Initialize(device, this->vertices.data(), (UINT)this->vertices.size())))
	{
		return false;
	}
	this->matrixConstBuffer.Initialize(device);
	if (!this->initializeShaders(device))
	{
		return false;
	}
	return true;
}

bool Skybox::setTexture(ID3D11Device* device, ID3D11DeviceContext* context, std::string file)
{
	std::string fileDir = "ObjFiles/";
	std::string fileRight, fileLeft, fileUp, fileDown, fileBack, fileFront;

	fileRight = fileDir + file + "_rt.tga";
	fileLeft = fileDir + file + "_lf.tga";
	fileUp = fileDir + file + "_up.tga";
	fileDown = fileDir + file + "_dn.tga";
	fileBack = fileDir + file + "_bk.tga";
	fileFront = fileDir + file + "_ft.tga";
	
	tgaTextures[0].Initialize(device, context, fileRight.c_str());
	tgaTextures[1].Initialize(device, context, fileLeft.c_str());
	tgaTextures[2].Initialize(device, context, fileUp.c_str());
	tgaTextures[3].Initialize(device, context, fileDown.c_str());
	tgaTextures[4].Initialize(device, context, fileBack.c_str());
	tgaTextures[5].Initialize(device, context, fileFront.c_str());

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = tgaTextures[0].getWidth();//Every texture has the same width and height
	texDesc.Height = tgaTextures[0].getHeight();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 6;//6 for all the faces of a cube
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.CPUAccessFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;//TextureCube
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = texDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SRVDesc.TextureCube.MipLevels = texDesc.MipLevels;
	SRVDesc.TextureCube.MostDetailedMip = 0;

	D3D11_SUBRESOURCE_DATA pData[6];
	for (int cubeMapFaceIndex = 0; cubeMapFaceIndex < 6; cubeMapFaceIndex++)
	{
		pData[cubeMapFaceIndex].pSysMem = this->tgaTextures[cubeMapFaceIndex].getTextureArrayOfChar();
		pData[cubeMapFaceIndex].SysMemPitch = this->tgaTextures[cubeMapFaceIndex].getWidth() * 4;
		pData[cubeMapFaceIndex].SysMemSlicePitch = 0;
	}
	HRESULT hr = device->CreateTexture2D(&texDesc,
		pData, &cubeTexture);
	if (FAILED(hr))
	{
		return false;
	}
	hr = device->CreateShaderResourceView(
		cubeTexture, &SRVDesc, &cubeSRV);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool Skybox::renderSkybox(ID3D11DeviceContext* context)
{

	//Setvertexbandtexture
	UINT32 vertexSize = sizeof(Vertex);

	UINT32 offset = 0;
	context->PSSetShaderResources(0, 1, &this->cubeSRV);
	context->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), &vertexSize, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->PSSetSamplers(0, 1, &this->samplerState);

	//render shader
	context->VSSetShader(this->skyboxVS, nullptr, 0);
	context->HSSetShader(nullptr, nullptr, 0);
	context->DSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(this->skyboxPS, nullptr, 0);
	context->IASetInputLayout(this->vertexLayout);

	context->DrawIndexed(36, 0, 0);
	//36 for 6 faces with 6 vertecies for each face 6*6=36
	return true;
}

bool Skybox::setWorldMatrix(XMMATRIX world, ID3D11DeviceContext* context)
{
	world = XMMatrixTranspose(world);
	this->matrixConstBuffer.data.world = world;
	this->matrixConstBuffer.updateConstantBuffer(context);
	return true;
}

bool Skybox::setViewProjMatrix(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, ID3D11DeviceContext* context)
{
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);
	this->matrixConstBuffer.data.view = view;
	this->matrixConstBuffer.data.projection = proj;
	//need to set campos separately to enable check for backface culling
	//this->perFrameCB.data.camPos = camPos; 
	this->matrixConstBuffer.updateConstantBuffer(context);
	return true;
}

void Skybox::setCB(ID3D11DeviceContext* context)
{
	context->VSSetConstantBuffers(0, 1, this->matrixConstBuffer.getConstantBuffer());
}

//Private
bool Skybox::initializeShaders(ID3D11Device* device)
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"ResourcesFiles/SkyboxVS.hlsl", // filename vsFilename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			//OutputShaderErrorMessage(errorBlob, hwnd, vsFilename); //able when parameter active
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		else
		{
			//MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK); //able when parameter active
		}
		if (pVS)
			pVS->Release();
		return false;
	}

	device->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&skyboxVS
	);

	// create input layout (verified using vertex shader)
	// Press F1 in Visual Studio with the cursor over the datatype to jump
	// to the documentation online!
	// please read:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32A32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			D3D11_APPEND_ALIGNED_ELEMENT, // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		}

	};
	result = device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &vertexLayout);

	if (FAILED(result))
	{
		return false;
	}
	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"ResourcesFiles/SkyboxPS.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);


	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return false;
	}

	device->CreatePixelShader(
		pPS->GetBufferPointer(),
		pPS->GetBufferSize(),
		nullptr,
		&skyboxPS);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();



	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &this->samplerState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}