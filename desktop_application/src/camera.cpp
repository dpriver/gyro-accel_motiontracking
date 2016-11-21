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

 

#include "camera.hpp"
 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

 
Camera::Camera() {
    projectionMatrix = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
    viewMatrix = glm::lookAt(glm::vec3(0,0,5), glm::vec3(0,0,0), glm::vec3(0,1,0));
}

Camera::~Camera() {
    
}

mat4 Camera::calculateMVP(mat4 modelMatrix) {
    
    return projectionMatrix * viewMatrix * modelMatrix;
}
 
void Camera::rotateCamera() {
    viewMatrix = viewMatrix;
}

void Camera::translateCamera() {
    viewMatrix = viewMatrix;
}

