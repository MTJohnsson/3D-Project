#include "Terrain.h"
#include <iostream>
#include <fstream>
using namespace std;
Terrain::Terrain(ID3D11Device* device, ID3D11DeviceContext *deviceContext, Shader *shader, Shader *shader2)
{
	this->device = device;
	this->deviceContext = deviceContext;

	shaderPtr = shader2;


	ofstream myfile("turnamn.txt");
	if (myfile.is_open())
	{
		myfile << "This is a line.\n";
		myfile << "This is another line.\n";
		myfile.close();
	}
	else cout << "Unable to open file";


	if (!this->readFromFile())
	{
		MessageBox(NULL, "HightMap Failed.",
			"CreateVertexBuffer Error", MB_OK);
	}

	this->generateVertex();
	this->generateIndex();
	this->initiateVertex();



	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = this->vertex.size() * sizeof(Vertex3);



	D3D11_SUBRESOURCE_DATA data;
	memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &this->vertex[0];
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data,
		&vertexBuffer);
	if (FAILED(hr)){
		MessageBox(NULL, "D3D11CreateDevice Failed.",
			"CreateVertexBuffer Error", MB_OK);
	}

	hr = CreateWICTextureFromFile(device, L"textureTerrain.jpg", nullptr, texture.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());
}

void Terrain::draw()
{
	// Bind the vertex buffer
	UINT stride = sizeof(Vertex3);
	UINT offset = 0;


	//this->deviceContext->IASetIndexBuffer(indicesbuffer, DXGI_FORMAT_R32_UINT, 0);


	this->deviceContext->VSSetShader(shaderPtr->GetVertexShader()->GetShader(), NULL, 0);
	this->deviceContext->GSSetShader(shaderPtr->GetGeometryShader()->GetShader(), nullptr, 0);
	this->deviceContext->PSSetShader(shaderPtr->GetPixelShader()->GetShader(), NULL, 0);

	this->deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	this->deviceContext->Draw(this->indCount, 0);
	//deviceContext->DrawIndexed(this->indCount, 0, 0);
}

void Terrain::generateIndex()
{
	int c = 0;
	int texC = 0;

	for (int z = 0; z < this->TERR_WIDTH; z++)
	{
		for (int x = 0; x < this->TERR_LENGTH; x++)
		{
			Vertex3 v;
			v.x = x;
			v.y = float(this->yValues.at(c) * 0.002);
			v.z = z;


			if (v.y == 0)
			{
				v.r = 0.0f;
				v.g = 0.0f;
				v.b = 1.0f;

				if (texC == 0 || texC == 1)
				{
					v.coordX = 0.f;
					v.coordY = 0.f;
					texC++;
				}
				else if (texC == 2)
				{
					v.coordX = 0.f;
					v.coordY = 0.f;
					texC = 0;
				}
			}
			else
			{
				v.r = 0.0f;
				v.g = 0.5f;
				v.b = 0.0f;

				if (texC == 0)
				{
					v.coordX = 0.f;
					v.coordY = 1.f;
					texC++;
				}
				else if (texC == 1)
				{
					v.coordX = 0.f;
					v.coordY = 0.f;
					texC++;
				}
				else if (texC == 2)
				{
					v.coordX = 1.f;
					v.coordY = 0.f;
					texC = 0;
				}
			}

			this->indices.push_back(v);
			c++;
		}
	}
}

