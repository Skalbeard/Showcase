// Texture.h
#ifndef _TEXTURE_H_
#define _TEXTURE_H_


class Texture
{
public:
	Texture(ID3D11Device* device, WCHAR* filename);
	~Texture();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool does_file_exist(const WCHAR *fileName);

	ID3D11Resource* m_resource;
	ID3D11ShaderResourceView* m_texture;
};

#endif