// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

TextureCube shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState SampleType : register(s0);
SamplerState SampleTypeClamp : register(s1);


cbuffer LightBuffer : register(cb0)
{
    float4 diffuseColour;
	float4 ambientColour;
    float3 lightDirection;
	float specularPower;
	float4 specularColour;
};

struct InputType
{
    float4 position : SV_POSITION;
    float3 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float4 lightViewPosition : TEXCOORD2;
	float3 lightPos : TEXCOORD3;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float3 lightDir;
    float lightIntensity;
    float4 colour;
	float3 reflection;
	float4 specular;
	float4 finalSpec = float4(0,0,0,0);
	float bias;
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;

	// bias value for fixing float precision issues
	bias = 0.0001f;

	colour = ambientColour;
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColour = shaderTexture.Sample(SampleType, reflect(normalize(-input.viewDirection), input.normal));
//	textureColour = shaderTexture.Sample(SampleType, input.tex);
	// Invert the light direction for calculations.
    lightDir = -lightDirection;
	lightDir = normalize(lightDir);

	// Calculate the projected texture coordinates.
	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
		depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

		// Calculate the depth of the light.
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

		// Subtract the bias from the lightDepthValue.
		lightDepthValue = lightDepthValue - bias;
		// check if it's in light
		if (lightDepthValue < depthValue)
		{
			lightIntensity = saturate(dot(input.normal, lightDir));
			if (lightIntensity > 0)
			{

				reflection = reflect(normalize(lightDirection), input.normal);


				// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
				colour += saturate(diffuseColour * lightIntensity);
				specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
				finalSpec = specularColour * specular;
			}
		}
	}
    // Calculate the amount of light on this pixel.
   
	
		// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	colour = colour * textureColour;
	colour = saturate(colour + finalSpec);
	return colour;
}