//float Terrain::intersectTerrain(XMFLOAT3 camPos, XMMATRIX world) // <-- multiply vertices with world??
//{
//	XMFLOAT3 rayDir(0.f, 1.f, 0.f);	// ray from tri towards the camera (up)
//	XMFLOAT3 v0v1(0.f, 0.f, 0.f);
//	XMFLOAT3 v0v2(0.f, 0.f, 0.f);
//	XMFLOAT3 normal(0.f, 0.f, 0.f);
//	
//	float factor = 0;	// used for different calculations, has no particular meaning
//	float t = 0.f;
//	float smallestT = 50.f;
//	bool intersect = true;
//
//
//	//int xMax = ceil(camPos.x);
//	//int xMin = floor(camPos.x);
//	//int zMax = ceil(camPos.z);
//	//int zMin = floor(camPos.z);
//
//
//	//// Triangle #1 ~ (- -) (- +) (+ +)
//	//for (int i = 0; i < this->vertex.size(); i + 3)
//	//{
//	//	if (this->vertex.at(i).x >= xMin && this->vertex.at(i).x <= xMax &&
//	//		this->vertex.at(i + 1).x >= xMin && this->vertex.at(i + 1).x <= xMax &&
//	//		this->vertex.at(i + 2).x >= xMin && this->vertex.at(i + 2).x <= xMax)
//	//	{
//	//		if (this->vertex.at(i).z >= zMin && this->vertex.at(i).z <= zMax &&
//	//			this->vertex.at(i + 1).z >= xMin && this->vertex.at(i + 1).z <= xMax &&
//	//			this->vertex.at(i + 2).z >= xMin && this->vertex.at(i + 2).z <= xMax)
//
//
//
//	//	}
//	//}
//
//
//
//	for (int i = 0; i < this->vertex.size(); i+3)
//	{
//		v0v1 = XMFLOAT3(this->vertex.at(i + 1).x - this->vertex.at(i).x, this->vertex.at(i + 1).y - this->vertex.at(i).y, this->vertex.at(i + 1).z - this->vertex.at(i).z);
//		v0v2 = XMFLOAT3(this->vertex.at(i + 2).x - this->vertex.at(i).x, this->vertex.at(i + 2).y - this->vertex.at(i).y, this->vertex.at(i + 2).z - this->vertex.at(i).z);
//		normalize(v0v1);
//		normalize(v0v2);
//
//		normal = cross(v0v1, v0v2);
//		normalize(normal);
//
//		intersect = true;
//
//
//		factor = dot(rayDir, normal);
//		if (factor > 0)	// if ray intersects with plane
//		{
//			// check if it intersects with triangle
//			cout << "Camera Position: " << std::to_string(camPos.x) << ", " << std::to_string(camPos.y) << "," << 
//				std::to_string(camPos.z) << endl;
//
//			XMFLOAT3 pvec = cross(rayDir, v0v2);
//			float det = dot(v0v1, pvec);
//
//			if (det < 0)
//			{
//				intersect = false;
//			}
//
//			float invDet = 1 / det;
//			XMFLOAT3 tvec = XMFLOAT3(camPos.x - this->vertex.at(i).x, camPos.y - this->vertex.at(i).y, camPos.z - this->vertex.at(i).z);
//			float u = dot(tvec, pvec) * invDet;
//
//			if (u < 0 || u > 1)
//			{
//				intersect = false;
//			}
//
//			XMFLOAT3 qvec = cross(tvec, v0v1);
//			float v = dot(rayDir, qvec) * invDet;
//
//			if (v < 0 || u + v > 1)
//			{
//				intersect = false;
//			}
//
//			if (intersect == true)
//			{
//				t = dot(v0v2, qvec) * invDet;
//
//				if (t != 0.f || t != 4.f)
//				{
//					//return 4.f - t;
//					if (t < smallestT)
//					{
//						smallestT = t;
//					}
//
//				}
//			}
//		}
//	}
//	return 4.f - smallestT;
//}

//float Terrain::getHeightOfTerrain(XMFLOAT3 camPos)
//{
//	float terrainX = camPos.x;
//	float terrainZ = camPos.z;
//	float gridSquareSize = 1;
//	float gridX = floor(camPos.x / gridSquareSize);
//	float gridZ = floor(camPos.z / gridSquareSize);
//
//	if (gridX >= TERR_WIDTH - 1 || gridZ >= TERR_LENGTH - 1 || gridX < 0 || gridZ < 0)
//	{
//		return 0;
//	}
//
//	float coordX = camPos.x;
//	float coordZ = camPos.z;
//}


float Terrain::getHeight(XMFLOAT3 camPos)
{
	findNearestVertecies(camPos);

	XMFLOAT3 v1;
	XMFLOAT3 v2;
	XMFLOAT3 v3;

	std::vector<float> f;

	for (int i = 0; i < 6; i++)
	{
		v1 = XMFLOAT3(nearestVertecies[i].x, nearestVertecies[i].y, nearestVertecies[i].z);
		v2 = XMFLOAT3(nearestVertecies[i + 1].x, nearestVertecies[i + 1].y, nearestVertecies[i + 1].z);
		v3 = XMFLOAT3(nearestVertecies[i + 2].x, nearestVertecies[i + 2].y, nearestVertecies[i + 2].z);

		f.emplace_back(barryCentric(v1, v2, v3, XMFLOAT2(camPos.x, camPos.z)));

		i += 2;
	}

	float terrainHeight = min(f[0], f[1]);

	return (camPos.y - terrainHeight) - 2;
}


