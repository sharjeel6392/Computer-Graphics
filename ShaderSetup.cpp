///
//  ShaderSetup
//
//  Sets up a GLSL shader based on supplied source files.
//
//  Based on code from www.lighthouse3d.com
//
//  This code can be compiled as either C or C++.
//
//  This module provides a simple way to create GLSL shader programs.
//  It has three primary entry points:
//
//      shaderSetup(vsfile,fsfile,err)
//      shaderSetupStr(vsstr,fsstr,err)
//          These two functions take C-style strings as the first two
//          parameters.  For shaderSetup(), these are pathnames of files
//          containing the GLSL code for the vertex and fragment shaders;
//          for shaderSetupStr(), they are strings containing the GLSL
//          code.  Both will create two shader objects, attach the source
//          code to them, and compiler them; next, they create a program
//          object, attached the shader objects, and link the program.
//
//      shaderCreate(src,type,err)
//      shaderLink(ids,num,err)
//          These functions provide more flexibility.  The first parameter
//          to shaderCreate() is an argv-style array of string pointers; a
//          shader object of the specified type is created, all the source
//          strings are attached to it, and it is compiled.  This allows
//          creation of any type of shader (e.g., geometry), but doesn't
//          do any linking of the result into a shader program.  The
//          shaderLink() function takes a collection of shader object ids,
//          creates a shader program object, attaches all the shader objects
//          to it, and links the result.
///

#ifdef __cplusplus
#include <cstdio>
#include <cstdlib>
#include <cstring>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "ShaderSetup.h"
#include "Utils.h"

///
// readTextFile(name)
//
// Read the text file given as 'name'.
//
// Returns the contents of the file in a dynamically-allocated
// string buffer, or NULL if an error occurs.
///
GLchar *readTextFile( const char *name ) {
    FILE *fp;
    GLchar *content = NULL;
    long count = 0;

    if( name != NULL ) {

        // Attempt to open the file
        // On Windows systems, may want to use "rt" here
        fp = fopen( name, "r" );
        if( fp != NULL ) {

            // Determine its length
            fseek( fp, 0, SEEK_END );
            count = ftell( fp );
            rewind( fp );

            if( count > 0 ) {

                // Allocate the string buffer
#ifdef __cplusplus
                content = new GLchar[ sizeof(GLchar) * (count+1) ];
#else
                content = (GLchar *) malloc( sizeof(GLchar) * (count+1) );
#endif
		if( content == NULL ) {
		    fprintf( stderr, "readTextFile: alloc failed\n" );
		    return( NULL );
		}

                // Read the file into the buffer
                count = fread( content, sizeof(GLchar), count, fp );
                // Add a NUL terminator
                content[count] = '\0';
            }

            fclose(fp);
        } else {
            perror( name );
        }
    } else {
        fprintf( stderr, "readTextFile:  no file name specified\n" );
    }

    return content;

}

///
// printShaderInfoLog(shader)
//
// Print the information log from a shader compilation attempt
///
void printShaderInfoLog( GLuint shader ) {
    GLint length = 0;
    GLsizei nchars  = 0;
    char *log;

    // Determine the length of the information log
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &length );

    if( length > 0 ) {

        // Allocate a buffer for the log
#ifdef __cplusplus
        log = new char[ length ];
#else
        log = (char *) malloc( length );
#endif

        if( log != NULL ) {

            // Retrieve the log
            glGetShaderInfoLog( shader, length, &nchars, log );

            // Report it
            if( log[0] != '\0' ) {
                printf( "Shader log:\n%s\n", log );
            }

#ifdef __cplusplus
            delete [] log;
#else
            free( log );
#endif

        } else {

            fprintf( stderr, "Shader log non-empty, but allocate failed\n" );

        }
    }

}

