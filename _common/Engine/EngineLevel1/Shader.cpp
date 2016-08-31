#include <vector>
#include <iostream>
#include <fstream>
#include "Shader.h"

inline const std::string getGLSLErrorString(const unsigned i) {
	const char* const aGLSLErrorString[] = {
        "(e0000) GLSL not enabled",
        "(e0001) not a valid program object",
        "(e0002) not a valid object",
        "(e0003) out of memory",
        "(e0004) unknown compiler error"};
	return aGLSLErrorString[i];
}

Shader::Shader(): compiler_log(0), is_compiled(false),
	ProgramObject(0), ShaderSource(0), _memalloc(false) {
}

Shader::~Shader() {
   if (compiler_log != 0) 
	   free(compiler_log);

   if (ShaderSource != 0) {
        if (_memalloc)
            delete[] ShaderSource;  // free ASCII Source
   }
   
   if (is_compiled) { 
        glDeleteObjectARB(ProgramObject);
        CHECK_GL_ERROR();
   }
}

unsigned long getFileSize(std::ifstream& file) {
    using namespace std;
	if(!file.good()) return 0;
    
    file.tellg();
    file.seekg(0,ios::end);
    unsigned long len = file.tellg();
    file.seekg(ios::beg);
    
    return len;
}

int Shader::load(const std::string& filename) {
	
	std::ifstream VertexShaderStream(filename, std::ios::in);

	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();

	}
    
    
      
	return 0;
}

void Shader::loadFromMemory(const char* program) {
    if (ShaderSource!=0)    // there is already a source loaded, free it!
    {
        if (_memalloc)
        delete[] ShaderSource;
    }
   _memalloc = false;
   ShaderSource = (GLubyte*) program;
      
}

const std::string Shader::getCompilerLog(void) {    
	int blen = 0;	
	int slen = 0;	


	if (ProgramObject == 0) 
		return getGLSLErrorString(1);

	glGetObjectParameterivARB(ProgramObject, GL_OBJECT_INFO_LOG_LENGTH_ARB , &blen);
	CHECK_GL_ERROR();

	if (blen > 1) {
		if (compiler_log!=0) {   
			free(compiler_log);
			compiler_log =0;
		}
		if ((compiler_log = (GLcharARB*)malloc(blen)) == NULL) {
			printf("ERROR: Could not allocate compiler_log buffer\n");
			return getGLSLErrorString(3);
		}

		glGetInfoLogARB(ProgramObject, blen, &slen, compiler_log);
		CHECK_GL_ERROR();
	}
	if (compiler_log != 0)
		return (char*) compiler_log;    

	return getGLSLErrorString(4);
}

bool Shader::compile(void)
{

	is_compiled = false;

	int compiled = 0;
 
 

	GLint	length = VertexShaderCode.length();
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(ProgramObject, 1, &VertexSourcePointer  , NULL);
	glCompileShader(ProgramObject);

	 glGetShaderiv(ProgramObject, GL_COMPILE_STATUS, &compiled);

	if (compiled) 
		is_compiled = true;
	 
	return is_compiled;
}

VertexShader::VertexShader() {
	ProgramObject = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	CHECK_GL_ERROR();
}

VertexShader::~VertexShader() {
}

FragmentShader::FragmentShader() {
	ProgramObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB); 
	CHECK_GL_ERROR();
}

FragmentShader::~FragmentShader() {
}

