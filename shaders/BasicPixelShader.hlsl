Texture2D tex : register(t0); // Bind texture to slot t0
SamplerState samplerState : register(s0); // Bind sampler to slot s0

cbuffer LightBuffer : register(b1)
{
    float3 lightDir; // Direction of the light
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL; // Incoming normal
};

float4 main(PSInput input) : SV_TARGET
{
    // Normalize normal and light direction
    float3 normalizedNormal = normalize(input.normal);
    float3 normalizedLightDir = -normalize(lightDir);

    // Calculate diffuse lighting
    float diffuse = max(dot(normalizedNormal, normalizedLightDir), 0.0);

    // Sample texture
    float4 baseColor = tex.Sample(samplerState, input.uv);

    // Add ambient lighting
    float ambient = 0.1;
    return saturate(baseColor * (diffuse + ambient));
}
