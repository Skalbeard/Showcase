// QuadMesh.h
// Simple quad mesh class.
#ifndef _QMESH_H_
#define _QMESH_H_


#include "BaseMesh.h"


class QuadMesh : public BaseMesh
{
public:
	QuadMesh(ID3D11Device* device, WCHAR* textureFilename);
	~QuadMesh();

protected:
	void InitBuffers(ID3D11Device* device);
};

#endif