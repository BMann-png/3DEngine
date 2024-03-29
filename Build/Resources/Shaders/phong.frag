#version 430 core
in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} fs_in;

out vec4 outColor;

struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform sampler2D sampler;

void main()
{
    //ambient
    vec3 ambient = light.ambient;

    //diffuse
    vec3 light_dir = normalize(vec3(light.position) - fs_in.position);

    float intensity = max(dot(light_dir, fs_in.normal), 0);
    vec3 diffuse = material.diffuse * light.diffuse * intensity;

    //specular
    vec3 specular = vec3(0);
    if(intensity > 0)
    {
        vec3 view_dir = normalize(-fs_in.position);
        vec3 reflection = reflect(-light_dir, fs_in.normal);
        intensity = max(dot(view_dir, reflection), 0);
        intensity = pow(intensity, material.shininess);
        specular = material.specular * light.specular * intensity;
    }

    outColor = vec4(ambient + diffuse, 1) * texture(sampler, fs_in.texcoord) + vec4(specular, 1);
}