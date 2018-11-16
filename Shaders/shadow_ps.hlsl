// Shadows with blurring.
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);
Texture2D depthMapTexture2 : register(t2);
Texture2D depthMapTexture3 : register(t3);
Texture2D depthMapTexture4 : register(t4);
Texture2D normalMapTexture : register(t5);

SamplerState SampleTypeWrap  : register(s0);
SamplerState SampleTypeClamp : register(s1);

cbuffer LightBuffer : register(cb0)
{
	float4 ambientColor;
	float4 diffuseColor;
	float4 diffuseColor2;
	float4 diffuseColor3;
	float4 diffuseColor4;
	float4 specularColor;
	float4 specularPower;
	float4 raw_position;
	float4 raw_position2;
	float4 raw_position3;
	float4 raw_position4;
};

struct InputType
{
    float4 position : SV_POSITION;			// Screenspace.
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
	float3 position3D : TEXCOORD10;			// Scenespace.
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};


float4 main(InputType input) : SV_TARGET
{
	// Colour.
	float4 color;
	float4 textureColor;
	float4 tempColour;
	float4 tempColour2;
	float4 tempColour3;
	float4 tempColour4;

	// Shadows.
	float shadowVal;
	float2 projectTexCoord;
	float smapWidth;
	float smapHeight;
	float width, height;
	float borderInset = 2.0f;

    // Light.
	float lightIntensity;
	float3 lightDir;
	float distance;
	float attenuation;
	float luminosity;
	float RangeQ;
	float range = 24.0f;
	float constantFact = 1.0f;
	float linearFact = 0.18f;
	float quadraticFact = 0.009f;

	// Specular.
	float4 specular;
	float4 finalSpec;
	float3 reflection;

	// Bumpmapping.
	float4 bumpMap;
	float3 bumpNormal;

	// Set the default output color to the ambient light value for all pixels.
	tempColour = ambientColor;
	tempColour2 = ambientColor;
	tempColour3 = ambientColor;
	tempColour4 = ambientColor;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);
	
	///***LIGHT 1***///
	// Set the light direction for calculations.
	lightDir = input.position3D.xyz - raw_position.xyz;

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
		RangeQ = length(range - distance) / length(range);

		if (lightIntensity > 0.0f)
		{
			// Calculate reflection vector based on the light intensity, normal vector and light direction.
			reflection = normalize(reflect(lightDir, input.normal));

			// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
			specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower.x);

			// Sum up the specular light colour.
			finalSpec = specularColor * specular;

			// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity and add specular into the attenuation field.
			tempColour += saturate(((diffuseColor * lightIntensity) + finalSpec) * RangeQ);

			// Calculate the attenuation value.
			attenuation = constantFact + (linearFact * distance) + (quadraticFact * distance);

			// Luminosity.
			luminosity = 4.0f / attenuation;

			// Calculate the projected texture coordinates.
			projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
			projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

			// Sample the shadow value from the shadow texture using the sampler at the projected texture coordinate location.
			shadowVal = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

			// Add the attenuation and the shadow value to the colour.
			tempColour = tempColour * luminosity * shadowVal;
		}
	}
	///***LIGHT 1***///


	///***LIGHT 2***///
	// Set the light direction for calculations.
	lightDir = input.position3D.xyz - raw_position2.xyz;

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
		RangeQ = length(range - distance) / length(range);

		if (lightIntensity > 0.0f)
		{
			// Calculate reflection vector based on the light intensity, normal vector and light direction.
			reflection = normalize(reflect(lightDir, input.normal));

			// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
			specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower.x);

			// Sum up the specular light colour.
			finalSpec = specularColor * specular;

			// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity and add specular into the attenuation field.
			tempColour2 += saturate(((diffuseColor2 * lightIntensity) + finalSpec) * RangeQ);

			// Calculate the attenuation value.
			attenuation = constantFact + (linearFact * distance) + (quadraticFact * distance);

			// Luminosity.
			luminosity = 4.0f / attenuation;

			// Calculate the projected texture coordinates.
			projectTexCoord.x = input.lightViewPosition2.x / input.lightViewPosition2.w / 2.0f + 0.5f;
			projectTexCoord.y = -input.lightViewPosition2.y / input.lightViewPosition2.w / 2.0f + 0.5f;

			// Sample the shadow value from the shadow texture using the sampler at the projected texture coordinate location.
			shadowVal = depthMapTexture2.Sample(SampleTypeClamp, projectTexCoord).r;

			// Add the attenuation and the shadow value to the colour.
			tempColour2 = tempColour2 * luminosity * shadowVal;
		}
	}
	///***LIGHT 2***///


	///***LIGHT 3***///
	// Set the light direction for calculations.
	lightDir = input.position3D.xyz - raw_position3.xyz;

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
		RangeQ = length(range - distance) / length(range);

		if (lightIntensity > 0.0f)
		{
			// Calculate reflection vector based on the light intensity, normal vector and light direction.
			reflection = normalize(reflect(lightDir, input.normal));

			// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
			specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower.x);

			// Sum up the specular light colour.
			finalSpec = specularColor * specular;

			// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity and add specular into the attenuation field.
			tempColour3 += saturate(((diffuseColor3 * lightIntensity) + finalSpec) * RangeQ);

			// Calculate the attenuation value.
			attenuation = constantFact + (linearFact * distance) + (quadraticFact * distance);

			// Luminosity.
			luminosity = 4.0f / attenuation;

			// Calculate the projected texture coordinates.
			projectTexCoord.x = input.lightViewPosition3.x / input.lightViewPosition3.w / 2.0f + 0.5f;
			projectTexCoord.y = -input.lightViewPosition3.y / input.lightViewPosition3.w / 2.0f + 0.5f;

			// Sample the shadow value from the shadow texture using the sampler at the projected texture coordinate location.
			shadowVal = depthMapTexture3.Sample(SampleTypeClamp, projectTexCoord).r;

			// Add the attenuation and the shadow value to the colour.
			tempColour3 = tempColour3 * luminosity * shadowVal;
		}
	}
	///***LIGHT 3***///


	///***LIGHT 4***///
	// Set the light direction for calculations.
	lightDir = input.position3D.xyz - raw_position4.xyz;

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
		RangeQ = length(range - distance) / length(range);

		if (lightIntensity > 0.0f)
		{
			// Calculate reflection vector based on the light intensity, normal vector and light direction.
			reflection = normalize(reflect(lightDir, input.normal));

			// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
			specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower.x);

			// Sum up the specular light colour.
			finalSpec = specularColor * specular;

			// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity and add specular into the attenuation field.
			tempColour4 += saturate(((diffuseColor4 * lightIntensity) + finalSpec) * RangeQ);

			// Calculate the attenuation value.
			attenuation = constantFact + (linearFact * distance) + (quadraticFact * distance);

			// Luminosity.
			luminosity = 4.0f / attenuation;

			// Calculate the projected texture coordinates.
			projectTexCoord.x = input.lightViewPosition4.x / input.lightViewPosition4.w / 2.0f + 0.5f;
			projectTexCoord.y = -input.lightViewPosition4.y / input.lightViewPosition4.w / 2.0f + 0.5f;

			// Sample the shadow value from the shadow texture using the sampler at the projected texture coordinate location.
			shadowVal = depthMapTexture4.Sample(SampleTypeClamp, projectTexCoord).r;

			// Add the attenuation and the shadow value to the colour.
			tempColour4 = tempColour4 * luminosity * shadowVal;
		}
	}
	///***LIGHT 4***///


	// Saturate the final light color.
	color = saturate(tempColour + tempColour2 + tempColour3 + tempColour4);

	// Combine the light and texture color.
	color = color * textureColor;

    return color;
}