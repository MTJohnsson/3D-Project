#ifndef TGATEXTURE_H
#define TGATEXTURE_H
#include <d3d11.h>
#include <stdio.h>
#include <fstream>

class TGATexture
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	bool LoadTarga(const char*, int&, int&);

	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;

	//TO STORE WIDTH & HEIGHT
	int width;
	int height;

	bool transparent;
public:
	TGATexture();
	TGATexture(const TGATexture&);
	~TGATexture();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const char*, int mipLevels = -1);
	void Shutdown();

	ID3D11ShaderResourceView*& GetTexture();

	bool isTransparent();

	unsigned short getWidth();
	unsigned short getHeight();
	unsigned char* getTextureArrayOfChar();
};

#endif