///
// printProgramInfoLog(shader)
//
// Print a program information log
//
// This is identical to printShaderInfoLog(), except that it uses
// glGetProgramiv() and glGetProgramInfoLog() instead of the *Shader*()
// versions.
///
void printProgramInfoLog( GLuint shader ) {
    GLint length = 0;
    GLsizei nchars  = 0;
    char *log;

    // Determine the length of the information log
    glGetProgramiv( shader, GL_INFO_LOG_LENGTH, &length );

    if( length > 0 ) {

        // Allocate a buffer for the log
#ifdef __cplusplus
        log = new char[ length ];
#else
        log = (char *) malloc( length );
#endif

        if( log != NULL ) {

            // Retrieve the log
            glGetProgramInfoLog( shader, length, &nchars, log );

            // Report it
            if( log[0] != '\0' ) {
                printf( "Program log:\n%s\n", log );
            }

#ifdef __cplusplus
            delete [] log;
#else
            free( log );
#endif

        } else {

            fprintf( stderr, "Program log non-empty, but allocate failed\n" );

        }
    }
}

///
// errorString( code )
//
// Returns a const char* with a text version of the supplied error code.
///
const char *errorString( ShaderError code ) {
    const char *msg;
    static char buffer[256];

    switch( code ) {

        case E_NO_ERROR:
            msg = "No error";
            break;

        case E_NO_STRING:
            msg = "No shader source string(s) provided";
            break;

        case E_NO_OBJECTS:
            msg = "Need at least two shader objects";
            break;

        case E_US_ALLOC:
            msg = "Error allocating (unknown) shader object";
            break;

        case E_US_LOAD:
            msg = "Error loading (unknown) shader code";
            break;

        case E_US_COMPILE:
            msg = "Error compiling (unknown) shader code";
            break;

        case E_VS_ALLOC:
            msg = "Error allocating vertex shader object";
            break;

        case E_VS_LOAD:
            msg = "Error loading vertex shader code";
            break;

        case E_VS_COMPILE:
            msg = "Error compiling vertex shader code";
            break;

        case E_FS_ALLOC:
            msg = "Error allocating fragment shader object";
            break;

        case E_FS_LOAD:
            msg = "Error loading fragment shader code";
            break;

        case E_FS_COMPILE:
            msg = "Error compiling fragment shader code";
            break;

        case E_PROG_ALLOC:
            msg = "Error allocating program object";
            break;

        case E_PROG_LINK:
            msg = "Error linking shader program";
            break;

        default:
            sprintf( buffer, "Unknown error code %d", code );
            msg = (const char *) buffer;

    }

    return( msg );

}

///
// shaderCreate( const GLchar **src, GLenum type, ShaderError *err )
//
// Create a GLSL shader object from a set of one or more source strings.
//
// If more than one source string is provided, care should be taken
// that only the first string contains a #version directive.
//
// @param  src  array of GLchar * containing shader source code
// @param  type type of shader object to create
// @param  err  pointer to status variable
// @return      id of the new shader object, or 0 on error, with
//              an error code in err
///
GLuint shaderCreate( const GLchar **src, GLenum type, ShaderError *err ) {
    GLuint id;
    GLint flag;
    int num;

    // Check for bad parameter
    if( src == 0 ) {
        return( 0 );
    }

    // Count the source pointers; make sure we have at least one
    num = 0;
    while( src[num] != 0 ) {
        ++num;
    }

    if( num < 1 ) {
	*err = E_NO_STRING;
        return( 0 );
    }

    // Create the shader object
    id = glCreateShader( type );

    // Verify that we were able to get an ID
    if( id == 0 ) {
	switch( type ) {
	case GL_VERTEX_SHADER:    *err = E_VS_ALLOC; break;
	case GL_FRAGMENT_SHADER:  *err = E_FS_ALLOC; break;
	default:                  *err = E_US_ALLOC; break;
	}
        return( 0 );
    }

    // Attach the source to the shaders
    glShaderSource( id, num, src, NULL );

    // Compile the source, and print any relevant message logs
    glCompileShader( id );
    glGetShaderiv( id, GL_COMPILE_STATUS, &flag );
    printShaderInfoLog( id );

    // If the creation failed, dump the shader object
    if( flag == GL_FALSE ) {
        glDeleteShader( id );
	switch( type ) {
	case GL_VERTEX_SHADER:    *err = E_VS_COMPILE; break;
	case GL_FRAGMENT_SHADER:  *err = E_FS_COMPILE; break;
	default:                  *err = E_US_COMPILE; break;
	}
        return( 0 );
    }

    return( id );

}

