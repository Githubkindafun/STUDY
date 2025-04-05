#version 330 core
in vec3 FragPos;
in vec3 LocalPos;

uniform vec3 objectColor;
uniform float time;

out vec4 FragColor;

void main()
{
    float scale = 4.2;   
    float pattern = sin(LocalPos.x * scale + time);

    vec3 white = vec3(1.0);
    vec3 finalColor = mix(objectColor, white, pattern * 0.3); // miksujemy kolor (interpolacja)

    FragColor = vec4(finalColor, 1.0);
}
