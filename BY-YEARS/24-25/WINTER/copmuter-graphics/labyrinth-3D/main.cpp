#include <GL/glew.h>
#include <GLFW/glfw3.h>

// #include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

// [1] : argumenty z lini polecen [done]
// [2] : malowanie piramidek odpowiednimi kolorkami [done]
// [3] : widok z przodu gracza/kulki (POV) [done]
// [4] : drugi widok (minimapa) [done]
// [5] : gracz sfera z scian [done]
// [6] : obslugiwanie myszy [done]
// [7] : resize okna z zachowaniem proporcji [done]
// [8] : mov speed poprawnie dobrany [done]
// [9] : wzorek kostki [done]
// [10] : wzorki na piramidkach [done]
// [EXTRA] : krecenie sie piramidek w roznych osiach [done]
 
using namespace std;
using namespace glm;

// ogolne dane :
int WIN_WIDTH = 800;
int WIN_HEIGHT = 600;

int grid = 5;
int generationSeed = 5;

const float cubeMin = -1.0f; // rozmiar kostki
const float cubeMax = 1.0f;

float sphereRadius = 0.05f; // promien kulki

float deltaTime = 0.0f; // czas
float lastFrameTime = 0.0f;
const int FPS = 1.0f / 1000.0f;
float time_accumulated = 0.0f;

vec3 cameraPos = vec3(-0.9f, -0.9f, -0.9f); // inicjalizacja kamery
float Yaw = 45.0f; // -z
float Pitch = 0.0f;
float cameraSpeed = 0.5f;

float lastX = WIN_WIDTH / 2.0f; // mysz
float lastY = WIN_HEIGHT / 2.0f; // ustawiamy ze orginalnie zaczynamy z myszka na srodku
bool firstMouse = true;

float mouseSensitivity = 0.1f;

class Sphere {
public:
    float radius;
    vec3 position;
    unsigned int VAO, VBO, EBO;
    vector<float> vertices;
    vector<unsigned int> indices;

    Sphere(float radius, vec3 position) {
        this->radius = radius;
        this->position = position;
        buildSphere();
        setupBuffers();
    }

    void buildSphere() {
        // https://stackoverflow.com/questions/33770689/how-to-render-a-sphere-with-triangle-strips
        const int verticalSlices = 20; // gdzies przeczytalem ze sensowny
        const int horizontalSlices = 20; // max to 20
        float x, y, z; // wspolrzędne wierzchłka

        float horizontalStep = M_PI / horizontalSlices; // θ 
        float verticalStep = 2 * M_PI / verticalSlices; // φ  
        
        // tutaj generujemy wierzchołki
        // x = r sinθ cosφ 
        // y = r cosθ 
        // z = r sinθ sinφ
        for(int i = 0; i < horizontalSlices; i++) {

            float theta = i * horizontalStep; // θ od 0 do pi
            float sinTheta = sinf(theta);
            float cosTheta = cosf(theta);

            for(int j = 0; j < verticalSlices; j++) {

                float phi = j * verticalStep; // φ od 0 do 2pi
                float sinPhi = sinf(phi);
                float cosPhi = cosf(phi);

                x = radius * sinTheta * cosPhi;
                y = radius * cosTheta;
                z = radius * sinTheta * sinPhi;

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }

        // tutaj generujemy sobie indeksy do ebo
        // robimy sobie kwadraciki skladajace sie z 2 trojkatow
        unsigned int currSlice, nextSlice;
        for(int i = 0; i < horizontalSlices; i++) {

            currSlice = i * (verticalSlices + 1); // poczatek aktualnego "plastra"
            nextSlice = currSlice + verticalSlices + 1; // pocztatek nastepnego "plastra"

            for(int j = 0; j < verticalSlices; j++, currSlice++, nextSlice++) {

                if(i != 0)
                {
                    indices.push_back(currSlice); // pierwszy trojkat
                    indices.push_back(nextSlice);
                    indices.push_back(currSlice + 1);
                }

                if(i != (horizontalSlices - 1))
                {
                    indices.push_back(currSlice + 1); // drogi trojkat
                    indices.push_back(nextSlice);
                    indices.push_back(nextSlice + 1);
                }
            }
        }
    }
    // https://learnopengl.com/Getting-started/Hello-Triangle
    void setupBuffers() {

        glGenVertexArrays(1, &VAO); // generujemy sobie nasze buffer'y
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO); // bindujem'y vao 
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // tutaj vbo
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW); // wgrywamy dane

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // analogicznie tylko dla ebo
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()* sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // vertex data dla shadera
        glEnableVertexAttribArray(0);

