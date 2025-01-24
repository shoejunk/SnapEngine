cbuffer Transform : register(b0)
{
    float4x4 worldViewProj; // Combined World, View, Projection matrix
};

struct VSInput
{
    float3 position : POSITION; // Input vertex position
    float3 normal : NORMAL;     // Input vertex normal (not used here, but included for future use)
};

struct VSOutput
{
    float4 position : SV_POSITION; // Transformed vertex position (clip space)
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = mul(float4(input.position, 1.0), worldViewProj); // Apply transformation
    return output;
}
