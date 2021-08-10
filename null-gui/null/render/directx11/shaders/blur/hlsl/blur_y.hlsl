struct PS_INPUT {
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
};

Texture2D tex_source : TEX_SOURCE;
sampler tex_sampler;

cbuffer pixel_buffer : register(b0) {
    float2 clamp_coordinats;
    float texel_height;
};

static const float offsets[3] = { 0.0f, 1.3846153846f, 3.2307692308f };
static const float weights[3] = { 0.2270270270f, 0.3162162162f, 0.0702702703f };

float4 main(PS_INPUT input) : SV_Target {
    float4 color = tex_source.Sample(tex_sampler, input.uv);
    color *= weights[0];
    
    for (int i = 1; i < 3; ++i) {
        float upper = clamp(input.uv.y - texel_height * offsets[i], clamp_coordinats.x, clamp_coordinats.y);
        float bottom = clamp(input.uv.y + texel_height * offsets[i], clamp_coordinats.x, clamp_coordinats.y);
        color += tex_source.Sample(tex_sampler, float2(input.uv.x, upper)) * weights[i];
        color += tex_source.Sample(tex_sampler, float2(input.uv.x, bottom)) * weights[i];
    }

    return color;
}