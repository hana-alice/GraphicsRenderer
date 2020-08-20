#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube envMap;

const float Pi = 3.14159265359;

void main()
{
    vec3 normal = normalize(WorldPos);
    vec3 irradiance = vec3(0.0);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    float samplerDelta = 0.025;
    float nrSamples = 0.0;
    for(float phi = 0.0; phi < 2.0 * Pi; phi += samplerDelta)
    {
        for(float theta = 0.0; theta < 0.5 * Pi; theta += samplerDelta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            irradiance += texture(envMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = Pi * irradiance * (1.0 / float(nrSamples));
    FragColor = vec4(irradiance, 1.0);
}