/*
 * CcGPUCtrl - ConsCAD utility library (Reduced, Limited functions Library)
 *
 * Copyright (c) 2019 Pinakin Zaveri <Pinakin.Zaveri@rediff.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "stdafx.h"
#include "pch.h"
// #include "Angel.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "CcGPUCtrlIntrFc.h"

// namespace Angel {

	/*
// Create a NULL-terminated string by reading the provided file
static char*
readShaderSource(const char* shaderFile)
{
    FILE* fp = fopen(shaderFile, "r");

    if ( fp == NULL ) { return NULL; }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, 1, size, fp);

    buf[size] = '\0';
    fclose(fp);

    return buf;
}
*/

// Create a NULL-terminated string by reading the provided file
char* CcGPUCtrlIntrFc::readShaderSource(const char* shaderFileP)
{
	char* buf;

	// 1. retrieve the vertex/fragment source code from filePath
	std::string shaderCodeL;
	std::ifstream strmShaderFileL;
	
	// ensure ifstream objects can throw exceptions:
	strmShaderFileL.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		strmShaderFileL.open(shaderFileP);
	
		std::stringstream strStreamShaderL;

		// read file's buffer contents into streams
		strStreamShaderL << strmShaderFileL.rdbuf();
		
		// close file handlers
		strmShaderFileL.close();
		
		// convert stream into string
		shaderCodeL = strStreamShaderL.str();
		
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* shaderCodeStrL = shaderCodeL.c_str();

	buf = strdup(shaderCodeStrL);
	
	return buf;
}


// Create a GLSL programL object from vertex and fragment shader files
unsigned int  CcGPUCtrlIntrFc::InitShaderFile(const char* vShaderFile, const char* fShaderFile)
{
    struct Shader {
	const char*  filename;
	GLenum       type;
	GLchar*      source;
    }  shaders[2] = {
	{ vShaderFile, GL_VERTEX_SHADER, NULL },
	{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
    };

	GLuint shaderAry[2];	// 20210111 as we have two shaders now GL_VERTEX_SHADER & GL_FRAGMENT_SHADER

    GLuint programL = glCreateProgram();
    
    for ( int i = 0; i < 2; ++i ) {
	Shader& s = shaders[i];
	s.source = readShaderSource( s.filename );
	if ( shaders[i].source == NULL ) {
	    std::cerr << "Failed to read " << s.filename << std::endl;
	    exit( EXIT_FAILURE );
	}

	GLuint shader = glCreateShader( s.type );
	glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );
	glCompileShader( shader );

	shaderAry[i] = shader;	// 20210111 , used later for delete

	GLint  compiled;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled ) {
	    std::cerr << s.filename << " failed to compile:" << std::endl;
	    GLint  logSize;
	    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
	    char* logMsg = new char[logSize];
	    glGetShaderInfoLog( shader, logSize, NULL, logMsg );
	    std::cerr << logMsg << std::endl;
	    delete [] logMsg;

	    exit( EXIT_FAILURE );
	}

	delete [] s.source;

	glAttachShader( programL, shader );
    }

    /* link  and error check */
    glLinkProgram(programL);

    GLint  linked;
    glGetProgramiv( programL, GL_LINK_STATUS, &linked );
    if ( !linked ) {
	std::cerr << "Shader programL failed to link" << std::endl;
	GLint  logSize;
	glGetProgramiv( programL, GL_INFO_LOG_LENGTH, &logSize);
	char* logMsg = new char[logSize];
	glGetProgramInfoLog( programL, logSize, NULL, logMsg );
	std::cerr << logMsg << std::endl;
	delete [] logMsg;

	exit( EXIT_FAILURE );
    }

  
	// 20210111 Delete Shaders as no longer needed (Shaders Source and object files are now availabe 
	// as linked code in program.)

	glDetachShader(programL, shaderAry[0]);
	glDeleteShader(shaderAry[0]);

	glDetachShader(programL, shaderAry[1]);
	glDeleteShader(shaderAry[1]);
	
    return programL;
}



static const char* vs_Source_InL =
{
	"#version 150\n"
	"\n"
	"in  vec4 vPosition; \n"
	"in  vec4 vNormal; \n"
	"uniform mat4 p; \n"
	"uniform mat4 mv; \n"
	"uniform vec4 lightPosition; \n"
	"uniform vec3 uColor; \n"
	"out vec4 color; \n"
	"float shininess; \n"
	"vec4 ambientProduct; \n"
	"vec4 diffuseProduct; \n"
	"vec4 specularProduct; \n"
	"vec4 mvPosition; \n"
	"void main()\n"
	"{\n"
		"shininess = 5.0; \n"
		"ambientProduct = vec4(0.2 * uColor, 1); \n"
		"diffuseProduct = vec4(0.8 * uColor,1); \n"
		"specularProduct = vec4(0.3); \n"
		"mvPosition = mv * vPosition; \n"
		"gl_Position = p * mvPosition; \n"
		"vec3 N = normalize((mv * vNormal).xyz); \n"
		"vec3 L = normalize(lightPosition.xyz - mvPosition.xyz); \n"
		"if (lightPosition.w == 0.0) L = normalize(lightPosition.xyz); \n"
		"vec3 E = -normalize(mvPosition.xyz); \n"
		"vec3 H = normalize(L + E); \n"
		"float Kd = max(dot(L,N), 0.0); \n"
		"float Ks = pow(max(dot(N,H), 0.0), shininess); \n"
		"color = Kd * diffuseProduct + Ks * specularProduct + ambientProduct; \n"
	"}\n"

};


static const char* fs_Source_InL =
{
	"#version 150\n"
	"\n"
	"in  vec4 color;\n"
	"out vec4 fColor;\n"
	"void main()\n"
	"{\n"
		"fColor = color; \n"
	"}\n"
};


// Create a GLSL programL object from vertex and fragment shader files
unsigned int CcGPUCtrlIntrFc::InitShaderIn()
{
	struct Shader {
		const char* filename;
		GLenum       type;
		GLchar* source;
	}  shaders[2] = {
	{ "V-SHD", GL_VERTEX_SHADER, NULL },
	{ "F_SHD", GL_FRAGMENT_SHADER, NULL }
	};

	GLuint programL = glCreateProgram();

	for (int i = 0; i < 2; ++i) {
		Shader& s = shaders[i];
		
		// s.source = readShaderSource(s.filename);
		if (i == 0)
		{ 
			s.source = strdup(vs_Source_InL);
		}else
		{
			s.source = strdup(fs_Source_InL);
		}



		if (shaders[i].source == NULL) {
			std::cerr << "Failed to read " << s.filename << std::endl;
			exit(EXIT_FAILURE);
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar * *)& s.source, NULL);
		glCompileShader(shader);

		GLint  compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			std::cerr << s.filename << " failed to compile:" << std::endl;
			GLint  logSize;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			std::cerr << logMsg << std::endl;
			delete[] logMsg;

			exit(EXIT_FAILURE);
		}

		delete[] s.source;

		glAttachShader(programL, shader);
	}

	/* link  and error check */
	glLinkProgram(programL);

	GLint  linked;
	glGetProgramiv(programL, GL_LINK_STATUS, &linked);
	if (!linked) {
		std::cerr << "Shader programL failed to link" << std::endl;
		GLint  logSize;
		glGetProgramiv(programL, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(programL, logSize, NULL, logMsg);
		std::cerr << logMsg << std::endl;
		delete[] logMsg;

		exit(EXIT_FAILURE);
	}

	/* use programL object */
// 	glUseProgram(programL);

	return programL;
}

// }  // Close namespace Angel block


