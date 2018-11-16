// LightShader.h
#include "pch.h"
#include "LightShader.h"


LightShader::LightShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	InitShader(L"../shaders/light_vs.hlsl", L"../shaders/light_ps.hlsl");
}


LightShader::~LightShader()
{
	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the light constant buffer.
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the camera buffer.
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
	}

	//Release base shader components.
	BaseShader::~BaseShader();
}


void LightShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;

	// Load (+ compile) shader files.
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Create the texture sampler state.
	m_device->CreateSamplerState(&samplerDesc, &m_sampleState);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	// Setup light buffer.
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);

	// Setup camera buffer.
	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;
	// Create the constant camera buffer pointer.
	m_device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
}

void LightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Light* light_0, Light* light_1, Light* light_2, Light* light_3, Camera* camera)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* lightPtr;
	CameraBufferType* camPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;

	/***VIEW/WORLD MATRIX***/
	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;
	dataPtr->view = tview;
	dataPtr->projection = tproj;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	/***VIEW/WORLD MATRIX***/


	/***CAMERA***/
	// Lock onto the camera buffer.
	deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// Get a pointer to the data in the constant buffer.
	camPtr = (CameraBufferType*)mappedResource.pData;
	// Copy the camera position into the constant buffer from the locked in camera buffer.
	camPtr->position = camera->GetPosition();
	camPtr->padding0 = 0.0f;
	// Unlock the buffer.
	deviceContext->Unmap(m_cameraBuffer, 0);
	// Reset the buffer register (to the buffer register I need).
	bufferNumber = 1;
	// Update the constant buffer.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
	/***CAMERA***/


	/***MATERIAL***/

	/***MATERIAL***/


	/***LIGHT***/
	// Send each light data to pixel shader.
	deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;

	lightPtr->ambient[0] = light_0->GetAmbientColour();
	lightPtr->ambient[1] = light_1->GetAmbientColour();
	lightPtr->ambient[2] = light_2->GetAmbientColour();
	lightPtr->ambient[3] = light_3->GetAmbientColour();

	lightPtr->diffuse[0] = light_0->GetDiffuseColour();
	lightPtr->diffuse[1] = light_1->GetDiffuseColour();
	lightPtr->diffuse[2] = light_2->GetDiffuseColour();
	lightPtr->diffuse[3] = light_3->GetDiffuseColour();

	lightPtr->specular[0] = light_0->GetSpecularColour();
	lightPtr->specular[1] = light_1->GetSpecularColour();
	lightPtr->specular[2] = light_2->GetSpecularColour();
	lightPtr->specular[3] = light_3->GetSpecularColour();

	lightPtr->position[0] = light_0->GetPosition();
	lightPtr->position[1] = light_1->GetPosition();
	lightPtr->position[2] = light_2->GetPosition();
	lightPtr->position[3] = light_3->GetPosition();

	lightPtr->specularPower.x = light_0->GetSpecularPower();
	lightPtr->specularPower.y = light_1->GetSpecularPower();
	lightPtr->specularPower.z = light_2->GetSpecularPower();
	lightPtr->specularPower.w = light_3->GetSpecularPower();

	// Unlock the buffer.
	deviceContext->Unmap(m_lightBuffer, 0);

	// Set buffer number (register value).
	bufferNumber = 0;

	// Update the constant buffer in the pixel shader.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);
	/***LIGHT***/


	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
}

void LightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Base render function.
	BaseShader::Render(deviceContext, indexCount);
}