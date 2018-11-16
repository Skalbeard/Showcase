// ColourShader.h
#ifndef _COLOURSHADER_H_
#define _COLOURSHADER_H_


#include "BaseShader.h"


class ColourShader : public BaseShader
{

public:

	ColourShader(ID3D11Device* device, HWND hwnd);
	~ColourShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
};

#endif