#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light
{
	float constant;
    float linear;
    float quadratic;
    vec3 Position;
    vec3 Color;
};

const float strenth = 0.1;
const int NR_LIGHTS = 32;
uniform Light lights[32];
uniform vec3 viewPos;

void main()
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
	float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 lighting = Albedo * strenth;

    //shading
    vec3 viewDir = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0)  * lights[i].Color;
		vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = lights[i].Color * spec * Specular;
		
        float distance = length(lights[i].Position - FragPos);
        float attenuation = 1.0 / (1.0 + lights[i].linear * distance + lights[i].quadratic * distance + lights[i].constant* distance);
        diffuse *= (attenuation);
        specular *= (attenuation);
        lighting += (diffuse + specular);
    }

	if(FragPos.x > 10000.0)
		FragColor = vec4(lights[1].Color, 1.0);
	else
		FragColor = vec4(lighting, 1.0);

}