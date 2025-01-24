struct PSInput
{
    float4 position : SV_POSITION; // Input from vertex shader
};

float4 main(PSInput input) : SV_TARGET
{
    return float4(0.1, 0.6, 0.8, 1.0); // Return a flat color (RGBA)
}
