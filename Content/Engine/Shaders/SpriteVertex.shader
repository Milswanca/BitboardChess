#version 410 core
layout(location = 0) in vec3 Position_VS_in;
layout(location = 1) in vec2 UV_VS_in;
layout(location = 5) in vec4 Color_VS_in;

out VS_OUT
{
    vec2 UV;
    vec4 Color;
} vs_out;

uniform mat4 gModel;
uniform mat4 gProjectionView;

void main()
{
    vs_out.UV = UV_VS_in;
    vs_out.Color = Color_VS_in;

    gl_Position = gProjectionView * gModel * vec4(Position_VS_in, 1.0);
}