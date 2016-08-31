#ifndef ShaderManagerH
#define ShaderManagerH

#include <vector>
#include "Shader.h"
//-----------------------------------------------------------------------------

class ShaderObject
{
public:
    ShaderObject();            // Standard Constructor
    ~ShaderObject();           // Destructor

    void addShader(Shader* ShaderProgram); //!< add a Vertex or Fragment Program
    
    bool link();            //!< Link all Shaders
    const char* const getLinkerLog(void);   //!< get Linker messages

    void begin();	//!< use Shader. OpenGL calls will go through shader.
    void end();		//!< Stop using this shader. OpenGL calls will go through regular pipeline.
    
    // Send Variables to Program
 
    bool sendUniform1f(const char* varname, const GLfloat& v0); //!< send float to program
    bool sendUniform2f(const char* varname, const GLfloat& v0, const GLfloat& v1); //!< send vec2 to program
    bool sendUniform3f(const char* varname, const GLfloat& v0, const GLfloat& v1, const GLfloat& v2); //!< send vec3 to program
    bool sendUniform4f(const char* varname, const GLfloat& v0, const GLfloat& v1, const GLfloat& v2, const GLfloat& v3); //!< send vec4 to program

    bool sendUniform1i(const char* varname, const GLint v0);
    bool sendUniform2i(const char* varname, const GLint v0, const GLint v1);
    bool sendUniform3i(const char* varname, const GLint v0, const GLint v1, const GLint v2);
    bool sendUniform4i(const char* varname, const GLint v0, const GLint v1, const GLint v2, const GLint v3);

    bool sendUniform1fv(const char* varname, const GLsizei count, const GLfloat *value);
    bool sendUniform2fv(const char* varname, const GLsizei count, const GLfloat *value);
    bool sendUniform3fv(const char* varname, const GLsizei count, const GLfloat *value);
    bool sendUniform4fv(const char* varname, const GLsizei count, const GLfloat *value);

    bool sendUniform1iv(const char* varname, const GLsizei count, const GLint *value);
    bool sendUniform2iv(const char* varname, const GLsizei count, const GLint *value);
    bool sendUniform3iv(const char* varname, const GLsizei count, const GLint *value);
    bool sendUniform4iv(const char* varname, const GLsizei count, const GLint *value);

    bool sendUniformMatrix2fv(const char* varname, const GLsizei count, const GLboolean transpose, const GLfloat *value);
    bool sendUniformMatrix3fv(const char* varname, const GLsizei count, const GLboolean transpose, const GLfloat *value);
    bool sendUniformMatrix4fv(const char* varname, const GLsizei count, const GLboolean transpose, const GLfloat *value);


    // Receive Uniform variables:
    void GetUniformfv(const char* name, GLfloat* values);
    void GetUniformiv(const char* name, GLint* values); 

    void manageMemory(){_mM = true;}
	inline bool isActive() const { return active; }
       
private:
    GLint GetUniLoc(const GLcharARB *name);      // get location of a variable
    
    GLhandleARB         hndShader;            // Shader Object
    

    GLcharARB*          linker_log;
    bool                is_linked;
    std::vector<Shader*> ShaderList;     // List of all Shader Programs

    bool                _mM;
	bool				active;
};

//-----------------------------------------------------------------------------
// To simplify the process loading/compiling/linking shaders I created this
// high level interface to setup a vertex/fragment shader.

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

	ShaderObject* loadfromFile(const std::string& vertexFile, const std::string& fragmentFile);    // load vertex/fragment shader from file
    ShaderObject* loadfromMemory(const char* vertexMem, const char* fragmentMem);
    
    bool free(ShaderObject* o);

	ShaderObject* getShaderObject(const unsigned id) const;

private:
    std::vector<ShaderObject*>  _shaderObjectList;      
};


#endif // A_GLSL_H