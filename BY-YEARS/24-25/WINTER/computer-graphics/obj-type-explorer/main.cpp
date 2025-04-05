#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

#include "shader.h"
#include "model.h"

#include <iostream>

using namespace std;
using namespace glm;

vec3 cameraPos = vec3(0.0f, 0.0f, 10.0f); /// setup kamery
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);


float orbitYaw = 0.0f; // tutaj mamy dla "orbitalnej kamety"
float orbitPitch = 0.0f;
float orbitDist = 0.0f;

float cameraSpeed = 100.0f;
// float mouseSensitivity = 0.1f;
float sensitivity = 1.0f;
float moveSpeed = 1.0f;

vec3 bboxCenter = vec3(0.0f);
bool drawBounding = true;

float FOV = 45.0f; // field of view

bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;

unsigned int WIN_WIDTH = 800;
unsigned int WIN_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float amp = 1.0f; // tu mamy zmienne do szumu
float freq = 1.0f; 
float Red = 0.0f;
float Green = 0.0f;
float Blue = 0.0f;
float Alpha = 0.0f;

class BoundingBox { // tutaj doslownie nic specjlanego po prostu malowanie lini
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    vec3 minCorner;
    vec3 maxCorner;
    vec3 color;

public:
    BoundingBox(vec3& minC, vec3& maxC, vec3 c) {
        VAO = 0;
        VBO = 0;
        EBO = 0;
        minCorner = minC;
        maxCorner = maxC;
        color = c;
        setupBuffers(); // odrazu sobie setupujemy wszystko
    }

    ~BoundingBox() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void setupBuffers() {
        float vertices[] = {
            minCorner.x, minCorner.y, minCorner.z,
            minCorner.x, minCorner.y, maxCorner.z,
            minCorner.x, maxCorner.y, minCorner.z,
            minCorner.x, maxCorner.y, maxCorner.z,
            maxCorner.x, minCorner.y, minCorner.z,
            maxCorner.x, minCorner.y, maxCorner.z,
            maxCorner.x, maxCorner.y, minCorner.z,
            maxCorner.x, maxCorner.y, maxCorner.z
        };

        int boundingBoxIndices[] = {
            0, 1, 0, 2, 0, 4,
            1, 3, 1, 5,
            2, 3, 2, 6,
            3, 7,
            4, 5, 4, 6,
            5, 7, 6, 7
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boundingBoxIndices), boundingBoxIndices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }

