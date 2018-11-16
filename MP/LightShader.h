// LightShader.h
#ifndef _LIGHTSHADER_H_
#define _LIGHTSHADER_H_


#include "BaseShader.h"
#include "Light.h"
#include "Camera.h"
#include "Timer.h"


class LightShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient[4];
		XMFLOAT4 diffuse[4];
		XMFLOAT4 specular[4];
		XMFLOAT4 position[4];
		XMFLOAT4 specularPower;
	};

	struct CameraBufferType
	{
		XMFLOAT3 position;
		float padding0;
	};

public:

	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light_0, Light* light_1, Light* light_2, Light* light_3, Camera* camera);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;
};

#endif