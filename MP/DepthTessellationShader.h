// DepthTessellationShader.h
#ifndef _DEPTHTESSELATIONSHADER_H_
#define _DEPTHTESSELATIONSHADER_H_


#include "BaseShader.h"
#include "Light.h"
#include "Camera.h"


class DepthTessellationShader : public BaseShader
{
private:
	struct TessellationBufferType
	{
		float tessellationFactor;
		XMFLOAT3 padding;
	};

public:

	DepthTessellationShader(ID3D11Device* device, HWND hwnd);
	~DepthTessellationShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, float tessF);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
	void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_tessellationBuffer;
};

#endif // _DEPTHTESSELATIONSHADER_H_