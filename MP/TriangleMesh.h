// TriangleMesh .h.
// Simple single triangle mesh.
#ifndef _TRIMESH_H_
#define _TRIMESH_H_


#include "BaseMesh.h"


class TriangleMesh : public BaseMesh
{

public:
	TriangleMesh(ID3D11Device* device, WCHAR* textureFilename);
	~TriangleMesh();

protected:
	void InitBuffers(ID3D11Device* device);
};

#endif