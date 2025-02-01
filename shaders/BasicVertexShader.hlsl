struct VSInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL; // Add normal
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL; // Pass normal to pixel shader
};

cbuffer Transform : register(b0)
{
    matrix worldViewProj;
    matrix world; // For transforming normals
}

PSInput main(VSInput input)
{
    PSInput output;
    output.position = mul(float4(input.position, 1.0), worldViewProj);
    output.uv = input.uv;
    output.normal = normalize(mul(float4(input.normal, 0.0), world).xyz);
    return output;
}
