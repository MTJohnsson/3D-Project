#include "Lib/LoadObj.h"
#include <fstream>
#include <iostream>
#include <iomanip>  
#include <sstream>


Vertex LoadObj::setFaces(std::string  f)
{
	Vertex index;
	int sPos = 0;
	int ePos = sPos;
	string temp;
	sPos = 0;
	ePos = sPos;
	ePos = f.find_first_of("/");
	temp = f.substr(sPos, ePos - sPos);
	index.Position = position.at(atof(temp.c_str()) - 1);

	sPos = ePos + 1;
	ePos = f.find("/", sPos);
	temp = f.substr(sPos, ePos - sPos);
	index.TextCoord = TexCoord.at(atof(temp.c_str()) - 1);

	sPos = ePos + 1;
	ePos = f.length();
	temp = f.substr(sPos, ePos - sPos);
	index.Normal = Normal.at(atof(temp.c_str()) - 1);

	return index;
}

XMFLOAT3 color = XMFLOAT3(1.0f, 1.0f, 1.0f);
Vertex LoadObj::setFaces2(std::string f)
{
	Vertex index;
	int sPos = 0;
	int ePos = sPos;
	string temp;
	sPos = 0;
	ePos = sPos;
	ePos = f.find_first_of("/");
	temp = f.substr(sPos, ePos - sPos);
	index.Position = position.at(atof(temp.c_str()) - 1);

	sPos = ePos + 2;
	ePos = f.find("/", sPos);
	temp = f.substr(sPos, ePos - sPos);
	index.Normal = Normal.at(atof(temp.c_str()) - 1);
	//index.TextCoord = TexCoordArray.at(atof(temp.c_str()) - 1);

	/*sPos = ePos + 1;
	ePos = f.length();
	temp = f.substr(sPos, ePos - sPos);
	index.Color = color;*/

	return index;
}

bool LoadObj::setTextureType(textureType type, std::string filepath)
{
	Texture texture;
	texture.LoadTextureFromFile(device, filepath, type);
	textures.push_back(texture);
	return false;
}
bool LoadObj::readMtlFile(string mtlName)
{
	std::string filePath = "ObjFiles\\";
	ifstream input2(filePath+mtlName); //ex boat.mtl
	string buffer;
	if (!input2.is_open()) {
		return false;
	}
	while (!input2.eof())
	{
		getline(input2, buffer);
		istringstream line(buffer);
		string temp;
		string f1;

		if (buffer.substr(0, 6) == "map_Kd") {
			line >> temp >> f1;
			
			setTextureType(DIFFUSE, filePath+f1);
		}
		else if (buffer.substr(0, 4) == "bump") {
			line >> temp >> f1;
			//load textures
			setTextureType(NORMAL, filePath+f1);
		}
	}
	input2.close();
}

XMFLOAT3 LoadObj::ConvertString(string &f1, string &f2, string& f3)
{
	XMFLOAT3 vertex;
	vertex.x = atof(f1.c_str());
	vertex.y = atof(f2.c_str());
	vertex.z = atof(f3.c_str());

	if (sphere.Max.x < vertex.x)
		sphere.Max.x = vertex.x;
	if(sphere.Max.y < vertex.y)
		sphere.Max.y = vertex.y;

	if (sphere.Min.x > vertex.x)
		sphere.Min.x = vertex.x;
	if (sphere.Min.y > vertex.y)
		sphere.Min.y = vertex.y;
	return vertex;
}
XMFLOAT2 LoadObj::ConvertString(string & f1, string & f2)
{
	XMFLOAT2 texture;
	texture.x = atof(f1.c_str());
	texture.y = 1 - atof(f2.c_str());

	return texture;
}
bool LoadObj::loadOBJ(std::string& file)
{
	ifstream input(file);
	string buffer;
	Vertex in = {};

	if (!input.is_open()) {
		return false;
	}
	while (!input.eof())
	{
		getline(input, buffer);
		istringstream line(buffer);

		string temp;
		Vertex index, ex1, ex2, ex3;
		string f1, f2, f3, f4;
		if (buffer.substr(0, 6) == "mtllib") {
			line >> temp >> f1; //f1 fins the mtl file
			readMtlFile(f1);
		}
		else if (buffer.substr(0, 6) == "usemtl") {
			line >> temp >> f1;
		}
		else if (buffer.substr(0, 2) == "v ") {
			line >> temp >> f1 >> f2 >> f3;
			XMFLOAT3 vertex = ConvertString(f1, f2, f3);
			position.push_back(vertex);
		}
		else if (buffer.substr(0, 2) == "vn") {
			line >> temp >> f1 >> f2 >> f3;
			XMFLOAT3 normal = ConvertString(f1, f2, f3);
			Normal.push_back(normal);
		}
		else if (buffer.substr(0, 2) == "vt") {
			line >> temp >> f1 >> f2 >> f3;
			XMFLOAT2 texture = ConvertString(f1, f2);
			TexCoord.push_back(texture);
		}
		else if (buffer.substr(0, 1) == "f") {
			int counter = 0;
			for (int i = 0; i < buffer.size(); i++)
			{
				if (buffer[i] == '/') {
					counter++;
				}
			}
			//check number of faces
			//if 8 means 4 faces else 3 faces
			bool found = false;
			if (this->TexCoord.size() == 0) {
				found = true;
			}
			if (counter == 8) {
				line >> temp >> f1 >> f2 >> f3 >> f4;
				counter = 4;
			}
			else {
				line >> temp >> f1 >> f2 >> f3;
				counter = 3;
			}
			for (int i = 0; i < counter; i++)
			{
				if (0 == i) {
					if (found)
						ex1 = setFaces2(f1);
					else
						ex1 = setFaces(f1);
					vertices.push_back(ex1);
				}
				else if (1 == i) {
					if (found)
						ex2 = setFaces2(f2);
					else
						ex2 = setFaces(f2);

					vertices.push_back(ex2);
				}if (2 == i) {
					if (found)
						ex3 = setFaces2(f3);
					else
						ex3 = setFaces(f3);

					vertices.push_back(ex3);
				}if (3 == i) {//4th face, make 2 more verticies
					if (found)
						index = setFaces2(f4);
					else
						index = setFaces(f4);

					vertices.push_back(index);
					vertices.push_back(ex1);
					vertices.push_back(ex3);
				}

			}
			numberOfTriangles++;
		}
	}
	input.close();

}
bool LoadObj::Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, string file)
{
	this->device = device;
	loadOBJ(file);
	HRESULT hr = vertexBuffer.Initialize(device, vertices.data(), vertices.size());
	if (FAILED(hr))
	{
		MessageBox(NULL, "vertexBuffer  loadOBJ Failed.",
			"Create buffer Error", MB_OK);
		return false;
	}
	//add texture if there are non
	if (textures.size() == NULL) {
		Texture tex;
		//tex.LoadTextureFromFile(device,"SciDiffuse.tga",DIFFUSE)
		tex.LoadDefaultTexture(device, DEFAULT);
		textures.push_back(tex);
	}
	sphere.calculateRadius();
	return true;
}

LoadObj::~LoadObj()
{
}

void Sphere::calculateRadius()
{
	this->radius = abs(Max.x - Min.x);
	this->radius  = radius/ 2;
}
