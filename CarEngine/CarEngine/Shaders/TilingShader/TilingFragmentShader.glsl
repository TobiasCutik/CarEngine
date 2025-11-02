#version 460 core

in vec3 normal;
in vec3 fragmentPosition;
in vec4 fragmentPositionLightSpace;
in vec2 texCoord;
in vec3 vertexPosition;

struct Material
{
    vec4 ambientModifier;
    vec4 diffuseModifier;
    vec4 specularModifier;
    float shininess;
};

uniform Material material;
uniform vec4 lightColor;
uniform vec4 lightPosition;
uniform vec4 cameraPosition;
uniform float ambientLightStrength;
uniform float diffuseLightStrength;
uniform float specularLightStrength;
uniform float shadowStrength;
uniform vec3 scale;

uniform sampler2D diffuseTexture;
uniform sampler2D depthMap;

out vec4 fragColor;

float calculateShadowValue(vec4 fragmentPositionLightSpace, vec3 normalizedNormal, vec3 lightDirection)
{
    vec3 projectedCoordinates = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;

    // transform projectedCoordinates to a range of 0-1
    projectedCoordinates = projectedCoordinates * 0.5f + 0.5f;

    float currentDepth = projectedCoordinates.z;

    // force no shadow, for fragments outside the light's far plane
    if (currentDepth > 1.f)
    return 0.f;

    float shadowBias = 0.001f;

    // percentage-closer filtering (PCF)
    float shadow = 0.f;
    vec2 texelSize = 1.f / textureSize(depthMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMap, projectedCoordinates.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - shadowBias > pcfDepth ? 1.f : 0.f;
        }
    }
    shadow /= 9.f;

    return shadow * shadowStrength;
}

void main()
{
    // ambient lighting
    vec4 ambientLight = ambientLightStrength * lightColor * material.ambientModifier;

    // diffuse lighting
    vec3 normalizedNormal = normalize(normal);
    vec3 lightDirection = normalize(vec3(lightPosition) - fragmentPosition);
    float attenuation = max(dot(normalizedNormal, lightDirection), 0.f);
    vec4 diffuseLight = diffuseLightStrength * lightColor * (attenuation * material.diffuseModifier);

    // specular lighting
    vec3 viewDirection = normalize(vec3(cameraPosition) - fragmentPosition);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specular = pow(max(dot(normal, halfwayDirection), 0.f), material.shininess);
    vec4 specularLight = specularLightStrength * lightColor * (specular * material.specularModifier);

    // calculate shadow
    float shadow = calculateShadowValue(fragmentPositionLightSpace, normalizedNormal, lightDirection);

    // calculate light color
    vec4 fragColorLight = (ambientLight + (1.f - shadow) * (diffuseLight + specularLight));

    vec2 newTexCoord = texCoord;

    if (abs(normal.x) > 0.99f)
    {
        newTexCoord.y *= scale.x;
        newTexCoord.x *= scale.y;
        fragColor = texture(diffuseTexture, newTexCoord) * fragColorLight;
    }
    else if (abs(normal.y) > 0.99f)
    {
        newTexCoord.x *= scale.x;
        newTexCoord.y *= scale.z;
        fragColor = texture(diffuseTexture, newTexCoord) * fragColorLight;
    }
    else if (abs(normal.z) > 0.99f)
    {
        newTexCoord.x *= scale.y;
        newTexCoord.y *= scale.z;
        fragColor = texture(diffuseTexture, newTexCoord) * fragColorLight;
    }
    else
    {
        fragColor = texture(diffuseTexture, texCoord) * fragColorLight;
    }
}