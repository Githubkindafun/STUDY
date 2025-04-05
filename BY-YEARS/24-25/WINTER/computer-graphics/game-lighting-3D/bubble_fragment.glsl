#version 330 core
#define MAX_LIGHTS 100 // ogranicznie dla swiecacych specjalnych babelkow

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
flat in int IsSpecial;

uniform vec3 viewPos;
uniform float alpha;

/////////////////////////////////////////////////
uniform vec3 waterAbsorption;
/////////////////////////////////////////////////

struct Light { // swiatlo specjalnych babelkow
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform int numLights;
uniform Light lights[MAX_LIGHTS];

struct MainLight { // main oswietlenie
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform MainLight mainLight;

struct Spotlight { // latarka gracza
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Spotlight spotlight;

struct Material { // material jakim jest babelek i jego wlasciwosci
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos); // kierunek do gracza
    vec3 result = vec3(0.0);

    vec3 lightDir = normalize(mainLight.position - FragPos); // kierunek do main swiatla

    vec3 ambient = mainLight.ambient * material.ambient * Color;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = mainLight.diffuse * diff * material.diffuse * Color;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = mainLight.specular * spec * material.specular;

    result += ambient + diffuse + specular;

    for (int i = 0; i < numLights; i++) { // swiatlo specjalnych babelkow

        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));

        vec3 lightDir = normalize(lights[i].position - FragPos);

        vec3 ambient = lights[i].ambient * material.ambient * Color;

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lights[i].diffuse * diff * material.diffuse * Color;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = lights[i].specular * spec * material.specular;

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        result += ambient + diffuse + specular;
    }

    vec3 spotLightDir = normalize(spotlight.position - FragPos); // swiatlo z latarki
    float theta = dot(spotLightDir, normalize(-spotlight.direction));
    float epsilon = spotlight.cutOff - spotlight.outerCutOff;
    float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(spotlight.position - FragPos);
    float attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance + spotlight.quadratic * (distance * distance));

    vec3 ambientSpot = spotlight.ambient * material.ambient * Color;

    float diffSpot = max(dot(norm, -spotLightDir), 0.0);
    vec3 diffuseSpot = spotlight.diffuse * diffSpot * material.diffuse * Color;

    vec3 reflectDirSpot = reflect(spotLightDir, norm);
    float specSpot = pow(max(dot(viewDir, reflectDirSpot), 0.0), material.shininess);
    vec3 specularSpot = spotlight.specular * specSpot * material.specular;

    ambientSpot *= attenuation * intensity;
    diffuseSpot *= attenuation * intensity;
    specularSpot *= attenuation * intensity;

    result += ambientSpot + diffuseSpot + specularSpot;

    if (IsSpecial == 1) {
        vec3 highlight = vec3(0.8f, 0.0f, 0.0f); // podswietlenie specjalnych babelkow
        result += highlight;
    }

///////////////////////////////////////////////////////////////
    float distanceToCamera = length(FragPos - viewPos);
    vec3 waterAttenuation = exp(-waterAbsorption * distanceToCamera);
    result *= waterAttenuation;
///////////////////////////////////////////////////////////////






    FragColor = vec4(result, alpha);
}
