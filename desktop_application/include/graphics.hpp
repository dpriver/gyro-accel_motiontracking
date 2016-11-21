/*******************************************************************************
 *	graphics.hpp
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

#ifndef __GRAPHICS_HPP
#define __GRAPHICS_HPP

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

#include "model.hpp"
#include "camera.hpp"

#define SHADER_DIR "../resources/shaders/"

class GraphicManager {
    
    private:
    int init;
    GLFWwindow* window;
    GLuint vertex_array_ID;
    GLuint programID;
    GLuint model_mvp_uniform;

    // Model to represent the virtual device
    Model model;
    Camera camera;
    
    public:
    GraphicManager();
    ~GraphicManager();
    
    int initGraphics();
    void drawGraphics();
    void transformModel(vec3 rotation, vec3 translation);
    bool exit();
};

#endif /* __GRAPHICS_HPP */
