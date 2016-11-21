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

 
Model::Model() 
    : modelVertexBufferData{
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
    },
    modelColorBufferData{
         1.0f, 0.0f, 0.0f, // triangle 1 : begin
         1.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f, // triangle 1 : end
         0.0f, 1.0f, 0.0f, // triangle 2 : begin
         0.0f, 1.0f, 0.0f,
         0.0f, 1.0f, 0.0f, // triangle 2 : end
         0.5f, 0.5f, 0.0f, // TOP 1
         0.5f, 0.5f, 0.0f,
         0.5f, 0.5f, 0.0f,
         0.0f, 1.0f, 0.0f,
         0.0f, 1.0f, 0.0f,
         0.0f, 1.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
         0.5f, 0.5f, 0.0f, // TOP 1
         0.5f, 0.5f, 0.0f,
         0.5f, 0.5f, 0.0f,
         0.0f, 0.5f, 0.5f,
         0.0f, 0.5f, 0.5f,
         0.0f, 0.5f, 0.5f,
         0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f,
         0.5f, 0.0f, 0.5f, // TOP 2
         0.5f, 0.0f, 0.5f,
         0.5f, 0.0f, 0.5f,
         0.5f, 0.0f, 0.5f, // TOP 2
         0.5f, 0.0f, 0.5f,
         0.5f, 0.0f, 0.5f,
         0.0f, 0.5f, 0.5f,
         0.0f, 0.5f, 0.5f,
         0.0f, 0.5f, 0.5f       
    },
    vertexBuffer(0), colorBuffer(0), buffersCreated(0) {

    vertexBuffer = 0;
    colorBuffer = 0;
    n_vertex = 3*12;
}

Model::~Model() {
    
    if (buffersCreated == 1) {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &colorBuffer);     
    }
}


glm::mat4 Model::getModelMatrix() {
    return modelMatrix;
}


GLuint Model::getVertexBuffer() {
    return vertexBuffer;
}


GLuint Model::getColorBuffer() {
    return colorBuffer;
}


int Model::getNumVertex() {
    return n_vertex;
}


void Model::createBuffers() {
    
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(modelVertexBufferData), modelVertexBufferData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(modelColorBufferData), modelColorBufferData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );
    buffersCreated = 1;
    
}

void Model::transformModel(glm::vec3 rotation, glm::vec3 translation) {
    
    glm::mat4 rotationMatrix = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
    
    modelMatrix = translationMatrix * rotationMatrix * modelMatrix;
}
