///
//  Utils.h
//
//  Various utility functions.
//
//  Created by Warren R. Carithers on 2019/11/18.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This code can be compiled as either C or C++.
//
//  This file should not be modified by students.
///

#ifndef _UTILS_H_
#define _UTILS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

///
// OpenGL error checking
//
// Can be called at any time to query the internal OpenGL error state,
// and reports all outstanding OpenGL error conditions at the time of
// the call.  If used periodically within an OpenGL application, this
// can help pinpoint the OpenGL API call(s) that encountered an error.
//
// @param msg  message prefix to print with each error message
///
void checkErrors( const char *msg );

///
// Convert a type number to a string.
//
// Used to decode GLSL data type numbers when printing error messages.
// Decodes the most commonly-used types; others are printed as integer
// strings.  (In the latter case, the return value points to a static
// character array that may be overwritten in subsequent calls.)
//
// @param num  the type
// @return a string containing a text description
///
const char *type2str( GLenum num );

///
// Dump the list of active attributes and uniforms from a shader program
//
// @param program  which shader program to query
///
void dumpActives( GLuint program );

///
// Retrieve a Uniform variable's location and verify the result
//
// @param program  the shader program
// @param name     the name of the desired variable
///
GLint getUniformLoc( GLuint program, const GLchar *name );

///
// Retrieve an Attribute variable's location and verify the result
//
// @param program  the shader program
// @param name     the name of the desired variable
///
GLint getAttribLoc( GLuint program, const GLchar *name );

#endif
