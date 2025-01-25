struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 lightDir = normalize(float3(0.0, -1.0, -1.0)); // Light direction
    float3 normal = normalize(input.normal); // Normalize interpolated normal

    // Simple diffuse lighting
    float lightIntensity = max(dot(normal, lightDir), 0.0);

    float3 baseColor = float3(0.2, 0.5, 0.8); // Light blue color
    return float4(baseColor * lightIntensity, 1.0); // Apply lighting
}
