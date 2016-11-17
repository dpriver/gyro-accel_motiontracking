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
 
#include "model.hpp"

 
model::model() {
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint shaderID = LoadShaders( SHADER_DIR "SimpleVertexShader.vertexshader", SHADER_DIR "SimpleFragmentShader.fragmentshader" );


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
}

glm::mat4 model::get_model_matrix() {
    return modelMatrix;
}

GLuint model::get_vertex_buffer() {
    return vertexBuffer;
}

GLuint model::get_color_buffer() {
    return colorBuffer;
}

int model::get_n_vertex() {
    return n_vertex;
}

void model::rotate_model() {
    
    
}

void model::translate_model() {
    
    
}
