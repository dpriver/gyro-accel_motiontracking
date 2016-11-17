

#include <GLFW/glfw3.h>
extern GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;


// Initial position: 5 units on +Z
glm::vec3 position = glm::vec3(0,0,5);

// Initial horizontal angle: toward -Z
float horizontalAngle = 3.14f;

// Initial vertical angle: none
float verticalAngle = 0.0f;

// Initial field of view: 45 degrees
float initialFoV = 45.0f;

float speed = 3.0f;
float mouseSpeed = 0.005f;


glm::mat4 getViewMatrix() {
    return viewMatrix;
}

glm::mat4 getProjectionMatrix() {
    return projectionMatrix;
}


void computeMatricesFromInputs() {
 
    static double lastTime = glfwGetTime();
    
    // time difference between frames
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    
    // get mouse position
    double xpos, ypos;
    int winWidth, winHeight;
    glfwGetWindowSize(window, &winWidth, &winHeight);
    
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, winWidth/2, winHeight/2);
    
    horizontalAngle += mouseSpeed * float(winWidth/2 - xpos);
    verticalAngle += mouseSpeed * float(winHeight/2 - ypos);
    
    // Direction: Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction (
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );
    
    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );
    
    // up vector
    glm::vec3 up = glm::cross(right, direction);
    
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }
    
    float FoV = initialFoV;
    
    projectionMatrix = glm::perspective(FoV, 4.0f/3.0f, 0.1f, 100.0f);
    viewMatrix = glm::lookAt(position, position+direction, up);
    
    lastTime = currentTime;
    
    
}
