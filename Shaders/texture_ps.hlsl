// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColor;
	float4 tempColor;
	
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    tempColor = texture0.Sample(Sampler0, input.tex);

	// LERP the texture with colours
	textureColor = tempColor;
	//textureColor = float4(1.0 - tempColor.rgb, 1.0);
	
    return textureColor;
}