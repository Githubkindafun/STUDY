#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

// [1] : strzalki jako kierunki gdzie sie porusza (przod, tyl, prawo, lewo)     : [done]
// [2] : W/S poruszanie do gory w dol                                           : [done]
// [3] : ustawienie kieruknu patrzeina za pomoca myszy (arcball?)               : [chyba done, tak jak poprzednio zrobilem zwyka zmiane kierunku nie uzywajac 
//                                                                                 arcball'a]
// [4] : +/- do zooma                                                           : [done]
// [5] : tab do przelaczania z POV na side view (bez bocznej sciany akwarium)   : [done]
// [6] : babelki to sfery z trojkatow (oswietlone)                              : [done]
// [7] : oswietlenie z nad akwarium (jedno glowne)                              : [done]
// [8] : gracza wyswietlamy przy side view                                      : [done]
// [9] : efektywnosc renderowania?                                              : [done]

// [10] : 1pkt.: oswietlenie z POV zalezne od odleglosci (w innym kolorze)      : [done]
//               tak dobrane aby widac bylo zblizajce sie babelki (attenuation)
// [11] : 1pkt.: dodatkowe wyrozniajace sie babelki (idk bonusowe pkt lub       : [done]
//               cos innego), o np dodatkowym swietle oswietlajacym inne
// [12] : 1pkt.: symulacja wody                                                 : [TO DO]
// [13] : 3pkt.: przezroczystosc babelkow (wymaga to renderowania ich w         : [done]
//               kolejnosci od najblizszego) (mozna wlaczyc/wylaczyc)
// [14] : 0-4pkt.: estetyka/grywalnosc (2pkt.) plus dodatkowe efekty (2pkt.)    : [?]
// [EXTRA] : tym razem bez extra pkt.
 
// uzylem tutaj kodu z poprzedniego zadania i podazalem za learnopengl

/*
INSTRUKCJA:
na poczatku rozgrywki prosze wejsc w widok pelnoekranowy za pomoca widocznego kursora
nastepnie kliknac 2 razy tab, dzieki temu bedziemy mieli full screen i ladnie schowany kursor.
Jest to lekko toporne ale po przebojach z tym w poprzednim zadainu zostawilem sobie taka mozliwosc.
Jezeli nie ma sie ochoty na full screen wystarczy wcisnac tab 2 razy i kursor zniknie.

TAB : zmiana widoku z POV na SIDE VIEW
SPACE BAR : ON / OFF przezroczystosc
+ / - : przybliza i oddala widok z obu widokow (POV / SIDE)
W / S : w dol / do gory 
STRZALKI : do przodu / w prawo / w lewo / do tylu
MYSZ : obsluguje gdzie patrzymy graczem i wplywa gdzie sie przemieszczamy strzalkami 

PRZEBIEG GRY:
Startujac z gory akwarium zbieramy wyrozniajace sie babelki (lososiowe / pomaranczowe).
Za kazdy babelek otrzymuemy ilosc pkt odpowiadajaca poziomowi na jakim jestesmy.
Przejscie na nastepny poziom tzn. przejsc z gory na dol akwarium (dotknac dolna scianke).
Skoro za kazdy poziom jest wiecej pkt to tez sa trudniejsze bo ilosc pkt babelkow punktowanych jest stale taka sama,
a na kazdym poziomie jest wiecej smiercionosnych babelkow oraz sa szybsze wiec trzeba UWAZAC.
Po zetknieciu z smiercionosnym babelkiem wyswietlany jest koncowy wynik.
MILEJ GRY!

*/


using namespace std;
using namespace glm;

// ogolne dane :

int WIN_WIDTH = 800; // rozmiar okna
int WIN_HEIGHT = 600;

const float CUBE_MIN = -5.0f; // rozmiar akwarium
const float CUBE_MAX = 5.0f;

float sphereRadius = 0.15f; // promien kulki gracza

float deltaTime = 0.0f; // czas
float lastFrameTime = 0.0f; // zasadniczo useless w tym co narazie jest
const int FPS = 1.0f / 1000.0f; // same
float time_accumulated = 0.0f; // same

vec3 cameraPos = vec3(0.0f, CUBE_MAX - sphereRadius, 0.0f); // inicjalizacja kamey
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
float Yaw = -90.0f; // -z
float Pitch = -90.0f;
float cameraSpeed = 2.5f;
float FOV = 45.0f; // FOV : field of view

