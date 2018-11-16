// BoxShader.h
#ifndef _BOX_SHADER_H_
#define _BOX_SHADER_H_


#include "BaseShader.h"


class BoxShader : public BaseShader
{
private:
	struct ScreenSizeBufferType
	{
		int screenWidth;
		int screenHeight;
		XMFLOAT2 padding;
	};

public:

	BoxShader(ID3D11Device* device, HWND hwnd);
	~BoxShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, int width, int height);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_ScreenSizeBuffer;
};

#endif