void Terrain::generateVertex()
{
	int c = 0;
	int l = 0;

	for (int n = 0; n < this->indCount; n++)
	{
		this->vertices.push_back(0);
	}

	for (int i = 0; i < this->indCount; i += 6)
	{
		this->vertices[i] = c;
		this->vertices[i+1] = c + this->TERR_LENGTH;
		this->vertices[i+2] = c + this->TERR_LENGTH + 1;
		this->vertices[i+3] = c;
		this->vertices[i+4] = c + this->TERR_LENGTH + 1;
		this->vertices[i+5] = c + 1;

		c++;
		l++;

		if (l == this->TERR_LENGTH - 1)
		{
			c++;
			l = 0;
		}
	}
}

void Terrain::initiateVertex()
{
	int pos = 0;
	for (int i = 0; i < this->indCount; i++)
	{
		pos = this->vertices.at(i);
		this->vertex.push_back(this->indices.at(pos));
	}
}

bool Terrain::readFromFile()
{
	std::ifstream heightMap("example.txt");
	std::string y;
	std::string line;
	
	int c2 = 0;

	if (!heightMap.is_open())
	{
		return false;
	}

	if (this->TERR_WIDTH < 200)
	{
		int skip = 200 - this->TERR_WIDTH;
		int c = 0;

		if (heightMap.is_open())
		{
			//for (int i = 0; i < 80000; i++)
			//{
			//	heightMap >> line;
			//	heightMap.ignore();
			//}

			while (!heightMap.eof())
			{
				heightMap >> y;

				this->yValues.push_back(stoi(y));
				//heights[c2][c2] = stoi(y);	// test

				c2++;
				c++;
				if (c == this->TERR_WIDTH)
				{
					c = 0;
					for (int i = 0; i < skip; i++)
					{
						heightMap >> line;
						heightMap.ignore();
					}
				}
			}
			heightMap.close();
		}
	}
	else
	{
		if (heightMap.is_open())
		{
			for (int i = 0; i < 4; i++)
			{
				heightMap >> line;
				heightMap.ignore();
			}

			while (!heightMap.eof())
			{
				heightMap >> y;
				this->yValues.push_back(stoi(y));
			}
			heightMap.close();
		}
	}
	return true;
}

void Terrain::normalize(XMFLOAT3 & vector)
{
	// Normalize ~~ | a | = sqrt((ax * ax) + (ay * ay) + (az * az))
	float factor = sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = XMFLOAT3(vector.x / factor, vector.y / factor, vector.z / factor);
}

XMFLOAT3 Terrain::cross(const XMFLOAT3 & vector1, const XMFLOAT3 & vector2)
{
	// Cross Product ~~ A x B = (a2b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1)
	return XMFLOAT3(vector1.y*vector2.z - vector1.z*vector2.y, vector1.z*vector2.x - vector1.x*vector2.z, vector1.x*vector2.y - vector1.y*vector2.x);
}

float Terrain::dot(const XMFLOAT3 & vector1, const XMFLOAT3 & vector2)
{
	// Dot Product ~~ A • B = a1b1 + a2b2 + a3b3
	//return (vector1.x*vector2.x + vector1.y*vector2.y + vector1.z*vector2.z);
	return (vector1.x*vector2.x + vector1.y*vector2.y + vector1.z*vector2.z);
}

void Terrain::findNearestVertecies(XMFLOAT3 camPos)
{
	int terrX = floor(camPos.x);
	int terrZ = floor(camPos.z);

	int pos = /*(6 * (TERR_WIDTH - 1) * terrX) +*/ (6 * (TERR_WIDTH - 1) * terrZ);

	nearestVertecies.clear();

	for (int i = pos; i < pos + 6 * TERR_WIDTH; i++)
	{
		if (vertex[i].x == terrX && vertex[i].z == terrZ)
		{
			for (int j = 0; j < 6; j++)
			{
				i++;
				nearestVertecies.emplace_back(vertex[i]);
			}
			break;
		}
	}
}

float Terrain::barryCentric(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, XMFLOAT2 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;

	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

Terrain::~Terrain()
{
	//this->vertexBuffer->Release();
	if (indicesbuffer != nullptr) {
		indicesbuffer->Release();
	}

	if (vertexBuffer != nullptr) {
		vertexBuffer->Release();
	}

	this->shaderPtr = nullptr;
	delete this->shaderPtr;
}
