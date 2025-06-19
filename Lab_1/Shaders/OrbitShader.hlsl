#pragma pack_matrix(row_major)

struct VS_IN
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
};

PS_IN VSMain(VS_IN input)
{
    PS_IN output;
    output.pos = mul(input.pos, gWorldViewProj);
    output.color = input.color;
    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    return input.color;
}