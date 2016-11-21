/*******************************************************************************
 *	graphics.cpp
 *
 *  graphic related functionality
 *
 * 
 *  Copyright (C) 2016  Dennis Pinto Rivero
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include "graphics.hpp"


GraphicManager::GraphicManager() : init(0) {
    
}


GraphicManager::~GraphicManager() {
    if (init == 1) {
        glDeleteVertexArrays(1, &vertex_array_ID);
        glDeleteProgram(programID);
        glfwTerminate();
    }
}


int GraphicManager::initGraphics() {
    
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
        
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        
    glGenVertexArrays(1, &vertex_array_ID);

    // Load vertex and fragment shaders
    programID = LoadShaders( SHADER_DIR "SimpleVertexShader.vertexshader", SHADER_DIR "SimpleFragmentShader.fragmentshader" );
    
    // Get location of shader uniform "MVP"
    model_mvp_uniform = glGetUniformLocation(programID, "MVP");
    
    
    glBindVertexArray(vertex_array_ID);
    model.createBuffers();
    
    glBindVertexArray(0);
    
    init = 1;
        
    return 0;
}


void GraphicManager::drawGraphics() {
    
    glm::mat4 model_MVP;
    
    // Bind VAO
    glBindVertexArray(vertex_array_ID);
    
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram(programID);
    
    // Calculate the model MVP    
    model_MVP = camera.calculateMVP(model.getModelMatrix());
    
    // Pass the model MVP to the shaders
    glUniformMatrix4fv(model_mvp_uniform, 1, GL_FALSE, &model_MVP[0][0]);
    
    glDrawArrays(GL_TRIANGLES, 0, model.getNumVertex());
    
    
    glfwSwapBuffers(window);
    glBindVertexArray(0);
}


void GraphicManager::transformModel(vec3 rotation, vec3 translation) {
    model.transformModel(rotation, translation);
}


bool GraphicManager::exit() {
    glfwPollEvents();
    
    return (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) ||
        (glfwWindowShouldClose(window) != 0);
}
