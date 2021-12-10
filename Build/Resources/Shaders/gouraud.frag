#version 430 core
in vec3 fs_color;
in vec2 fs_texcoord;

out vec4 outColor;

uniform sampler2D sampler;

void main()
{
    outColor = vec4(fs_color, 1.0) * texture(sampler, fs_texcoord);
}