///
// shaderLink( GLuint ids[], size_t num, ShaderError *err )
//
// Link a collection of shaders into a shader program.
//
// @param ids   array of shader object ids
// @param num   number of elements in the array
// @param err   pointer to status variable
// @return      id of the new shader program, or 0 on error, with
//              an error code in err
///
GLuint shaderLink( GLuint ids[], size_t num, ShaderError *err ) {
    GLuint prog;
    GLint flag;

    if( num < 2 ) {
        *err = E_NO_OBJECTS;
	return( 0 );
    }

    // Create the program and attach the shaders
    prog = glCreateProgram();

    if( prog == 0 ) {
        *err = E_PROG_ALLOC;
	return( 0 );
    }

    for( int i = 0; i < num; ++i ) {
        glAttachShader( prog, ids[i] );
    }

    // Report any message log information
    printProgramInfoLog( prog );

    // Link the program, and print any message log information
    glLinkProgram( prog );
    glGetProgramiv( prog, GL_LINK_STATUS, &flag );
    printProgramInfoLog( prog );
    if( flag == GL_FALSE ) {
        *err = E_PROG_LINK;
	glDeleteProgram( prog );
        return( 0 );
    }

    return( prog );
}

///
// shaderSetupStr(vertex,fragment,err)
//
// Set up a GLSL shader program.
//
// Requires two strings containing the source code for GLSL vertex
// and fragment shaders.  Returns a program ID for the linked program.
//
// @param  vert   vertex shader program source code
// @param  frag   fragment shader program source code
// @param  err    pointer to status variable
//
// On success:
//      Returns the GLSL shader program handle, and sets the 'err'
//      parameter to E_NO_ERROR.
//
// On failure:
//      Returns 0, and assigns an error code to 'err'.
///
GLuint shaderSetupStr( const GLchar *vsrc, const GLchar *fsrc,
                       ShaderError *err ) {
    GLuint vs, fs, prog;
    const GLchar *src[2];

    // Assume that everything will work
    *err = E_NO_ERROR;

    // Create the shader objects
    src[0] = vsrc;
    src[1] = 0;

    vs = shaderCreate( src, GL_VERTEX_SHADER, err );

    if( vs == 0 ) {
	return( 0 );
    }

    src[0] = fsrc;
    fs = shaderCreate( src, GL_FRAGMENT_SHADER, err );

    if( fs == 0 ) {
	glDeleteShader( vs );
	return( 0 );
    }

    // Create the program and attach the shaders

    GLuint ids[2] = { vs, fs };
    prog = shaderLink( ids, 2, err );

    if( prog == 0 ) {
	glDeleteShader( vs );
	glDeleteShader( fs );
    }

    return( prog );
}

///
// shaderSetup(vertex,fragment,err)
//
// Set up a GLSL shader program.
//
// Requires the names of vertex and fragment program source
// files.  Returns a handle to the created GLSL program.
//
// @param  vert   vertex shader program source file
// @param  frag   fragment shader program source file
// @param  err    pointer to status variable
//
// On success:
//      Returns the GLSL shader program handle, and sets the 'err'
//      parameter to E_NO_ERROR.
//
// On failure:
//      Returns 0, and assigns an error code to 'err'.
///
GLuint shaderSetup( const char *vert, const char *frag, ShaderError *err ) {
    GLchar *vsrc = NULL, *fsrc = NULL;

    // Read in shader source
    vsrc = readTextFile( vert );
    if( vsrc == NULL ) {
        fprintf( stderr, "Error reading vertex shader file %s\n",
             vert);
        *err = E_VS_LOAD;
        return( 0 );
    }

    fsrc = readTextFile( frag );
    if( fsrc == NULL ) {
        fprintf( stderr, "Error reading fragment shader file %s\n",
             frag);
        *err = E_FS_LOAD;
#ifdef __cplusplus
        delete [] vsrc;
#else
        free( vsrc );
#endif
        return( 0 );
    }

    // Do the actual setup
    GLuint ret = shaderSetupStr( vsrc, fsrc, err );

    // Whatever happened, we're done with the source code now
#ifdef __cplusplus
    delete [] vsrc;
    delete [] fsrc;
#else
    free( vsrc );
    free( fsrc );
#endif

    return( ret );
}
