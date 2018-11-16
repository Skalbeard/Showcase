// ShadowShader.cpp
#include "pch.h"
#include "ShadowShader.h"


ShadowShader::ShadowShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	InitShader(L"../shaders/shadow_vs.hlsl", L"../shaders/shadow_ps.hlsl");
}


ShadowShader::~ShadowShader()
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
	if (m_LightBuffer)
	{
		m_LightBuffer->Release();
		m_LightBuffer = 0;
	}
	// Release the light constant buffer.
	if (m_LightBuffer2)
	{
		m_LightBuffer2->Release();
		m_LightBuffer2 = 0;
	}

	// Release the camera buffer.
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
	}

	// Release base shader components.
	BaseShader::~BaseShader();
}


void ShadowShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc2;
	D3D11_BUFFER_DESC cameraBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);


	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);


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

	// Required a CLAMPED sampler for sampling the depth map
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	// Create the texture sampler state.
	m_device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);


	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&lightBufferDesc, NULL, &m_LightBuffer);


	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	lightBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc2.ByteWidth = sizeof(LightBufferType2);
	lightBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc2.MiscFlags = 0;
	lightBufferDesc2.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&lightBufferDesc2, NULL, &m_LightBuffer2);


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


void ShadowShader::SetShaderParameters(	ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Camera* camera,
										ID3D11ShaderResourceView*depthMap_0, Light* light_0,
										ID3D11ShaderResourceView*depthMap_1, Light* light_1,
										ID3D11ShaderResourceView*depthMap_2, Light* light_2,
										ID3D11ShaderResourceView*depthMap_3, Light* light_3)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* lightPtr;
	LightBufferType2* lightPtr2;
	CameraBufferType* camPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj,	tLightViewMatrix, tLightProjectionMatrix, tLightViewMatrix2, tLightProjectionMatrix2,
									tLightViewMatrix3, tLightProjectionMatrix3, tLightViewMatrix4, tLightProjectionMatrix4;


	///***MATRICES***///
	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	tLightViewMatrix = XMMatrixTranspose(light_0->GetViewMatrix());
	tLightProjectionMatrix = XMMatrixTranspose(light_0->GetProjectionMatrix());
	tLightViewMatrix2 = XMMatrixTranspose(light_1->GetViewMatrix());
	tLightProjectionMatrix2 = XMMatrixTranspose(light_1->GetProjectionMatrix());
	tLightViewMatrix3 = XMMatrixTranspose(light_2->GetViewMatrix());
	tLightProjectionMatrix3 = XMMatrixTranspose(light_2->GetProjectionMatrix());
	tLightViewMatrix4 = XMMatrixTranspose(light_3->GetViewMatrix());
	tLightProjectionMatrix4 = XMMatrixTranspose(light_3->GetProjectionMatrix());

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	dataPtr->lightView = tLightViewMatrix;
	dataPtr->lightProjection = tLightProjectionMatrix;
	dataPtr->lightView2 = tLightViewMatrix2;
	dataPtr->lightProjection2 = tLightProjectionMatrix2;
	dataPtr->lightView3 = tLightViewMatrix3;
	dataPtr->lightProjection3 = tLightProjectionMatrix3;
	dataPtr->lightView4 = tLightViewMatrix4;
	dataPtr->lightProjection4 = tLightProjectionMatrix4;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	///***MATRICES***///


	///***CAMERA***///
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
	bufferNumber = 2;
	// Update the constant buffer.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
	///***CAMERA***///


	///***LIGHTS***///
	// Send light data to pixel shader.
	deviceContext->Map(m_LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	lightPtr->ambient = light_0->GetAmbientColour();
	lightPtr->diffuse = light_0->GetDiffuseColour();
	lightPtr->diffuse2 = light_1->GetDiffuseColour();
	lightPtr->diffuse3 = light_2->GetDiffuseColour();
	lightPtr->diffuse4 = light_3->GetDiffuseColour();
	lightPtr->specularColor = light_0->GetSpecularColour();
	lightPtr->specularPower.x = light_0->GetSpecularPower();
	lightPtr->specularPower.y = light_1->GetSpecularPower();
	lightPtr->specularPower.z = light_2->GetSpecularPower();
	lightPtr->specularPower.w = light_3->GetSpecularPower();
	lightPtr->raw_position = light_0->GetPosition();
	lightPtr->raw_position2 = light_1->GetPosition();
	lightPtr->raw_position3 = light_2->GetPosition();
	lightPtr->raw_position4 = light_3->GetPosition();
	deviceContext->Unmap(m_LightBuffer, 0);
	bufferNumber = 0;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_LightBuffer);


	// Send light data to vertex shader
	deviceContext->Map(m_LightBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr2 = (LightBufferType2*)mappedResource.pData;
	lightPtr2->position = light_0->GetPosition();
	lightPtr2->position2 = light_1->GetPosition();
	lightPtr2->position3 = light_2->GetPosition();
	lightPtr2->position4 = light_3->GetPosition();
	deviceContext->Unmap(m_LightBuffer2, 0);
	bufferNumber = 1;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_LightBuffer2);
	///***LIGHTS***///


	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	// Set shader depth map texture resources.
	deviceContext->PSSetShaderResources(1, 1, &depthMap_0);
	deviceContext->PSSetShaderResources(2, 1, &depthMap_1);
	deviceContext->PSSetShaderResources(3, 1, &depthMap_2);
	deviceContext->PSSetShaderResources(4, 1, &depthMap_3);
}

void ShadowShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	deviceContext->PSSetSamplers(1, 1, &m_sampleStateClamp);

	// Base render function.
	BaseShader::Render(deviceContext, indexCount);
}