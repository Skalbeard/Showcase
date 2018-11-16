// Light pixel shader
// Calculate colour using ambient, diffuse, specular, texturing for a single light.

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

// The order of the data in this struct must match the one in the shader class.
cbuffer LightBuffer : register(cb0)
{								// 16 byte	REGISTERS.//Each float4 takes up a 16 byte register
	float4 ambientColour[4];	// 16	16	16	16		so there is four 16 byte registers filled
	float4 diffuseColour[4];	// 16	16	16	16		up per an array of size [4].
	float4 specularColour[4];	// 16	16	16	16
	float4 position[4];			// 16	16	16	16
	float4 specularPower;		// 4	4	4	4
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float3 position3D : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{
	int numOfLights = 4;
	float4 textureColour;
	float3 lightDir;
	float lightIntensity;
	float4 colour =			{ 0.0f, 0.0f, 0.0f, 0.0f };
	float3 reflection;
	float4 specular;
	float4 finalSpec =		{ 0.0f, 0.0f, 0.0f, 0.0f };
	float4 tempColour[4] = {{ 0.0f, 0.0f, 0.0f, 0.0f },
							{ 0.0f, 0.0f, 0.0f, 0.0f },
							{ 0.0f, 0.0f, 0.0f, 0.0f },
							{ 0.0f, 0.0f, 0.0f, 0.0f }};
	float constantFact = 1.0f;
	float linearFact = 0.18f;
	float quadraticFact = 0.009f;
	float distance;
	float attenuation;
	float luminosity;
	float range = 50.0f;
	float RangeQ;


	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColour = shaderTexture.Sample(SampleType, input.tex);

	for (int i = 0; i < numOfLights; i++)
	{
		// Set the light direction for calculations.
		lightDir = input.position3D.xyz - position[i].xyz;

		// Calculate distance.
		distance = length(lightDir);

		// If within range.
		if (distance < range)
		{
			// Normalize the light vector.
			lightDir = normalize(lightDir);

			// Calculate the amount of light on this pixel/diffuse colour calculation.
			lightIntensity = saturate(dot(input.normal, -lightDir));

			// Range quefficient calculated from range minus distance vector.
			RangeQ = length(range-distance)/length(range);

			// If the relative range is above zero
			if (lightIntensity > 0.0f)
			{
				// Calculate reflection vector based on the light intensity, normal vector and light direction.
				reflection = normalize(reflect(lightDir, input.normal));

				// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
				specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower[i]);

				// Sum up the specular light colour.
				finalSpec = specularColour[i] * specular;

				// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity and add specular into the attenuation field.
				tempColour[i] += saturate(((diffuseColour[i] * lightIntensity) + finalSpec) * RangeQ);

				// Calculate the attenuation value.
				attenuation = constantFact + (linearFact * distance) + (quadraticFact * distance);

				// Luminosity
				luminosity = 4.0f / attenuation;

				// Add the attenuation calculation to the colour.
				tempColour[i] = tempColour[i] * luminosity;
			}
		}
	}

	// Get the final colour from the temporary colour array.
	colour = tempColour[0] + tempColour[1] + tempColour[2] + tempColour[3];

	// Add the ambient and texture colour.
	colour = colour * textureColour;

	

	// Return final colour.
	return colour;
}