float lastX = WIN_WIDTH / 2.0f; // mysz
float lastY = WIN_HEIGHT / 2.0f; // ustawiamy ze orginalnie zaczynamy z myszka na srodku
bool firstMouse = true;
float mouseSensitivity = 0.1f;

bool povView = true; // zaczynamy rozgrywke z widkoiem POV

int currentLevel = 1; // to do samej "gry" 
int points = 0;
int bubbleSpeedMultiplier = 1.0f;

bool transparent = true; // on/off przezroczystosc babelkow

float bubbleConstant = 1.0f; // do swiecacych specjalnych babelkow
float bubbleLinear = 0.22f;
float bubbleQuadratic = 0.20f;

const int MAX_BUBBLES = 500; // max liczba babelkow
// tutaj troche sobie napsolem krwi bo chcialem robic wszystko dynamicznie ale jak doszlo
// do organizacji "swiecacych specjalnych babelkow" to zaczelo sie troche nakladac rzeczy wiec koncowo
// tam w shaderze "capuje" mozliwa ilosc swiecacych babelkow


// prototypy funkcji vol. I
float RandomFloat(float min, float max); 

class Sphere { // tu bez zmian jedynie lekkie poprawki
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
        for(int i = 0; i <= horizontalSlices; i++) { // debugging : problem naprawiony brakowalo <=

            float theta = i * horizontalStep; // θ od 0 do pi
            float sinTheta = sinf(theta);
            float cosTheta = cosf(theta);

            for(int j = 0; j <= verticalSlices; j++) { // debugging

                float phi = j * verticalStep; // φ od 0 do 2pi
                float sinPhi = sinf(phi);
                float cosPhi = cosf(phi);

                x = radius * sinTheta * cosPhi;
                y = radius * cosTheta;
                z = radius * sinTheta * sinPhi;

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // dodajemy znormalizowane dla oswietlenia
                vec3 normal = normalize(vec3(x, y, z));
                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);
            }
        }

        // tutaj generujemy sobie indeksy do ebo
        // robimy sobie kwadraciki skladajace sie z 2 trojkatow
        unsigned int currSlice, nextSlice;
        for(int i = 0; i < horizontalSlices; i++) {

            currSlice = i * (verticalSlices + 1); // poczatek aktualnego "plastra"
            nextSlice = currSlice + verticalSlices + 1; // pocztatek nastepnego "plastra"

            for(int j = 0; j < verticalSlices; j++, currSlice++, nextSlice++) {

                if(i != 0) {
                    indices.push_back(currSlice); // pierwszy trojkat
                    indices.push_back(nextSlice);
                    indices.push_back(currSlice + 1);
                }

                if(i != (horizontalSlices - 1)) {
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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // vertex data dla shadera
        glEnableVertexAttribArray(0); // teraz jest 6

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1); // znormalizowane sa tutaj

        glBindVertexArray(0); // "odbindowujemy" vao zeby nic przypadkiem nie zmienic
    }

    void draw(Shader& shader) {
        shader.use();
        mat4 model = mat4(1.0f);
        model = translate(model, position);
        shader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

class Bubble { 
public: 
    vec3 color;
    float speed;
    vec3 position;
    float distFromPlayer; // to wykorzystamy do posortowania babelkow do odpowiedniego renderowania ich (od najdalej do najblizej od gracza)
    mat4 modelMatrix;
    bool isSpecial; // specialne babelki co beda dawac pkt
    float bubbleScale; 

    static unsigned int VAO, VBO, EBO; // statyczne pola ktore wykorzystamy do wspoldzielonych rzeczy miedzy babelkami
    static unsigned int modelMatrixVBO;
    static unsigned int colorVBO;
    static unsigned int isSpecialVBO;
    static vector<float> vertices;
    static vector<unsigned int> indices;

    Bubble(vec3 position, vec3 color, float speed, bool isSpecial = false) {
        this->color = color;
        this->speed = speed;
        this->position = position;
        this->isSpecial = isSpecial;
        this->bubbleScale = 1.0f;
        updateModelMatrix();
    }

    void update(float deltaTime) {
        position.y += speed * deltaTime; // z czasem babelki podnosza sie do gory

        if (position.y > CUBE_MAX) { // babelek dotarl na gore no to teraz niech pojawi sie na dole ale lekko zmienony
            position.y = CUBE_MIN;
            position.x = RandomFloat(CUBE_MIN, CUBE_MAX);
            position.z = RandomFloat(CUBE_MIN, CUBE_MAX);

            if (!isSpecial) {
                color = vec3( 
                    (float)rand() / RAND_MAX,
                    (float)rand() / RAND_MAX,
                    (float)rand() / RAND_MAX);
            }
            bubbleScale *= 1.25f; // wiekszy
        }

        updateModelMatrix();
    }

    void updateModelMatrix() {
        modelMatrix = translate(mat4(1.0f), position);
        modelMatrix = scale(modelMatrix, vec3(bubbleScale));
    }


    // tutaj mamy statyczne metody 

    static void initBubbleGeometry(float radius) {

        const int verticalSlices = 20; // tutaj mamy analogicznie do sfery kod
        const int horizontalSlices = 20;
        float x, y, z;

        float horizontalStep = M_PI / horizontalSlices;
        float verticalStep = 2 * M_PI / verticalSlices;

        for (int i = 0; i <= horizontalSlices; i++) { // wierzcholki
            float theta = i * horizontalStep;
            float sinTheta = sinf(theta);
            float cosTheta = cosf(theta);

            for (int j = 0; j <= verticalSlices; j++) {
                float phi = j * verticalStep;
                float sinPhi = sinf(phi);
                float cosPhi = cosf(phi);

                x = radius * sinTheta * cosPhi;
                y = radius * cosTheta;
                z = radius * sinTheta * sinPhi;

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                vec3 normal = normalize(vec3(x, y, z));
                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);
            }
        }

        unsigned int currSlice, nextSlice; // indeksy
        for (int i = 0; i < horizontalSlices; i++) {
            currSlice = i * (verticalSlices + 1);
            nextSlice = currSlice + verticalSlices + 1;

            for (int j = 0; j < verticalSlices; j++, currSlice++, nextSlice++) {
                if (i != 0) {
                    indices.push_back(currSlice);
                    indices.push_back(nextSlice);
                    indices.push_back(currSlice + 1);
                }

                if (i != (horizontalSlices - 1)) {
                    indices.push_back(currSlice + 1);
                    indices.push_back(nextSlice);
                    indices.push_back(nextSlice + 1);
                }
            }
        }

        glGenVertexArrays(1, &Bubble::VAO); // VAO VBO i EBO
        glGenBuffers(1, &Bubble::VBO);
        glGenBuffers(1, &Bubble::EBO);

        glBindVertexArray(Bubble::VAO);

        glBindBuffer(GL_ARRAY_BUFFER, Bubble::VBO);
        glBufferData(GL_ARRAY_BUFFER, Bubble::vertices.size() * sizeof(float), &Bubble::vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Bubble::EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Bubble::indices.size() * sizeof(unsigned int), &Bubble::indices[0], GL_STATIC_DRAW);

        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // pozycje wierzcholkow : aPos
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // znormalizowe : aNormal
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    static void setupInstancing(int maxNumBubbles) { // bufory dla model macierzy oraz kolorow

        glBindVertexArray(VAO); 

        glGenBuffers(1, &modelMatrixVBO);
        glBindBuffer(GL_ARRAY_BUFFER, modelMatrixVBO);
        glBufferData(GL_ARRAY_BUFFER, maxNumBubbles * sizeof(mat4), nullptr, GL_DYNAMIC_DRAW);

        size_t vec4Size = sizeof(vec4); // trikas
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glVertexAttribDivisor(4, 1);

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
        glVertexAttribDivisor(5, 1);

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glVertexAttribDivisor(6, 1);

        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
        glVertexAttribDivisor(7, 1);


        glGenBuffers(1, &colorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, maxNumBubbles * sizeof(vec3), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
        glVertexAttribDivisor(3, 1);


        glGenBuffers(1, &isSpecialVBO);
        glBindBuffer(GL_ARRAY_BUFFER, isSpecialVBO);
        glBufferData(GL_ARRAY_BUFFER, maxNumBubbles * sizeof(int), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(8);
        glVertexAttribIPointer(8, 1, GL_INT, sizeof(int), (void*)0); // IPointer no bo int
        glVertexAttribDivisor(8, 1);

        glBindVertexArray(0);
    }

    static void updateInstancingData(const vector<Bubble>& bubbles, int numBubbles) {

        if (numBubbles > (int)bubbles.size()) {
            numBubbles = (int)bubbles.size();
        }

        if (numBubbles > MAX_BUBBLES) {
            numBubbles = MAX_BUBBLES;
        }

        vector<mat4> modelMatrices(numBubbles);
        vector<vec3> colors(numBubbles);
        vector<int> isSpecialFlags(numBubbles); // tu byl problem

        for (int i = 0; i < numBubbles; i++) {

            modelMatrices[i] = bubbles[i].modelMatrix;
            colors[i] = bubbles[i].color;

            if(bubbles[i].isSpecial) {
                isSpecialFlags[i] = 1;
            } else {
                isSpecialFlags[i] = 0;
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, modelMatrixVBO); // aktualizujemy model
        glBufferSubData(GL_ARRAY_BUFFER, 0, numBubbles * 4 * sizeof(vec4), modelMatrices.data());

        glBindBuffer(GL_ARRAY_BUFFER, colorVBO); // aktualizujemy kolorki
        glBufferSubData(GL_ARRAY_BUFFER, 0, numBubbles * sizeof(vec3), colors.data());

        glBindBuffer(GL_ARRAY_BUFFER, isSpecialVBO); // aktualizujemy flagi
        glBufferSubData(GL_ARRAY_BUFFER, 0, numBubbles * sizeof(int), isSpecialFlags.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    static void drawBubbles(Shader& shader, int numBubbles) { // malowaine babelkow
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, numBubbles);
        glBindVertexArray(0);
    }

};

unsigned int Bubble::VAO = 0; // alokujemy sobie pamiec dla statyznych
unsigned int Bubble::VBO = 0; // pol Bubble
unsigned int Bubble::EBO = 0;
unsigned int Bubble::modelMatrixVBO = 0;
unsigned int Bubble::colorVBO = 0;
unsigned int Bubble::isSpecialVBO = 0;
vector<float> Bubble::vertices;
vector<unsigned int> Bubble::indices;



class Cube { // to jest nasza przestrzen gry : tym razem akwarium
public: 
    float CUBE_MIN, CUBE_MAX;
    unsigned int VAO, VBO, EBO;
    vector<float> vertices;
    vector<int> indices;
    bool includeRightFace; // flaga determinujaca jaki mamy widok

    Cube(float CUBE_MIN, float CUBE_MAX, bool includeRightFace = true) {
        this->CUBE_MIN = CUBE_MIN;
        this->CUBE_MAX = CUBE_MAX;
        this->includeRightFace = includeRightFace;
        buildCube();
        setBuffers();
    }

    void buildCube() {
        
        vec3 corners[8] = {
            vec3(CUBE_MIN, CUBE_MIN, CUBE_MIN), // -1 -1 -1 
            vec3(CUBE_MAX, CUBE_MIN, CUBE_MIN), // 1 -1 -1
            vec3(CUBE_MAX, CUBE_MAX, CUBE_MIN), // 1 1 -1
            vec3(CUBE_MIN, CUBE_MAX, CUBE_MIN), // -1 1 -1 
            vec3(CUBE_MIN, CUBE_MIN, CUBE_MAX), // -1 -1 1
            vec3(CUBE_MAX, CUBE_MIN, CUBE_MAX), // 1 -1 1
            vec3(CUBE_MAX, CUBE_MAX, CUBE_MAX), // 1 1 1
            vec3(CUBE_MIN, CUBE_MAX, CUBE_MAX)  // -1 1 1
        };

        vector<array<int, 4>> sides = {
            {3, 2, 1, 0}, // przod
            {6, 7, 4, 5}, // tyl
            {7, 3, 0, 4}, // lewa sciana
            {2, 6, 5, 1}, // prawa sciana
            {4, 5, 1, 0}  // dolna sciana
            // gora nas nie obchodzi to jej nie bierzemy pod uwage
        };

        vector<vec3> normals = { // wektory do środka
            vec3(0.0f, 0.0f, 1.0f), // analogicznie jak w sides (jakiej scianie odpowiadaja)
            vec3(0.0f, 0.0f, -1.0f),
            vec3(1.0f, 0.0f, 0.0f),
            vec3(-1.0f, 0.0f, 0.0f),
            vec3(0.0f, 1.0f, 0.0f)
        };

        if (!includeRightFace) { // jak chcemy miec rzut z boku akwarium to bez tej jednej scianki
            sides.erase(sides.begin() + 3);
            normals.erase(normals.begin() + 3);
        }


        vertices.clear(); // czyscimy poniewaz przy zmianie widoku no zasadniczo renderujemy troszke 
        indices.clear(); // inaczej więc aby byc safe czyscimy

        for (int i = 0; i < (int)sides.size(); i++) {
            vec3 normal = normals[i];

            for (int j = 0; j < 4; j++) {
                vec3 position = corners[sides[i][j]];

                vertices.push_back(position.x);
                vertices.push_back(position.y);
                vertices.push_back(position.z);

                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);
            }

            int baseIndex = i * 4; // dwa trojkaty na sciane
            indices.push_back(baseIndex);
            indices.push_back(baseIndex + 1);
            indices.push_back(baseIndex + 2);

            indices.push_back(baseIndex);
            indices.push_back(baseIndex + 2);
            indices.push_back(baseIndex + 3);
        }

    }

    void setBuffers() {
        glGenVertexArrays(1, &VAO); 
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO); 
        glBindBuffer(GL_ARRAY_BUFFER, VBO); 
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW); 

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()* sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); 
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1); // normalne dla swiatla

        glBindVertexArray(0); 
    }

    void draw(Shader& shader) {
        shader.use();
        mat4 model = mat4(1.0f);
        shader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};


// prototypy funkcji
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Sphere &playerSphere);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


int main(int argc, char* argv[]) {

    srand(time(0));
    
    if (!glfwInit()) { // jak jest to super
        cerr << "Error: GLFW initialization failed" << endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // GLFW konkretna wersja
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    // ogarnijmy okno
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Z5", NULL, NULL);
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
    glDisable(GL_CULL_FACE); // renderujemy obie strony akwarium


    // Tutaj odpalamy nasze shadery
    Shader cubeShader("cube_vertex.glsl", "cube_fragment.glsl"); // akwarium
    Shader sphereShader("sphere_vertex.glsl", "sphere_fragment.glsl"); // kulka gracza
    Shader bubbleShader("bubble_vertex.glsl", "bubble_fragment.glsl"); // babelki


    Cube gameCubeWithRightFace(CUBE_MIN, CUBE_MAX, true); // akwarium POV
    Cube gameCubeWithoutRightFace(CUBE_MIN, CUBE_MAX, false); // akwarium side view
    Sphere playerSphere(sphereRadius, cameraPos); // gracz


    float bubbleRadius = 0.1f; // inicjalizujemy babelki
    int initialNumBubbles = 2 * CUBE_MAX * 10; 
    int numBubbles = initialNumBubbles; 
    int numSpecialBubbles = initialNumBubbles * 0.2f; // specialne babelki beda stanowic 20% wszystkcih
    
    if(numSpecialBubbles > 100) {
        numSpecialBubbles = 100;
    }
    
    Bubble::initBubbleGeometry(bubbleRadius);
    Bubble::setupInstancing(MAX_BUBBLES);
    vector<Bubble> bubbles;
    int specialBubblesCreated = 0;

    for (int i = 0; i < numBubbles; i++) { // tutaj losowo rozstawiamy babelki na poziomie y = 0 i puszczamy je w gore
        float x = RandomFloat(CUBE_MIN, CUBE_MAX);
        float y = CUBE_MIN;
        float z = RandomFloat(CUBE_MIN, CUBE_MAX);

        bool isSpecial = false;
        if (specialBubblesCreated < numSpecialBubbles) {
            isSpecial = true;
            specialBubblesCreated++;
        }

        vec3 color;
        if (isSpecial) {
            color = vec3(1.0f, 0.2f, 0.0f); // byl rozowy ;> !! tutaj lezal problem go roznowy i.e. 1 0 1 sie srednio laczyl z zielonym 0 1 0
        } else {
            color = vec3(
            RandomFloat(0.0f, 1.0f),
            RandomFloat(0.0f, 1.0f),
            RandomFloat(0.0f, 1.0f));
        }
        

        float speed = 0.2 + RandomFloat(0.1f, 0.2f) * bubbleSpeedMultiplier; // predkosc babelkow

        bubbles.emplace_back(vec3(x, y, z), color, speed, isSpecial);
    }

    vec3 lightPos = vec3(0.0f, CUBE_MAX + 2.0f, 0.0f); // main swiatlo jest nad akwarium

    while (!glfwWindowShouldClose(window)) {
        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        if (deltaTime > 0.1f){
            deltaTime = 0.1f;
        }

        processInput(window, playerSphere);


        if(playerSphere.position.y <= CUBE_MIN + sphereRadius) { // tutaj ogarniamy "przchodzenie miedzy poziomami"
                                                                 // i zmiany miedzy "poziomami"
            playerSphere.position.y = CUBE_MAX - sphereRadius; // cofka na szczyt

            bubbleSpeedMultiplier *= 1.1f;

            currentLevel++; // "przechodzimy na nastepny poziom"

            int nextLevelBubblesCount = (int)(numBubbles * 0.05f);
            int totalBubbles = numBubbles + nextLevelBubblesCount;

            if (totalBubbles > MAX_BUBBLES) {
                nextLevelBubblesCount = MAX_BUBBLES - numBubbles;
                totalBubbles = MAX_BUBBLES;
            }

            numBubbles = totalBubbles;

            for (int i = 0; i < nextLevelBubblesCount; i++) { // dorabiamy nowe babelki 
                float x = RandomFloat(CUBE_MIN, CUBE_MAX);
                float y = CUBE_MIN;
                float z = RandomFloat(CUBE_MIN, CUBE_MAX);

                vec3 color = vec3(
                    RandomFloat(0.0f, 1.0f),
                    RandomFloat(0.0f, 1.0f),
                    RandomFloat(0.0f, 1.0f)
                );

                float speed = 0.2f + RandomFloat(0.1f, 0.2f) * bubbleSpeedMultiplier;
                bubbles.emplace_back(vec3(x, y, z), color, speed, false);
            }

            for(auto& bubble : bubbles) { // calkiem cool opcja dziala troche jak w python for obj in collection, cool
                bubble.speed *= 1.1f;
                bubble.position.y = CUBE_MIN + bubbleRadius + RandomFloat(0.0f, 0.1f);
            }

            cout << "Let's go you advanced to the " << currentLevel << " level!\n" ;
        }


        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT); 

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        int width, height;
        glfwGetFramebufferSize(window, &width, &height); 
        float aspectRatio = (float)width / (float)height; 

        mat4 projection = perspective(radians(FOV), aspectRatio, 0.1f, 100.0f);

        cameraPos = playerSphere.position; // kamera podarza za graczem (no sa w tym samym miejscu)

        cameraFront.x = cos(radians(Yaw)) * cos(radians(Pitch));
        cameraFront.y = sin(radians(Pitch));
        cameraFront.z = sin(radians(Yaw)) * cos(radians(Pitch));
        cameraFront = normalize(cameraFront);

        mat4 view;
        vec3 currentViewPos;
        if (povView) { // pov albo side view w zaleznosci od flagi
            view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            currentViewPos = cameraPos; 
        } else {
            vec3 sideCameraPos = vec3(3.5*CUBE_MAX, 0.0f, 0.0f); // jak side no to patrzymy z boku ofc
            vec3 target = vec3(0.0f, 0.0f, 0.0f);
            vec3 up = vec3(0.0f, 1.0f, 0.0f);
            view = lookAt(sideCameraPos, target, up);
            currentViewPos = sideCameraPos;
        }


        vector<vec3> lightPositions; // to bedziemy uzywac dla specjalnych babelkow
        vector<vec3> lightColors; // skoro maja oswietlac do okola siebie

        for (int i = 0; i < numBubbles; i++) { // aktualizacja babelkow

            bubbles[i].update(deltaTime);
            bubbles[i].distFromPlayer = length(bubbles[i].position - currentViewPos);

            if(bubbles[i].isSpecial) {
                lightPositions.push_back(bubbles[i].position);
                lightColors.push_back(vec3(1.0f, 0.0f, 0.0f));
            }


            float distance = length(bubbles[i].position - playerSphere.position);
            if (distance < (bubbleRadius + sphereRadius)) { // kolizja ?
                if(bubbles[i].isSpecial) {
                    points += currentLevel;
                    cout << "Wow you've collected special bubble thats " << currentLevel << " points !\n";
                    bubbles[i].position.x = RandomFloat(CUBE_MIN, CUBE_MAX);
                    bubbles[i].position.y = CUBE_MIN + bubbleRadius;
                    bubbles[i].position.z = RandomFloat(CUBE_MIN, CUBE_MAX); 
                } else {
                cout << "THE END OF THE GAME\n";
                cout << "YOU SCORED: " << points << " POINTS\n";
                glfwSetWindowShouldClose(window, 1);
                }
            }
        }

        sort(bubbles.begin(), bubbles.begin() + numBubbles, // w c++ jest sort() i jest calkiem cool
            [](const Bubble& a, const Bubble& b) { // bo jak damy mu custom comparator to pozwala robic 
                return a.distFromPlayer > b.distFromPlayer; // takie czaderskie rzeczy
            }
        );
        
        Bubble::updateInstancingData(bubbles, numBubbles); // aktualizujemy dane babelkow

        cubeShader.use();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);

        cubeShader.setVec3("viewPos", currentViewPos);

        // cubeShader.setVec3("light.position", lightPos); // tutaj ja zasadniczo niedoczytalem i dodalem oswietlenie akwarium
        // cubeShader.setVec3("light.ambient", vec3(0.3f, 0.3f, 0.3f));
        // cubeShader.setVec3("light.diffuse", vec3(0.5f, 0.5f, 0.5f));
        // cubeShader.setVec3("light.specular", vec3(0.7f, 0.7f, 0.7f));

        // cubeShader.setVec3("material.ambient", vec3(0.1f, 0.1f, 0.1f));
        // cubeShader.setVec3("material.diffuse", vec3(0.3f, 0.3f, 0.8f));
        // cubeShader.setVec3("material.specular", vec3(0.3f, 0.3f, 0.3f));
        // cubeShader.setFloat("material.shininess", 32.0f);

////////////////////////////////////////////////////////////////////////////////////
        cubeShader.setVec3("waterAbsorption", vec3(0.2f, 0.5f, 0.15f));
////////////////////////////////////////////////////////////////////////////////////

        if (!povView) {
            gameCubeWithoutRightFace.draw(cubeShader);
        } else {
            gameCubeWithRightFace.draw(cubeShader);
        }

        if (!povView) { // jak side view to malujemy nasza kulke
            sphereShader.use();
            sphereShader.setMat4("projection", projection);
            sphereShader.setMat4("view", view);
            sphereShader.setVec3("viewPos", currentViewPos);

            sphereShader.setVec3("light.position", lightPos);
            sphereShader.setVec3("light.ambient", vec3(0.2f, 0.2f, 0.2f));
            sphereShader.setVec3("light.diffuse", vec3(0.5f, 0.5f, 0.5f));
            sphereShader.setVec3("light.specular", vec3(1.0f, 1.0f, 1.0f));

            sphereShader.setVec3("material.ambient", vec3(0.0f, 1.0f, 0.0f));
            sphereShader.setVec3("material.diffuse", vec3(0.0f, 0.8f, 0.0f));
            sphereShader.setVec3("material.specular", vec3(0.5f, 0.5f, 0.5f));
            sphereShader.setFloat("material.shininess", 32.0f);

            playerSphere.draw(sphereShader);
        }

        if (transparent) { // on / off przezroczystosc babelkow
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);
        } else {
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
        }
        
        
        bubbleShader.use();
        bubbleShader.setMat4("projection", projection);
        bubbleShader.setMat4("view", view);
        bubbleShader.setVec3("viewPos", currentViewPos);

        bubbleShader.setVec3("mainLight.position", lightPos);
        bubbleShader.setVec3("mainLight.ambient", vec3(0.1f));
        bubbleShader.setVec3("mainLight.diffuse", vec3(0.2f));
        bubbleShader.setVec3("mainLight.specular", vec3(0.4f)); 

        bubbleShader.setVec3("material.ambient", vec3(1.0f, 1.0f, 1.0f));
        bubbleShader.setVec3("material.diffuse", vec3(1.0f, 1.0f, 1.0f));
        bubbleShader.setVec3("material.specular", vec3(0.5f, 0.5f, 0.5f));
        bubbleShader.setFloat("material.shininess", 32.0f);

////////////////////////////////////////////////////////////////////////////////////
        bubbleShader.setVec3("waterAbsorption", vec3(0.2f, 0.1f, 0.15f));
////////////////////////////////////////////////////////////////////////////////////


        if(transparent) {
            bubbleShader.setFloat("alpha", 0.5f);
        } else {
            bubbleShader.setFloat("alpha", 1.0f);
        }

        bubbleShader.setInt("numLights", (int)lightPositions.size());

        for (int i = 0; i < (int)lightPositions.size(); i++) { // lecimy i ustawiamy wlasciwosci specjalnych babelkow (ich swiatla)
            string index = to_string(i);

            bubbleShader.setVec3("lights[" + index + "].position", lightPositions[i]);
            bubbleShader.setVec3("lights[" + index + "].ambient", vec3(0.1f, 0.0f, 0.0f));
            bubbleShader.setVec3("lights[" + index + "].diffuse", vec3(0.3f, 0.0f, 0.0f));
            bubbleShader.setVec3("lights[" + index + "].specular", vec3(0.5f, 0.0f, 0.0f));

            bubbleShader.setFloat("lights[" + index + "].constant", bubbleConstant);
            bubbleShader.setFloat("lights[" + index + "].linear", bubbleLinear);
            bubbleShader.setFloat("lights[" + index + "].quadratic", bubbleQuadratic);
        }

        bubbleShader.setVec3("viewPos", currentViewPos);

        if (povView) { // latarka
            vec3 spotlightPos = playerSphere.position;
            vec3 spotlightDir = normalize(cameraFront);

            float cutOff = cos(radians(12.5f));
            float outerCutOff = cos(radians(17.5f));

            float constant = 1.0f;
            float linear = 0.027f;
            float quadratic = 0.0028f;

            bubbleShader.setVec3("spotlight.position", spotlightPos);
            bubbleShader.setVec3("spotlight.direction", spotlightDir);
            bubbleShader.setFloat("spotlight.cutOff", cutOff);
            bubbleShader.setFloat("spotlight.outerCutOff", outerCutOff);

            bubbleShader.setVec3("spotlight.ambient", vec3(0.0f, 0.8f, 0.0f));
            bubbleShader.setVec3("spotlight.diffuse", vec3(0.0f, 1.0f, 0.0f));
            bubbleShader.setVec3("spotlight.specular", vec3(0.0f, 1.0f, 0.0f));

            bubbleShader.setFloat("spotlight.constant", constant);
            bubbleShader.setFloat("spotlight.linear", linear);
            bubbleShader.setFloat("spotlight.quadratic", quadratic);
        } else {

            bubbleShader.setFloat("spotlight.cutOff", cos(radians(5.0f)));
            bubbleShader.setFloat("spotlight.outerCutOff", cos(radians(7.5f)));
        }

        Bubble::drawBubbles(bubbleShader, numBubbles); // rysujemy babelki ;>

        if (transparent) {
            glDepthMask(GL_TRUE);
        }   


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // sprzatamy po sobie
    glfwTerminate();
    glfwDestroyWindow(window);
    return 0;
}

