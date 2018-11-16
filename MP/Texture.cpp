// Texture.h
#include "pch.h"
#include "Texture.h"


Texture::Texture(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	// check if file exists
	if (!filename)
	{
		filename = L"../res/DefaultDiffuse.png";
	}
	// if not set default texture
	if (!does_file_exist(filename))
	{
		// change default texture
		filename = L"../res/DefaultDiffuse.png";
	}

	// check file extension for correct loading function.
	std::wstring fn(filename);
	std::string::size_type idx;
	std::wstring extension;

	idx = fn.rfind('.');

	if (idx != std::string::npos)
	{
		extension = fn.substr(idx + 1);
	}
	else
	{
		// No extension found
	}

	// Load the texture in.
	if (extension == L"dds")
	{

	/*	result = CreateDDSTextureFromFileEx(
			device,
			filename,
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ, 
			D3D11_RESOURCE_MISC_GENERATE_MIPS,
			false, 
			&m_resource,
			&m_texture); */
		result = CreateDDSTextureFromFile(device, filename, &m_resource, &m_texture, 0);
	}
	else
	{
		result = CreateWICTextureFromFile(device, filename, NULL, &m_texture, 0);
	}
	
	/*if (FAILED(result))
	{
		MessageBox(NULL, L"Texture loading error", L"ERROR", MB_OK);
	}*/
}

Texture::~Texture()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}


ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}

bool Texture::does_file_exist(const WCHAR *fname)
{
	std::ifstream infile(fname);
	return infile.good();
}