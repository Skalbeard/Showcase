
cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix;
	matrix lightProjectionMatrix;
	matrix lightViewMatrix2;
	matrix lightProjectionMatrix2;
	matrix lightViewMatrix3;
	matrix lightProjectionMatrix3;
	matrix lightViewMatrix4;
	matrix lightProjectionMatrix4;
};

cbuffer LightBuffer2 : register(cb1)
{
    float4 lightPosition;
	float4 lightPosition2;
	float4 lightPosition3;
	float4 lightPosition4;
};

cbuffer CameraBuffer : register(cb2)
{
	float3 cameraPosition;
	float padding0;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
	float4 lightViewPosition2 : TEXCOORD3;
	float3 lightPos2 : TEXCOORD4;
	float4 lightViewPosition3 : TEXCOORD5;
	float3 lightPos3 : TEXCOORD6;
	float4 lightViewPosition4 : TEXCOORD7;
	float3 lightPos4 : TEXCOORD8;
	float3 viewDirection : TEXCOORD9;
	float3 position3D : TEXCOORD10;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};


OutputType main(InputType input)
{
    OutputType output;
	float4 worldPosition;
        
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
    
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Calculate the position of the vertex in the world.
	worldPosition = mul(input.position, worldMatrix);

	// Store the position of the vertice as viewed by the camera in a separate variable.
	output.lightViewPosition = output.position;
	output.lightViewPosition2 = output.position;
	output.lightViewPosition3 = output.position;
	output.lightViewPosition4 = output.position;

	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// For each light.
	// Determine the light position based on the position of the light 1 and the position of the vertex in the world.
	output.lightPos = lightPosition.xyz - worldPosition.xyz;

	// Normalize the light position vector.
	output.lightPos = normalize(output.lightPos);

	// Determine the light position based on the position of the light 2 and the position of the vertex in the world.
	output.lightPos2 = lightPosition2.xyz - worldPosition.xyz;

	// Normalize the light position vector.
	output.lightPos2 = normalize(output.lightPos2);

	// Determine the light position based on the position of the light 3 and the position of the vertex in the world.
	output.lightPos3 = lightPosition3.xyz - worldPosition.xyz;

	// Normalize the light position vector.
	output.lightPos3 = normalize(output.lightPos3);

	// Determine the light position based on the position of the light 4 and the position of the vertex in the world.
	output.lightPos4 = lightPosition4.xyz - worldPosition.xyz;

	// Normalize the light position vector.
	output.lightPos4 = normalize(output.lightPos4);

	// World position of vertex for the output.
	output.position3D = mul(input.position, worldMatrix);

	// Calculate the tangent vector against the world matrix only and then normalize the final value.
	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	// Calculate the binormal vector against the world matrix only and then normalize the final value.
	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);


	return output;
}