cbuffer TransformBuffer
{
    matrix worldViewProj; // Ensure this matches the buffer in C++ code
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

PSInput main(VSInput input)
{
    PSInput output;
    output.position = mul(float4(input.position, 1.0f), worldViewProj); // Apply the transformation
    output.normal = input.normal; // Pass through the normal
    return output;
}
