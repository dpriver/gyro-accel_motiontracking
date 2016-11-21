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
 
#ifndef __MODEL_CUBE_HPP
#define __MODEL_CUBE_HPP
 
 
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/transform.hpp>
 
class Model {
    
    private:
    glm::mat4 modelMatrix;
    GLfloat modelVertexBufferData[36*3];
    GLfloat modelColorBufferData[36*3];
    GLuint vertexBuffer;
    GLuint colorBuffer;
    int n_vertex;
    int buffersCreated;
    
    
    public:
    Model();
    ~Model();
     
    glm::mat4 getModelMatrix();
    GLuint getVertexBuffer();
    GLuint getColorBuffer();
    int getNumVertex();
     
    void createBuffers();
    void transformModel(glm::vec3 rotation, glm::vec3 translation);
};

#endif /* __MODEL_CUBE_HPP */

