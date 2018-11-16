// SphereMesh.h
#ifndef _SPHEREMESH_H_
#define _SPHEREMESH_H_


#include "BaseMesh.h"


class SphereMesh : public BaseMesh
{

public:
	SphereMesh(ID3D11Device* device, WCHAR* textureFilename, int resolution = 20, bool reverse = false);
	~SphereMesh();

	void SetPosition(float, float, float);

protected:
	void InitBuffers(ID3D11Device* device);
	int m_resolution;
	bool m_reverse;

private:
	XMVECTOR m_position;
};

#endif