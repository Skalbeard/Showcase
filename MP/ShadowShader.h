// ShadowShader.h
#ifndef _SHADOWSHADER_H_
#define _SHADOWSHADER_H_


#include "BaseShader.h"
#include "Light.h"
#include "Camera.h"


class ShadowShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView;
		XMMATRIX lightProjection;
		XMMATRIX lightView2;
		XMMATRIX lightProjection2;
		XMMATRIX lightView3;
		XMMATRIX lightProjection3;
		XMMATRIX lightView4;
		XMMATRIX lightProjection4;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 diffuse2;
		XMFLOAT4 diffuse3;
		XMFLOAT4 diffuse4;
		XMFLOAT4 specularColor;
		XMFLOAT4 specularPower;
		XMFLOAT4 raw_position;
		XMFLOAT4 raw_position2;
		XMFLOAT4 raw_position3;
		XMFLOAT4 raw_position4;
	};

	struct LightBufferType2
	{
		XMFLOAT4 position;
		XMFLOAT4 position2;
		XMFLOAT4 position3;
		XMFLOAT4 position4;
	};

	struct CameraBufferType
	{
		XMFLOAT3 position;
		float padding0;
	};

public:

	ShadowShader(ID3D11Device* device, HWND hwnd);
	~ShadowShader();

	void SetShaderParameters(	ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Camera* camera,
								ID3D11ShaderResourceView*depthMap_0, Light* light_0, 
								ID3D11ShaderResourceView*depthMap_1, Light* light_1, 
								ID3D11ShaderResourceView*depthMap_2, Light* light_2, 
								ID3D11ShaderResourceView*depthMap_3, Light* light_3);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_LightBuffer;
	ID3D11Buffer* m_LightBuffer2;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleStateClamp;
};

#endif