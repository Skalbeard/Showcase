// ShadowBaseShader.h
#ifndef _SHADOW_BASE_SHADER_H_
#define _SHADOW_BASE_SHADER_H_


#include "BaseShader.h"
#include "Light.h"


class ShadowBaseShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView;
		XMMATRIX lightProjection;
	};

	struct LightBufferType
	{
		XMFLOAT4 raw_position;
	};

	struct LightBufferType2
	{
		XMFLOAT4 position;
	};

public:
	ShadowBaseShader(ID3D11Device* device, HWND hwnd);
	~ShadowBaseShader();

	void SetShaderParameters(	ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture,
								ID3D11ShaderResourceView* depthMap, Light* light);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_LightBuffer;
	ID3D11Buffer* m_LightBuffer2;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleStateClamp;
};

#endif