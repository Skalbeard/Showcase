// PointMesh.h
#ifndef _POINTMESH_H_
#define _POINTMESH_H_


#include "BaseMesh.h"


class PointMesh : public BaseMesh
{

public:
	PointMesh(ID3D11Device* device, WCHAR* textureFilename);
	~PointMesh();

	void SendData(ID3D11DeviceContext*);

protected:
	void InitBuffers(ID3D11Device* device);

};

#endif