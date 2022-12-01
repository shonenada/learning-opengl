#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
//    sampler2D diffuse;    // !!! THIS CASE SEGMENTATION FAULT
//    sampler2D specular;    // !!! THIS CASE SEGMENTATION FAULT
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

// http://devernay.free.fr/cours/opengl/materials.html
void main() {
    // ambient, the reflect color of surface under ambient lighting
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));   // !!! THIS CAUSE SEGMENTATION FAULT
    vec3 ambient = light.ambient * vec3(texture(diffuseTexture, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    // diffuse, the color of surface under diffuse lighting, set to the desired surface's color

    // vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));   // !!! THIS CAUSE SEGMENTATION FAULt
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseTexture, TexCoords));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // the color of highlight
    vec3 specular = light.specular * (spec * vec3(texture(specularTexture, TexCoords)));

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 result = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(result, 1.0);
}
