#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "graphics.hpp"
#include "camera.hpp"
#include "model.hpp"


#define DEVICE_FILE "/dev/ttyACM0"


int main( int argc, char **argv ) {

    GraphicManager graphicManager;

    FILE *device_file;
    printf("Path: %s\n", argv[0]);
    
    
    //device_file = fopen(DEVICE_FILE, "r");
    //if (device_file == NULL) {
    //    fprintf(stderr, "Failed to open device input file\n");
    //    return 0;
    //}
    
/*******************************************************************************
 *  Init Environment and context
 ******************************************************************************/
    
    if (graphicManager.initGraphics() != 0) {
        fprintf(stderr, "Could not initialize the graphic system.");
        return -1;
    }


/*******************************************************************************
 *  Main loop
 ******************************************************************************/
    double last_time = glfwGetTime();
    double current_time;
    double time_difference;
    //do {
    //    current_time = glfwGetTime();
    //    char line[300];
    //    fgets(line, 300, device_file);
        
    //} while( (glfwGetTime() - last_time) < 1);
 
 
	last_time = glfwGetTime();
    do{
        current_time = glfwGetTime();
        time_difference = current_time - last_time;
        last_time = current_time;
        
        vec3 rotation;
        vec3 translation;
        
        rotation.y = -(float)0 * (3.14/180) * time_difference;
        rotation.x = (float)0 * (3.14/180) * time_difference;
        rotation.z = (float)90 * (3.14/180) * time_difference;
        
        translation.y = 0 * time_difference;
        translation.x = -2 * time_difference;
        translation.z = 0;
        
        graphicManager.transformModel(rotation, translation);

        // Draw scene
        graphicManager.drawGraphics();
	
    // Check if the exit conditions are accomplished
    } while (!graphicManager.exit());


/*******************************************************************************
 *  Cleanup
 ******************************************************************************/

    
    return 0;
}
