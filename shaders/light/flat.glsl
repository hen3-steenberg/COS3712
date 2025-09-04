vec3 getDiffuse(vec3 normal, vec3 lightDirection, vec3 lightColour)
{
    float factor = max(dot(lightDirection, normal), 0);
    return factor * lightColour;
}

vec3 getSpeccular(vec3 normal, vec3 lightDirection, vec3 lightColour, vec3 cameraDirection)
{
    float factor = dot(lightDirection, normal);
    vec3 R = 2 * factor * normal - lightDirection;
    R = normalize(R);
    float factor2 = dot(R, cameraDirection);
    return factor2 * lightColour;
}
