#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    //spotlight
    vec3 position;
    vec3 direction;
    float cutoff;

    //vec3 lightPos;
    vec3 lightVec;//vector or point, it depends
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //light attenuation
    float constant;
    float linear;
    float quadratic;
};
uniform Light light;
uniform Material material;

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

void main()
{
    //--------phong's model
    vec3 lightDir =normalize(light.lightVec - FragPos);
    float theta = dot(lightDir,normalize(-light.direction));
    vec3 result;

    if(theta > light.cutoff)
    {
        //ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

        //diffuse
        vec3 norm = normalize(Normal);
       
        //vec3 lightDir =normalize(-light.lightDir);
        float diff = max(dot(norm,lightDir),0.0);
        vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

        //specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir,norm);
        float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));

        float dotdistance = length(light.lightVec - FragPos);
        float attenuation = 1.0/(light.constant + light.linear * dotdistance + light.quadratic * dotdistance * dotdistance);

        specular *= attenuation;
        ambient *= attenuation;
        diffuse *= attenuation;
        result = specular + ambient + diffuse;
    }
    else
        result = vec3(light.ambient * vec3(texture(material.diffuse, TexCoords)));
    
    FragColor = vec4(result, 1.0);
}