// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

TextureCube texCube: register(t0);
SamplerState SampleType : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float3 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
	return texCube.Sample(SampleType, input.tex);
}