#version 410 core
out vec4 Color_FS_out;

in VS_OUT
{
    vec2 UV;
    vec4 Color;
} fs_in;

// Diffuse
uniform sampler2D gSprite;
uniform vec4 gTint;

void main()
{
    vec4 result = texture(gSprite, fs_in.UV);
    Color_FS_out = result * gTint;
}