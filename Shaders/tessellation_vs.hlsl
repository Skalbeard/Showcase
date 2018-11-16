// Tessellation vertex shader.
// Doesn't do much, could manipulate the control points.
// Pass forward data, strip out some values not required for example.

cbuffer TimeBuffer : register(cb0)
{
	float time;
	float3 padding1;
}

struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
	float3 normal : NORMAL;
};

OutputType main(InputType input)
{
    OutputType output;
	float height = 1.0f;

	// Sine offset for manipulation.
	input.position.y = height * sin(input.position.x + time);

	// Modify normals accordingly.
	input.normal.x = 1 - cos(input.position.x + time);
	input.normal.y = abs(cos(input.position.x + time));

	 // Pass the vertex position into the hull shader.
    output.position = input.position;
    
    // Pass the input color into the hull shader.
    output.colour = float4(1.0, 0.0, 0.0, 1.0);
    
    return output;
}