// funckje 

void processInput(GLFWwindow *window, Sphere &playerSphere) {
    
    float adjustedSpeed = cameraSpeed * deltaTime;

    vec3 direction(0.0f);

    vec3 frontDir = normalize(vec3(cameraFront.x, 0.0f, cameraFront.z));

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { // przod tyl prawo lewo
        direction += frontDir * adjustedSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        direction -= frontDir * adjustedSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        direction -= normalize(cross(frontDir, cameraUp)) * adjustedSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        direction += normalize(cross(frontDir, cameraUp)) * adjustedSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { // w dol i do gory
        direction -= cameraUp * adjustedSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        direction += cameraUp * adjustedSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        FOV -= 50.0f * deltaTime;
        if (FOV < 10.0f) {
            FOV = 10.0f; // najmniejszy FOV
        }
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        FOV += 50.0f * deltaTime;
        if (FOV > 90.0f) {
            FOV = 90.0f; // najwiekszy FOV
        }
    }

    vec3 newPos = playerSphere.position + direction;

    if (newPos.x >= CUBE_MIN + sphereRadius && newPos.x <= CUBE_MAX - sphereRadius && 
        newPos.y >= CUBE_MIN  && newPos.y <= CUBE_MAX - sphereRadius && 
        newPos.z >= CUBE_MIN + sphereRadius && newPos.z <= CUBE_MAX - sphereRadius) {
        playerSphere.position = newPos; // jak mozna to mozna
    }


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        povView = !povView;
        if (!povView) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true; 
        }
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        transparent = !transparent;
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

float RandomFloat(float min, float max) { // https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    float r = (float)rand() / (float)RAND_MAX;
    return min + r * (max - min);
}


