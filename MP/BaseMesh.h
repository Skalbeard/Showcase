// BaseMesh.h
// Base mesh class to be inherited by all the actual meshes.
#ifndef _BASEMESH_H_
#define _BASEMESH_H_


#include "Texture.h"


class BaseMesh
{
protected:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}

	BaseMesh();
	~BaseMesh();

	virtual void SendData(ID3D11DeviceContext*);
	virtual void SendTessellationData(ID3D11DeviceContext*);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	void Rotate(float radians_x, float radians_y, float radians_z);

protected:
	virtual void InitBuffers(ID3D11Device*) = 0;
	void LoadTexture(ID3D11Device*, WCHAR*);

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
};

#endif