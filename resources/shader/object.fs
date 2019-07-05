#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 lightPos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
uniform Material material;

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    //--------phong's model
    //ambient
    float ambientStrength = 0.1;
    vec3 ambient = light.ambient * material.ambient;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir =normalize(lightPos - FragPos);
    float diff = max(dot(lightDir,norm),0.0);
    vec3 diffuse = diff * material.diffuse * light.diffuse;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    vec3 result = (specular + ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}