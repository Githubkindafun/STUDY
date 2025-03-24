#version 330 core


in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;

/////////////////////////////////////////////////////////////
uniform vec3 waterAbsorption;
/////////////////////////////////////////////////////////////

void main() {
    
    vec3 wallsColor = vec3(0.2, 0.2, 1.0);

///////////////////////////////////////////////////////////// wzorek
    float distanceToCamera = length(FragPos - viewPos);
    vec3 attenuation = exp(-waterAbsorption * distanceToCamera);
    vec3 result = wallsColor * attenuation;
/////////////////////////////////////////////////////////////


    FragColor = vec4(result, 1.0);
}
