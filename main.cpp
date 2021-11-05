///
//  main
//
//  Main program for assignments.
//
//  Created by Warren R. Carithers on 02/27/14.
//  Updates: 2019/09/09 by wrc.
//  Based on earlier versions created by Joe Geigel and Warren R. Carithers
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This code can be compiled as either C or C++.
//
//  This file should not be modified by students.
///

#ifdef __cplusplus
#include <cstdlib>
#include <iostream>
#else
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//
// GLEW and GLFW header files also pull in the OpenGL definitions
//

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Application.h"

#ifdef __cplusplus
using namespace std;
#endif

///
// Event callback routines
//
// We define a general GLFW callback routine; all others must be
// defined and registered in the assignment-specific code.
///

///
// Error callback for GLFW
///
void glfwError( int code, const char *desc )
{
#ifdef __cplusplus
    cerr << "GLFW error " << code << ": " << desc << endl;
#else
    fprintf( stderr, "GLFW error %d: %s\n", code, desc );
#endif
    exit( 2 );
}


///
// Main program for this assignment
///
int main( int argc, char *argv[] )
{
    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
#ifdef __cplusplus
        cerr << "Can't initialize GLFW!" << endl;
#else
        fputs( "Can't initialize GLFW!\n", stderr );
#endif
        exit( 1 );
    }

    // w_width, w_height, and w_title come from the Application module
    w_window = glfwCreateWindow( w_width, w_height, w_title, NULL, NULL );

    if( !w_window ) {
#ifdef __cplusplus
        cerr << "GLFW window create failed!" << endl;
#else
        fputs( "GLFW window create failed!\n", stderr );
#endif
	glfwTerminate();
	exit( 1 );
    }

    glfwMakeContextCurrent( w_window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
#ifdef __cplusplus
        cerr << "GLEW error: " << glewGetErrorString(err) << endl;
#else
        fprintf( stderr, "GLEW error: %s\n", glewGetErrorString(err) );
#endif
        glfwTerminate();
        exit( 1 );
    }

    if( !GLEW_VERSION_3_0 ) {
#ifdef __cplusplus
        cerr << "OpenGL 3.0 not available" << endl;
#else
        fputs( "GLEW: OpenGL 3.0 not available\n", stderr );
#endif
	if( !GLEW_VERSION_2_1 ) {
#ifdef __cplusplus
	    cerr << "OpenGL 2.1 not available, either!" << endl;
#else
            fputs( "GLEW: OpenGL 2.1 not available, either!\n", stderr );
#endif
            glfwTerminate();
            exit( 1 );
	}
    }
#endif

    // determine whether or not we can use GLSL 1.30
    gl_maj = glfwGetWindowAttrib( w_window, GLFW_CONTEXT_VERSION_MAJOR );
    gl_min = glfwGetWindowAttrib( w_window, GLFW_CONTEXT_VERSION_MINOR );

#ifdef __cplusplus
    cerr << "GLFW: using " << gl_maj << "." << gl_min << " context" << endl;
#else
    fprintf( stderr, "GLFW: using %d.%d context\n", gl_maj, gl_min );
#endif

    // do all application-specific work
    application( argc, argv );

    // all done - shut everything down cleanly
    glfwDestroyWindow( w_window );
    glfwTerminate();

    return 0;
}
