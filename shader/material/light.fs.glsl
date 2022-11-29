#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

// http://devernay.free.fr/cours/opengl/materials.html
void main() {
    // ambient, the reflect color of surface under ambient lighting
    vec3 ambient = light.ambient * material.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    // diffuse, the color of surface under diffuse lighting, set to the desired surface's color
    vec3 diffuse = light.diffuse * (material.diffuse * diff);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // the color of highlight
    vec3 specular = light.specular * (material.specular * spec);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
