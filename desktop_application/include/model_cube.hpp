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
 
class model_cube {
    
    private:
    mat4 modelMatrix;
    GLuint vertexBuffer;
    GLuint colorBuffer;
    
    
    public:
    /* model_cube();
     * 
     * get_model_matrix();
     * get_vertex_buffer();
     * get_color_buffer();
     * 
     * rotate_model();
     * translate_model();
     */
    
}



#endif /* __MODEL_CUBE_HPP */
