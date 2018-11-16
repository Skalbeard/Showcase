// Tessellation domain shader.
// After tessellation the domain shader processes the all the vertices.

cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer : register(cb1)
{
	float3 cameraPosition;
	float padding;
};

struct ConstantOutputType
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

struct InputType
{
	float3 position : POSITION;
	float3 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float3 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection: TEXCOORD1;
};

[domain("tri")]

OutputType main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 3> patch)
{
	float3 vertexPosition;
	float3 vertexNormal;
	OutputType output;

	// Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
	vertexPosition = uvwCoord.x * patch[0].position + -uvwCoord.y * patch[1].position + -uvwCoord.z * patch[2].position;
	vertexNormal = uvwCoord.x * patch[0].normal + -uvwCoord.y * patch[1].normal + -uvwCoord.z * patch[2].position;
	// Calculate the position of the new vertex against the world, view, and projection matrices.
	output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Send the input color into the pixel shader.
	output.tex = vertexPosition;
	output.normal = mul(vertexNormal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);
	float4 worldPosition;
	worldPosition = mul(vertexPosition, worldMatrix);
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);
	
	return output;
}

