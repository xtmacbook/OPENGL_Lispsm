#ifndef ShaderH
#define ShaderH

#include <string>
#include <GL/glHeader.h>

inline const std::string getGLSLErrorString(const unsigned);

class Shader {
public:
    Shader();
    ~Shader();
    int load(const std::string& filename);   //!< read file, if result is 0 everything is ok. -1: File not found, -2: Empty File, -3: no memory
    void loadFromMemory(const char* program); //!< load program from char array, make sure program is 0 terminated! 
	bool isCompiled() { return is_compiled; }
    bool compile();
    const std::string getCompilerLog();
	GLhandleARB getHandle() { return ProgramObject; }
	std::string VertexShaderCode;
protected:
    GLhandleARB         ProgramObject;         //!< Program Object
    GLubyte*            ShaderSource;          //!< ASCII Source-Code
    GLcharARB*          compiler_log;
    bool                is_compiled;            //!< true if compiled
    bool                _memalloc;               //!< true if shader allocated memory
};

class VertexShader : public Shader {
public:
	VertexShader();
	~VertexShader(); 
};


class FragmentShader : public Shader {
public:
	FragmentShader();
	~FragmentShader(); 
};

#endif