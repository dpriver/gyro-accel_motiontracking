#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include "shader.hpp"
#include "controls.hpp"

#define SHADER_DIR "../resources/shaders/"
#define DEVICE_FILE "/dev/ttyACM0"


int main( int argc, char **argv ) {

    FILE *device_file;
    int gyroX, gyroY, gyroZ;
    int accelX, accelY, accelZ;
    printf("Path: %s\n", argv[0]);
    
    
    device_file = fopen(DEVICE_FILE, "r");
    if (device_file == NULL) {
        fprintf(stderr, "Failed to open device input file\n");
        return -1;
    }
    
/*******************************************************************************
 *  Init Environment and context
 ******************************************************************************/
    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Cube", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it is closer to the camera than the former one
    glDepthFunc(GL_LESS);

/*******************************************************************************
 *  Create model
 ******************************************************************************/
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( SHADER_DIR "SimpleVertexShader.vertexshader", SHADER_DIR "SimpleFragmentShader.fragmentshader" );


    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat g_cube_vertex_buffer_data[] = {
        -0.2f,-0.2f,-0.2f, // triangle 1 : begin
        -0.2f,-0.2f, 0.2f,
        -0.2f, 0.2f, 0.2f, // triangle 1 : end
         0.2f, 0.2f,-0.2f, // triangle 2 : begin
        -0.2f,-0.2f,-0.2f,
        -0.2f, 0.2f,-0.2f, // triangle 2 : end
         0.2f,-0.2f, 0.2f,
        -0.2f,-0.2f,-0.2f,
         0.2f,-0.2f,-0.2f,
         0.2f, 0.2f,-0.2f,
         0.2f,-0.2f,-0.2f,
        -0.2f,-0.2f,-0.2f,
        -0.2f,-0.2f,-0.2f,
        -0.2f, 0.2f, 0.2f,
        -0.2f, 0.2f,-0.2f,
         0.2f,-0.2f, 0.2f,
        -0.2f,-0.2f, 0.2f,
        -0.2f,-0.2f,-0.2f,
        -0.2f, 0.2f, 0.2f,
        -0.2f,-0.2f, 0.2f,
         0.2f,-0.2f, 0.2f,
         0.2f, 0.2f, 0.2f,
         0.2f,-0.2f,-0.2f,
         0.2f, 0.2f,-0.2f,
         0.2f,-0.2f,-0.2f,
         0.2f, 0.2f, 0.2f,
         0.2f,-0.2f, 0.2f,
         0.2f, 0.2f, 0.2f,
         0.2f, 0.2f,-0.2f,
        -0.2f, 0.2f,-0.2f,
         0.2f, 0.2f, 0.2f,
        -0.2f, 0.2f,-0.2f,
        -0.2f, 0.2f, 0.2f,
         0.2f, 0.2f, 0.2f,
        -0.2f, 0.2f, 0.2f,
         0.2f,-0.2f, 0.2f
    };

    // One color for each vertex. They were generated randomly.
    static const GLfloat g_cube_color_buffer_data[] = {
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_vertex_buffer_data), g_cube_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_color_buffer_data), g_cube_color_buffer_data, GL_STATIC_DRAW);


/*******************************************************************************
 *  Create MVP matrix
 ******************************************************************************/
    // get a handle for our MVP uniform
    GLuint cubeMatrixID = glGetUniformLocation(programID, "MVP");

    glm::mat4 modelMatrix = glm::mat4(1.0);

/*******************************************************************************
 *  Main loop
 ******************************************************************************/
	double last_time = glfwGetTime();
    
    do{
        double current_time = glfwGetTime();
        float deltaTime = current_time - last_time;
        last_time = current_time;
        
        fscanf(device_file, "%d, %d, %d, %d, %d, %d", &gyroX, &gyroY, &gyroZ, &accelX, &accelY, &accelZ);
        
        vec3 orientation;
        
        orientation.y = 3.144159f/2.0 * deltaTime;
        orientation.x = 0;
        orientation.z = 0;
        
        mat4 rotationMatrix = eulerAngleYXZ(orientation.y, orientation.x, orientation.z);
        modelMatrix = rotationMatrix * modelMatrix;
        // Get data from controller
        // calculate rotation and translation
        // rotate and translate modelMatrix;

        computeMatricesFromInputs();
        glm::mat4 projectionMatrix = getProjectionMatrix();
        glm::mat4 viewMatrix = getViewMatrix();
        glm::mat4 cubeMVP = projectionMatrix * viewMatrix * modelMatrix;

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

        glUniformMatrix4fv(cubeMatrixID, 1, GL_FALSE, &cubeMVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3*12); // 3 indices starting at 0 -> 12 triangle
		glDisableVertexAttribArray(0);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );


/*******************************************************************************
 *  Cleanup
 ******************************************************************************/
	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
    
    return 0;
}
