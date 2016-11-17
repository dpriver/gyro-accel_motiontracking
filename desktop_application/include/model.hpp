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
 
 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
 
#define SHADER_DIR "../resources/shaders/"
 
class model {
    
    private:
    mat4 modelMatrix;
    GLuint vertexBuffer;
    GLuint colorBuffer;
    int n_vertex;
    
    GLuint VertexArrayID;
    GLuint shaderID;
    
    
    public:
    model();
     
    glm::mat4 get_model_matrix();
    GLuint get_vertex_buffer();
    GLuint get_color_buffer();
    int get_n_vertex();
     
    void rotate_model();
    void translate_model();
}

#endif /* __MODEL_CUBE_HPP */

