
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "application.h"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main( void )
{
    GLFWwindow* window;

    glfwSetErrorCallback( error_callback );

    if( !glfwInit() )
    {
        exit( EXIT_FAILURE );
    }

    window = glfwCreateWindow( 800, 600, "Nav EX v1.0", NULL, NULL );
    if( !window )
    {
        glfwTerminate();
        exit( EXIT_FAILURE );
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback( window, key_callback );

    // Application.
    Application app;
    app.Initialise();

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);

        glClear( GL_COLOR_BUFFER_BIT );

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();

        gluPerspective( 3.1415f, ratio, 1.0f, 3000.0f );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();

        // Set a background color.  
        glClearColor(0.25f, 0.25f, 0.25f, 0.0f);  
        
        gluLookAt( -1.0f, 1000.0f, 0.0f, /* eye */
                   0.0f, 0.0f, 0.0f,     /* center of vision */
                   0.0f, 1.0f, 0.0f );   /* up vector */

        app.Update();
        app.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    app.~Application();
    
    glfwDestroyWindow( window );

    glfwTerminate();
    exit( EXIT_SUCCESS );
}
