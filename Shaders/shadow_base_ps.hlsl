Texture2D depthMapTexture : register(t0);

SamplerState SampleTypeWrap : register(s0);
SamplerState SampleTypeClamp : register(s1);

cbuffer LightBuffer : register (cb0)
{
	float4 raw_position;
}

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
	float3 position3D: TEXCOORD3;
};

float4 main(InputType input) : SV_TARGET
{
	float4 colour;
	float2 projectTexCoord;
	float depthValue;
	float bias;
	float lightDepthValue;
	float lightIntensity;

	float3 lightDir;
	float distance;
	float range = 24.0f;

	// Set the bias value for fixing the floating point precision issues.
	bias = 0.0005f;

	// Calculate the projected texture coordinates.
	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	// Set the light direction for calculations.
	lightDir = input.position3D.xyz - raw_position.xyz;

	// And if we determine a pixel is illuminated then it is coloured pure white.
	colour = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// Calculate distance.
	distance = length(lightDir);

	// If within range.
	if (distance < range)
	{
		// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
		if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
		{
			// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
			// Set to wrapping sampler state because of how it's set up further down.
			depthValue = depthMapTexture.Sample(SampleTypeWrap, projectTexCoord).r;

			// Calculate the depth of the light.
			lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

			// Subtract the bias from the lightDepthValue.
			lightDepthValue = lightDepthValue - bias;

			// Calculate the amount of light on this pixel.
			lightIntensity = saturate(dot(input.normal, input.lightPos));

			// If this pixel is illuminated then set it to pure white (non-shadow).
			if (lightIntensity > 0.0f)
			{
				// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
				// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
				if (lightDepthValue > depthValue)
				{
					// Set the output colour to be black (shadow).
					colour = float4(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}	
		}
	}

	// This will return a black and white image of the shadowed scene so we can use it as input to the blurring shader.
	return colour;
}