        glBindVertexArray(0); // "odbindowujemy" vao zeby nic przypadkiem nie zmienic
    }

    void draw(Shader& shader, mat4 model) {
        shader.use();
        shader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

class Cube { // to jest nasza przestrzen gry
public: 
    float cubeMin, cubeMax;
    unsigned int VAO, VBO, EBO;

    Cube(float cubeMin, float cubeMax) {
        this->cubeMin = cubeMin;
        this->cubeMax = cubeMax;
        setBuffers();
    }

    void setBuffers() {
        float cubeVertices[] = { // wierzcholki kostki
            cubeMin, cubeMin, cubeMin, // -1 -1 -1 
            cubeMax, cubeMin, cubeMin, // 1 -1 -1
            cubeMax, cubeMax, cubeMin, // 1 1 -1
            cubeMin, cubeMax, cubeMin, // -1 1 -1 
            cubeMin, cubeMin, cubeMax, // -1 -1 1
            cubeMax, cubeMin, cubeMax, // 1 -1 1
            cubeMax, cubeMax, cubeMax, // 1 1 1 
            cubeMin, cubeMax, cubeMax, // -1 1 1
        };

        unsigned int cubeIndices[] = { // to są "krawędzie"
            0,1,2,2,3,0,
            0,3,4,4,7,3,
            3,7,6,6,3,2,
            2,1,5,5,2,6,
            6,7,5,5,7,4,
            4,0,5,5,1,0
        };

        glGenVertexArrays(1, &VAO); // tutaj jest juz analogicznie do kulki
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // nasze wspolrzedne ktore w vertex'ie wyciagniemy
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // layout 0
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    void draw(Shader& shader, mat4 view, mat4 projection, vec3 color) {
        
        shader.use();

        mat4 model = mat4(1.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("objectColor", color);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

class Pyramid { // piramidki do labiryntu
public:
    vec3 position;
    float pyramid_scale;
    float rotationAngle;
    float rotationSpeed;
    vec3 rotationAxis;
    vec3 color;
    unsigned int VAO, VBO, EBO;

    Pyramid(vec3 position, float rotationAngle, vec3 rotationAxis, float pyramid_scale, vec3 color) {
        this->position = position;
        this->rotationAngle = rotationAngle;
        this->rotationAxis = rotationAxis;
        this->pyramid_scale = pyramid_scale;
        this->color = color;
        this->rotationSpeed = (float)(rand()) / RAND_MAX * 2.0f;

        setupBuffers();
    }

    void setupBuffers() {
        float vertices[] = {
            -0.5f, 0.0f, -0.5f, // podstawa
             0.5f, 0.0f, -0.5f,
             0.5f, 0.0f,  0.5f,
            -0.5f, 0.0f,  0.5f,
             0.0f, 1.0f,  0.0f // czobek
        };

        unsigned int indices[] = {
            0, 1, 2, // podstawa
            0, 2, 3,
            0, 1, 4, // sciany
            1, 2, 4,
            2, 3, 4,
            3, 0, 4
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    vector<vec3> getTransformedVertices(float time) { // odtwarzamy wierzchołki na te "po transformacjach"
        vector<vec3> vertices = {
            vec3(-0.5f, 0.0f, -0.5f),
            vec3(0.5f, 0.0f, -0.5f),
            vec3(0.5f, 0.0f, 0.5f),
            vec3(-0.5f, 0.0f, 0.5f),
            vec3(0.0f, 1.0f, 0.0f)
        };

        mat4 model = mat4(1.0f);
        model = translate(model, position);

        float rotationAngleOverTime = rotationAngle + time;
        model = rotate(model, rotationAngleOverTime, rotationAxis);
        model = scale(model, vec3(pyramid_scale));

        for (int i = 0; i < (int)vertices.size(); ++i) {
            vec4 transformed = model * vec4(vertices[i], 1.0f);
            vertices[i] = vec3(transformed);
        }

        return vertices;
    }

    void draw(Shader &shader, float time) {
        mat4 model = mat4(1.0f);
        model = translate(model, position);

        float rotationAngleOverTime = rotationAngle + time;

        model = rotate(model, rotationAngleOverTime, rotationAxis);
        model = scale(model, vec3(pyramid_scale));

        shader.setMat4("model", model);
        shader.setVec3("objectColor", color);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

class Exit { // tutaj mamy wyjsciowa piramidke
public:
    vec3 position;
    float exit_scale;
    float rotationAngle;
    vec3 color;
    unsigned int VAO, VBO, EBO;

    Exit(vec3 position, float exit_scale, vec3 color, float rotationAngle = 0.0f) {
        this->position = position;
        this->exit_scale = exit_scale;
        this->color = color;
        this->rotationAngle = rotationAngle;
        setupBuffers();
    }

    void setupBuffers() {
        float vertices[] = {
            -0.5f, 0.0f, -0.5f, // podstawa
             0.5f, 0.0f, -0.5f,
             0.5f, 0.0f,  0.5f,
            -0.5f, 0.0f,  0.5f,
             0.0f, 1.0f,  0.0f // czobek
        };

        unsigned int indices[] = {
            0, 1, 2, // podstawa
            0, 2, 3,
            0, 1, 4, // sciany
            1, 2, 4,
            2, 3, 4,
            3, 0, 4
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    vector<vec3> getTransformedVertices() { // odtwarzamy wierzchołki na te "po transformacjach"
        vector<vec3> vertices = {
            vec3(-0.5f, 0.0f, -0.5f),
            vec3(0.5f, 0.0f, -0.5f),
            vec3(0.5f, 0.0f, 0.5f),
            vec3(-0.5f, 0.0f, 0.5f),
            vec3(0.0f, 1.0f, 0.0f)
        };

        mat4 model = mat4(1.0f);
        model = translate(model, position);
        model = scale(model, vec3(exit_scale));

        for (int i = 0; i < (int)vertices.size(); ++i) {
            vec4 transformed = model * vec4(vertices[i], 1.0f);
            vertices[i] = vec3(transformed);
        }

        return vertices;
    }

    void draw(Shader &shader) {
        mat4 model = mat4(1.0f);
        model = translate(model, position);
        model = rotate(model, rotationAngle, vec3(0.0f, 1.0f, 0.0f));
        model = scale(model, vec3(exit_scale));

        shader.setMat4("model", model);
        shader.setVec3("objectColor", color);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};


// globalny labirynt piramid
vector<Pyramid> mazePyramids;

vec3 closestPointTriangle(vec3 const& p, vec3 const& a, vec3 const& b, vec3 const& c) { // funkcja sprawdza jaki jest najblizszy punkt do trojkatu
   const vec3 ab = b - a;
   const vec3 ac = c - a;
   const vec3 ap = p - a;

   const float d1 = dot(ab, ap);
   const float d2 = dot(ac, ap);
   if (d1 <= 0.f && d2 <= 0.f) return a; //#1

   const vec3 bp = p - b;
   const float d3 = dot(ab, bp);
   const float d4 = dot(ac, bp);
   if (d3 >= 0.f && d4 <= d3) return b; //#2

   const vec3 cp = p - c;
   const float d5 = dot(ab, cp);
   const float d6 = dot(ac, cp);
   if (d6 >= 0.f && d5 <= d6) return c; //#3

   const float vc = d1 * d4 - d3 * d2;
   if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f) {
      const float v = d1 / (d1 - d3);
      return a + v * ab; //#4
   }

   const float vb = d5 * d2 - d1 * d6;
   if (vb <= 0.f && d2 >= 0.f && d6 <= 0.f) {
      const float v = d2 / (d2 - d6);
      return a + v * ac; //#5
   }

   const float va = d3 * d6 - d5 * d4;
   if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) >= 0.f) {
      const float v = (d4 - d3) / ((d4 - d3) + (d5 - d6));
      return b + v * (c - b); //#6
   }

   const float denom = 1.f / (va + vb + vc);
   const float v = vb * denom;
   const float w = vc * denom;
   return a + v * ab + w * ac; //#0
}

// prototypy funkcji
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Sphere playerSphere, float current_time);
void checkEndpoint(GLFWwindow* window, Exit exitPyramid);
void InitializeMaze(int gridSize, int generationSeed);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


int main(int argc, char* argv[]) {

    switch(argc) {
        case 1:
            break;
        case 2:
            grid = atoi(argv[1]);
            if (grid < 3) {
                grid = 3;
            }
            break;
        case 3:
            grid = atoi(argv[1]);
            if (grid < 3) {
                grid = 3;
            }
            generationSeed = atoi(argv[2]);
            break;
        default:
            throw runtime_error("to much arguments :C");
   }

    srand(generationSeed); // generator do losowych przekręceń
    
    if (!glfwInit()) { // jak jest to super
        cerr << "Error: GLFW initialization failed" << endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // GLFW konkretna wersja
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    // ogarnijmy okno
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Z4", NULL, NULL);
    if (!window) { // jest jak tak to super
        cerr << "Error: Window creation failed" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // lapiemy myszke
    glfwSetCursorPosCallback(window, mouse_callback);

    if (glewInit()) { // jak jest to super
        cerr << "Error: GLEW initialization failed" << endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Tutaj odpalamy nasze shadery
    Shader cubeShader("cube_vertex.glsl", "cube_fragment.glsl"); // kostka
    Shader sphereShader("sphere_vertex.glsl", "sphere_fragment.glsl"); // shader kulki
    Shader colorShader("color_vertex.glsl", "color_fragment.glsl"); // najprostsze bazowe shadery spelniaja wymagania dla exit / piramidek wireframe
    Shader pyramidShader("pyramid_vertex.glsl", "pyramid_fragment.glsl"); // dla main sceny piramidek


    Cube gameCube(cubeMin, cubeMax); // tworzymy kostke

    InitializeMaze(grid, generationSeed); // inicjalizujemy labirynt piramidek

    Sphere playerSphere(sphereRadius, cameraPos); // kulka gracza

    vec3 exitPosition = vec3(0.9f, 0.9f, 0.9f); // dane wyjsciowej piramidki
    float exitScale = 0.1f; // "skala"
    vec3 exitColor = vec3(1.0f, 0.0f, 0.0f); // kolor piramidki
    Exit exitPyramid(exitPosition, exitScale, exitColor); // no i tu sobie tworzymy ta piramidke

    while (!glfwWindowShouldClose(window)) {

        // time logic here
        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        

        if (time_accumulated > FPS) { // ograniczenie pod FPS
            deltaTime = time_accumulated;
            time_accumulated = 0.0f;
        } else {
            time_accumulated += deltaTime;
        }

        processInput(window, playerSphere, currentFrameTime); // oczywiste

        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT); // tutaj mamy "glowny widok" / main scene

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // czyscimy oba buffory kolor i depth

        // projection : jak 3d bedzie mapowane na 2d (3D -> NDC)
        // mat4 projection = perspective(radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height); // sciagamy rozmiar okna
        float aspectRatio = (float)width / (float)height; // ustawiamy nowe proporcje
        
        mat4 projection = perspective(radians(45.0f), aspectRatio, 0.1f, 100.0f); // i cyk dziala
        // view : z projection tak jakby scena/klatka była widoczna z perspektywy kamery
        vec3 cameraFront;
        cameraFront.x = cos(radians(Yaw)) * cos(radians(Pitch));
        cameraFront.y = sin(radians(Pitch));
        cameraFront.z = sin(radians(Yaw)) * cos(radians(Pitch));
        mat4 view = lookAt(cameraPos, cameraPos + cameraFront, vec3(0.0f, 1.0f, 0.0f));

        gameCube.draw(cubeShader, view, projection, vec3(1.0f, 1.0f, 1.0f)); // kostka ograniczajaca przestrzen gry

        float pyramidTime = glfwGetTime();
        pyramidShader.use(); // piramidy z labiryntu
        pyramidShader.setMat4("projection", projection);
        pyramidShader.setMat4("view", view);
        pyramidShader.setFloat("time", pyramidTime);

        for (int i = 0; i < (int)mazePyramids.size(); i++) {
            mazePyramids[i].draw(pyramidShader, pyramidTime);
        }

        sphereShader.use();
        sphereShader.setMat4("projection", projection);
        sphereShader.setMat4("view", view);
        sphereShader.setVec3("objectColor", vec3(0.0f, 1.0f, 0.0f)); // kulka gracza
        // ustawiamy kolor na zielony
        mat4 playerModel = mat4(1.0f);
        playerModel = translate(playerModel, cameraPos); // przesuwamy kulke na pozycje kamery
        playerSphere.draw(sphereShader, playerModel); // no tu oczywiscie malowanie modelu gracza

        colorShader.use();
        colorShader.setMat4("projection", projection);
        colorShader.setMat4("view", view);
        exitPyramid.draw(colorShader); // malujemy wyjsiowa piramide

        checkEndpoint(window, exitPyramid); // doszlismy do wyjscia ?
        
        glClear( GL_DEPTH_BUFFER_BIT); // tutaj dodaje bo jak tego nie bylo to mi obiekty przy kontakcie sie
                                       // brzydko zachowywaly co ma w sumie sens

        // minimapa
        int miniMapWidth = WIN_WIDTH / 5;
        int miniMapHeight = WIN_HEIGHT / 5;
        int xOffset = WIN_WIDTH - miniMapWidth - 5; // tak zeby ladnie bylo
        int yOffset = WIN_HEIGHT - miniMapHeight - 5; // to bedzie kawalek od kranca ekranu
        glViewport(xOffset, yOffset, miniMapWidth, miniMapHeight);

        // An orthographic projection matrix defines a cube-like frustum box that defines the 
        // clipping space where each vertex outside this box is clipped. (Cytat z tutoriala)
        mat4 miniMapProjection = ortho(-0.75f, 0.75f, -0.75f, 0.75f, 0.1f, 100.0f);
        
        // view : dla minimapy tutaj dokladnie nad graczem
        vec3 miniMapCameraPos = cameraPos + vec3(0.0f, 2.0f, 0.0f); // pozycja kamery nad graczem
        vec3 miniMapCameraTarget = cameraPos; // targetem kamey jest gracz
        vec3 miniMapCameraUp = vec3(0.0f, 0.0f, -1.0f); // jak jest "zorientowana" kamera tutaj prostopadle do main widoku
        // macierz look at view 
        mat4 miniMapView = lookAt(miniMapCameraPos, miniMapCameraTarget, miniMapCameraUp);

        // tutaj bedziemy generowac content dla minimapy
        // gameCube.draw(cubeShader, miniMapView, miniMapProjection, vec3(1.0f, 1.0f, 1.0f)); // tego nie maluje bo brzydko z kostką wygląda minimapa

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // pramidki sa wireframe
        colorShader.use();
        colorShader.setMat4("projection", miniMapProjection);
        colorShader.setMat4("view", miniMapView);

        for (int i = 0; i < (int)mazePyramids.size(); i++) {
            mazePyramids[i].draw(colorShader, pyramidTime);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // wyjscie maluemy normalnie
        exitPyramid.draw(colorShader);

        sphereShader.use();
        sphereShader.setMat4("projection", miniMapProjection);
        sphereShader.setMat4("view", miniMapView);
        // tutaj analogicznie jak dla main "sceny"
        sphereShader.setVec3("objectColor", vec3(0.0f, 1.0f, 0.0f));

        mat4 playerModel2 = mat4(1.0f);
        playerModel2 = translate(playerModel2, cameraPos);

        playerSphere.draw(sphereShader, playerModel2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // posprzątajmy po sobie
    glfwTerminate();
    glfwDestroyWindow(window);
    return 0;
}

// funckje 

void processInput(GLFWwindow *window, Sphere playerSphere, float currentTime) {
    
    float adjustedSpeed = cameraSpeed * deltaTime;

    // https://learnopengl.com/Getting-started/Camera
    vec3 cameraDirection;
    cameraDirection.x = cos(radians(Yaw)) * cos(radians(Pitch));
    cameraDirection.y = sin(radians(Pitch));
    cameraDirection.z = sin(radians(Yaw)) * cos(radians(Pitch));

    // vec3 right = cross(cameraDirection, vec3(0.0f, 1.0f, 0.0f));
    

    vec3 newPos = cameraPos;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        newPos += adjustedSpeed * cameraDirection;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        newPos -= adjustedSpeed * cameraDirection;
    }

    if (newPos.x > cubeMin && newPos.x < cubeMax && newPos.y > cubeMin && newPos.y < cubeMax && newPos.z > cubeMin && newPos.z < cubeMax) {
        // jezeli jaki kolwiek z tych jest false no to byli bysmy poza kostka i wtedy nie aktualizujemy pozycji gracza : kolizja wykryta

        bool collision = false;
        for (int i = 0; i < (int)mazePyramids.size(); i++) {

            Pyramid& pyramid = mazePyramids[i];
            vector<vec3> vertices = pyramid.getTransformedVertices(currentTime);

            vector<array<int, 3>> triangles = {
                {0, 1, 2},
                {0, 2, 3},
                {0, 1, 4},
                {1, 2, 4},
                {2, 3, 4},
                {3, 0, 4}
            };

            for (int j = 0; j < (int)triangles.size(); j++) {

                const array<int, 3>& tri = triangles[j];
                vec3 a = vertices[tri[0]];
                vec3 b = vertices[tri[1]];
                vec3 c = vertices[tri[2]];

                vec3 closestPoint = closestPointTriangle(newPos, a, b, c); // [1]
                vec3 diff = newPos - closestPoint; // [2]
                float distanceSq = dot(diff, diff); // [3]
                // liczymy tutaj odleglosc miedzy "najblizszym pkt na trojkacie" a pozycja gracza
                // [1] zwraca nam ten pkt
                // [2] zwraca nam (px - qx), (py - qy), (pz, - qz)
                // i teraz zeby policzyc dystans to musieli bysmy liczyc sqrt z ^2 ale jest fancy trik
                // [3] uzywamy dot product aby policzyc kwadraty tych roznic bo dot da nam :
                // (px - qx) * (px - qx) + ... co zasadniczo bedzie suma kwadratow
                // zatem majac ta kwadratowa odleglosc mozemy porownac to z kwadratem promienia 
                // bo matma dziala, pretty cool 

                if (distanceSq < playerSphere.radius * playerSphere.radius) { // dlatego to r^2
                    collision = true;
                    break;
                }
            }
            if (collision)
                break;
        }
        if (!collision) {
            cameraPos = newPos;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {

        float sensitivity = 2.0f;
        if (key == GLFW_KEY_UP)
            Pitch += sensitivity;
        if (key == GLFW_KEY_DOWN)
            Pitch -= sensitivity;
        if (key == GLFW_KEY_LEFT)
            Yaw -= sensitivity;
        if (key == GLFW_KEY_RIGHT)
            Yaw += sensitivity;

        if (Pitch > 89.0f) // zeby nie przeskakiwala kamera
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // https://learnopengl.com/Getting-started/Camera

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Constrain pitch
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;
}

// do zmiany rozmiaru okna
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    WIN_WIDTH = width;
    WIN_HEIGHT = height;

    if (height == 0) // zero nie bylo by cool
        height = 1;

    glViewport(0, 0, width, height);
}

void checkEndpoint(GLFWwindow* window, Exit exitPyramid) { // przyznaje sie to moglo byc ładniej
    float distance = length(cameraPos - exitPyramid.position);
    if (distance < 0.1f) {
        glfwSetWindowShouldClose(window, true);
    }
}

void InitializeMaze(int gridSize, int generationSeed) { // tutaj analogicznie do porzedniego zadaina inicijalizowany jest labirynt

    const float margin = cubeMax - 0.05f;

    float startingBoundary = -margin;
    float distribution = (margin * 2.0f) / (gridSize - 1);
    float pyramidScale = distribution * 0.65f;

    srand(generationSeed);

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            for (int k = 0; k < gridSize; k++) {

                if ((i == 0 && j == 0 && k == 0) || (i == gridSize - 1 && j == gridSize - 1 && k == gridSize - 1)) // pozbywamy sie pierwszej piramidki i ostatniej
                    continue;

                float x = startingBoundary + (i * distribution);
                float y = startingBoundary + (j * distribution);
                float z = startingBoundary + (k * distribution);
                vec3 position(x, y, z);

                float rotationAngle = 0.0f;

                vec3 rotationAxis(
                    (float)(rand()) / RAND_MAX,
                    (float)(rand()) / RAND_MAX,
                    (float)(rand()) / RAND_MAX
                );

                // kolor do zmiany
                vec3 color(
                    x*x + 0.2f,
                    y,
                    z);

                Pyramid pyramid(position, rotationAngle, rotationAxis, pyramidScale, color);
                mazePyramids.push_back(pyramid);
            }
        }
    }
}