    void draw(Shader& boundingBoxShader, mat4& projection, mat4& view, mat4& model) {

        boundingBoxShader.use();
        boundingBoxShader.setMat4("projection", projection);
        boundingBoxShader.setMat4("view", view);
        boundingBoxShader.setMat4("model", model);
        boundingBoxShader.setVec3("boxColor", color);

        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

void updateOrbitCamera();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main(int argc, char* argv[]) {

    bool catalogFlagFound = false;
    string modelName = "";

    for(int i = 1; i < argc; ++i) {
        string arg = argv[i];

        if (arg == "-catalog") {
            catalogFlagFound = true;
            if (i + 1 < argc) {
                modelName = argv[i + 1];
                ++i; 
            } else {
                cout << "Error: '-catalog' flag provided but no model name specified.\n";
                return -1;
            }
        } 
        else if (arg == "-amp" && i + 1 < argc) {
            amp = stof(argv[++i]);
        }
        else if (arg == "-freq" && i + 1 < argc) {
            freq = stof(argv[++i]);
        } 
        else if (arg == "-r" && i + 1 < argc) {
            Red = stof(argv[++i]);
        } 
        else if (arg == "-g" && i + 1 < argc) {
            Green = stof(argv[++i]);
        } 
        else if (arg == "-b" && i + 1 < argc) {
            Blue = stof(argv[++i]);
        } 
        else if (arg == "-a" && i + 1 < argc) {
            Alpha = stof(argv[++i]);
        }
        else {
            cout << "Warning: Unrecognized or incomplete argument: " << arg << endl;
        }
    }

    // cout << amp << " " << freq << " " << Red << " " << Green << " " << Blue << " " << Alpha << endl;
     
    if (!catalogFlagFound || modelName.empty()) {
        cout << "Usage: " << argv[0] << " -catalog <modelName> "
                  << "[-amp <float>] [-freq <float>] [-r <float>] [-g <float>] [-b <float>] [-a <float>]\n";
        return -1;
    }

    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "zad7", NULL, NULL);
    
    if (!window) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    lastX = WIN_WIDTH / 2.0f;
    lastY = WIN_HEIGHT / 2.0f;

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    stbi_set_flip_vertically_on_load(true); // flipujemy na y

    glEnable(GL_DEPTH_TEST);

    string modelPath = "objs/" + modelName + "/" + modelName + ".obj";

    if (!filesystem::exists(modelPath)) {
        cout << "Error: The specified model file does not exist: " << modelPath << endl;
        return -1;
    }

    Model ourModel(modelPath);

    bboxCenter = ourModel.getBoundingBoxCenter();
    vec3 bboxSize = ourModel.getBoundingBoxSize();

    // tutaj robimy lekko inny approach bedziemy mieli bounding box sphere 
    // powinno pomoc z problematycznymi modelami ktore sa wysokie ale 
    // cienutkie i krance sie nie mieszcza np.: piesek

    float radius = 0.5f * length(bboxSize); // promien sfery

    float aspect = float(WIN_WIDTH) / float(WIN_HEIGHT);

    float verticalFOV = radians(FOV); // katy jak kamera widzi "prostokat"
    float horizontalFOV = 2.0f * atanf(aspect * tan(verticalFOV * 0.5f));

    float distVert = radius / sin(verticalFOV * 0.5f); // odleglosc kamey
    float distHorz = radius / sin(horizontalFOV * 0.5f);
    orbitDist = std::max(distVert, distHorz);

    cameraPos = bboxCenter + vec3(0.0f, 0.0f, orbitDist);
    cameraFront = normalize(bboxCenter - cameraPos);

    vec3 offset = cameraPos - bboxCenter;
    float distXZ = length(vec2(offset.x, offset.z));
    orbitPitch = degrees(atan2(offset.y, distXZ)); 
    float rawYaw = atan2(offset.z, offset.x);
    orbitYaw = degrees(rawYaw);

    vec3 boundingBoxMin = ourModel.getBoundingBoxMin();
    vec3 boundingBoxMax = ourModel.getBoundingBoxMax();
    BoundingBox boundingBox(boundingBoxMin, boundingBoxMax, vec3(1.0f, 1.0f, 1.0f));

    Shader ourShader("modelLoadingVertex.glsl", "modelLoadingFragment.glsl");
    Shader boundingBoxShader("boundingBoxVertex.glsl", "boundingBoxFragment.glsl");

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        ourShader.setFloat("amp", amp);
        ourShader.setFloat("freq", freq);
        ourShader.setFloat("Red", Red);
        ourShader.setFloat("Green", Green);
        ourShader.setFloat("Blue", Blue);
        ourShader.setFloat("Alpha", Alpha);

        mat4 projection = perspective(radians(FOV), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 10000.0f);
        ourShader.setMat4("projection", projection);

        mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        mat4 model = mat4(1.0f);
        model = translate(model, vec3(0.0f, 0.0f, 0.0f)); 
        model = scale(model, vec3(1.0f));  
        ourShader.setMat4("model", model);

        ourModel.Draw(ourShader);

        if(drawBounding) {
            boundingBox.draw(boundingBoxShader, projection, view, model);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void updateOrbitCamera() {

    float yawRad = radians(orbitYaw);
    float pitchRad = radians(orbitPitch);

    float cosPitch = cos(pitchRad);
    vec3 offset;
    offset.x = orbitDist * cosPitch * cos(yawRad);
    offset.y = orbitDist * sin(pitchRad);
    offset.z = orbitDist * cosPitch * sin(yawRad);

    cameraPos = bboxCenter + offset;
    cameraFront = normalize(bboxCenter - cameraPos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {

        float velocity = cameraSpeed * deltaTime * moveSpeed;

        switch (key) {

            case GLFW_KEY_ESCAPE:
                if (action == GLFW_PRESS) { 
                    glfwSetWindowShouldClose(window, true);
                }
                break;
            case GLFW_KEY_W:
                orbitDist -= velocity;
                if (orbitDist < 1.0f) {
                    orbitDist = 1.0f;
                }
                updateOrbitCamera();
                break;
            case GLFW_KEY_S:
                orbitDist += velocity;
                updateOrbitCamera();
                break;
            case GLFW_KEY_UP:
                orbitPitch -= sensitivity;
                if(orbitPitch > 89.0f) {
                    orbitPitch = 89.0f;
                } else if(orbitPitch < -89.0f) {
                    orbitPitch = -89.0f;
                }
                updateOrbitCamera();
                break;
            case GLFW_KEY_DOWN:
                orbitPitch += sensitivity;
                if(orbitPitch > 89.0f) {
                    orbitPitch = 89.0f;
                } else if(orbitPitch < -89.0f) {
                    orbitPitch = -89.0f;
                }
                updateOrbitCamera();
                break;
            case GLFW_KEY_LEFT:
                orbitYaw -= sensitivity;
                updateOrbitCamera();
                break;
            case GLFW_KEY_RIGHT:
                orbitYaw += sensitivity;
                updateOrbitCamera();
                break;
            case GLFW_KEY_EQUAL:
            case GLFW_KEY_KP_ADD:
                moveSpeed *= 1.1f;
                break;
            case GLFW_KEY_MINUS:
            case GLFW_KEY_KP_SUBTRACT:
                moveSpeed *= 0.9f;
                break;
            case GLFW_KEY_SPACE:
                if (action == GLFW_PRESS) {
                    drawBounding = !drawBounding;
                }
                break;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

    WIN_WIDTH = width;
    WIN_HEIGHT = height;

    glViewport(0, 0, width, height);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) { 
    // koncowo nieuzywane
}

