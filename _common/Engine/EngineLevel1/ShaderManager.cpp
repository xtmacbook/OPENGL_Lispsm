#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include "ShaderManager.h"

ShaderObject::ShaderObject(): active(false) {
	using namespace std;
	hndShader = 0;
	linker_log = 0;
	_mM = false;

	if (!GLEW_ARB_shader_objects)
		cerr << "**warning** GL_ARB_shader_objects not defined!!\n"; 
	if (!GLEW_ARB_vertex_shader)
		cerr << "**warning** GL_ARB_vertex_shader not defined!!\n";
	if (!GLEW_ARB_fragment_shader)
		cerr << "**warning** GL_ARB_fragment_shader not defined!!\n";

	if (!GLEW_ARB_shading_language_100)
		cerr << "**warning** GL_ARB_shading_language_100 not defined!!\n";
	    
	hndShader = glCreateProgramObjectARB();
	is_linked = false;       
}

//----------------------------------------------------------------------------- 

ShaderObject::~ShaderObject()
{
    if (linker_log!=0) 
		free(linker_log);
    for (unsigned int i=0;i<ShaderList.size();i++)
    {
		glDetachObjectARB(hndShader, ShaderList[i]->getHandle());
        CHECK_GL_ERROR(); // if you get an error here, you deleted the Program object first and then
                        // the hndShader! Always delete ShaderObjects last!
        if (_mM) delete ShaderList[i]; 
    }                      

    glDeleteObjectARB(hndShader);
    CHECK_GL_ERROR();

}

void ShaderObject::addShader(Shader* ShaderProgram) {
	using namespace std;
	if(0 == ShaderProgram) 
		return;

	if(!ShaderProgram->isCompiled()) {
		cerr << "**warning** please compile program before adding object! trying to compile now...\n";
		if(!ShaderProgram->compile()) {
			cerr << "...compile ERROR!\n";
			return;
		} 
		else {   
			cerr << "...ok!\n";
		}
	}
	ShaderList.push_back(ShaderProgram); 
}


bool ShaderObject::link() {
	using namespace std;
	unsigned int i;
	if(is_linked) {
		// already linked, detach everything first
		cerr << "warning object is already linked, trying to link again" << endl;
		for(i = 0; i < ShaderList.size(); i++) {
			glDetachObjectARB(hndShader, ShaderList[i]->getHandle());
			CHECK_GL_ERROR();
		}
	}

	for(i = 0; i < ShaderList.size(); i++) {
		glAttachObjectARB(hndShader, ShaderList[i]->getHandle());
		CHECK_GL_ERROR();
		//cerr << "attaching ProgramObj [" << i << "] @ 0x" << hex << ShaderList[i]->ProgramObject << " in ShaderObj @ 0x"  << hndShader << endl;
	}

	int linked;
	glLinkProgramARB(hndShader);
	CHECK_GL_ERROR();
	glGetObjectParameterivARB(hndShader, GL_OBJECT_LINK_STATUS_ARB, &linked);
	CHECK_GL_ERROR();

	if(linked) {
		is_linked = true;
		return true;
	}
	else {
		cerr << "**linker error**\n";
		return false;
	}
}

//----------------------------------------------------------------------------- 
// Compiler Log: Ausgabe der Compiler Meldungen in String
const char* const ShaderObject::getLinkerLog(void) {    
	int blen = 0;	
	int slen = 0;	

	if(0 == hndShader) 
		return getGLSLErrorString(2).c_str();

	glGetObjectParameterivARB(hndShader, GL_OBJECT_INFO_LOG_LENGTH_ARB , &blen);
	CHECK_GL_ERROR();

	if (blen > 1) {
		if (linker_log!=0) {   
			free(linker_log);
			linker_log =0;
		}
		if ((linker_log = (GLcharARB*)malloc(blen)) == NULL) {
			std::cerr << "ERROR: Could not allocate compiler_log buffer\n";
			return getGLSLErrorString(3).c_str();
		}
		glGetInfoLogARB(hndShader, blen, &slen, linker_log);
		CHECK_GL_ERROR();
	}
	if (linker_log!=0)
		return (char*) linker_log;    

	return getGLSLErrorString(4).c_str();
}

void ShaderObject::begin(void) {
	if (hndShader == 0) 
		return;
    if (is_linked){
        glUseProgramObjectARB(hndShader);
        CHECK_GL_ERROR();
		active = true;
    }
}

//----------------------------------------------------------------------------- 

void ShaderObject::end(void) {
    glUseProgramObjectARB(0);
    CHECK_GL_ERROR();
	active = false;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform1f(const char* varname, const GLfloat& v0) {
    GLint loc = GetUniLoc(varname);
    if (loc==-1) 
		return false;  // can't find variable
    glUniform1fARB(loc, v0);
    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform2f(const char* varname, const GLfloat& v0, const GLfloat& v1) {
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    glUniform2fARB(loc, v0, v1);
    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform3f(const char* varname, const GLfloat& v0, const GLfloat& v1, const GLfloat& v2) {
	GLint loc = GetUniLoc(varname);
	if (loc==-1) return false;  // can't find variable

	glUniform3fARB(loc, v0, v1, v2);

	return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform4f(const char* varname, const GLfloat& v0, const GLfloat& v1, const GLfloat& v2, const GLfloat& v3)
{
    
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform4fARB(loc, v0, v1, v2, v3);

    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform1i(const char* varname, const GLint v0) { 
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform1iARB(loc, v0);
    
    return true;
}
bool ShaderObject::sendUniform2i(const char* varname, const GLint v0, const GLint v1) {
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform2iARB(loc, v0, v1);


    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform3i(const char* varname, const GLint v0, const GLint v1, const GLint v2)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform3iARB(loc, v0, v1, v2);

    return true;
}
bool ShaderObject::sendUniform4i(const char* varname, const GLint v0, const GLint v1, const GLint v2, const GLint v3)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform4iARB(loc, v0, v1, v2, v3);

    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform1fv(const char* varname, const GLsizei count, const GLfloat *value)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform1fvARB(loc, count, value);

    return true;
}
bool ShaderObject::sendUniform2fv(const char* varname, const GLsizei count, const GLfloat *value)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform2fvARB(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform3fv(const char* varname, const GLsizei count, const GLfloat *value)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform3fvARB(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform4fv(const char* varname, const GLsizei count, const GLfloat *value)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform4fvARB(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform1iv(const char* varname, const GLsizei count, const GLint *value)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform1ivARB(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform2iv(const char* varname, const GLsizei count, const GLint *value)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform2ivARB(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform3iv(const char* varname, const GLsizei count, const GLint *value)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform3ivARB(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniform4iv(const char* varname, const GLsizei count, const GLint *value)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniform4ivARB(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniformMatrix2fv(const char* varname, const GLsizei count, const GLboolean transpose, const GLfloat *value)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniformMatrix2fvARB(loc, count, transpose, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniformMatrix3fv(const char* varname, const GLsizei count, const GLboolean transpose, const GLfloat *value)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniformMatrix3fvARB(loc, count, transpose, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool ShaderObject::sendUniformMatrix4fv(const char* varname, const GLsizei count, const GLboolean transpose, const GLfloat *value)
{
    GLint loc = GetUniLoc(varname);
    if (loc==-1) return false;  // can't find variable
    
    glUniformMatrix4fvARB(loc, count, transpose, value);

    return true;
}

//----------------------------------------------------------------------------- 

GLint ShaderObject::GetUniLoc(const GLcharARB *name)
{
	GLint loc;

	loc = glGetUniformLocationARB(hndShader, name);
	if (loc == -1) {
		std::cerr << "Error: can't find uniform variable \"" << name << "\"\n";
	}
    CHECK_GL_ERROR();
	return loc;
}

//----------------------------------------------------------------------------- 

void ShaderObject::GetUniformfv(const char* name, GLfloat* values)
{
    GLint loc;

	loc = glGetUniformLocationARB(hndShader, name);
	if (loc == -1) 
	{
        std::cout << "Error: can't find uniform variable \"" << name << "\"\n";
	}
	glGetUniformfvARB(hndShader, loc, values);
	
}

//----------------------------------------------------------------------------- 

void ShaderObject::GetUniformiv(const char* name, GLint* values)
{
    GLint loc;

	loc = glGetUniformLocationARB(hndShader, name);
	if (loc == -1) 
	{
		std::cout << "Error: can't find uniform variable \"" << name << "\"\n";
	}
	
	glGetUniformivARB(hndShader, loc, values);

}

ShaderManager::ShaderManager() {
}

ShaderManager::~ShaderManager() {
	using namespace std;

   // free objects
   vector<ShaderObject*>::iterator  i = _shaderObjectList.begin();
   while (i!=_shaderObjectList.end()) 
   {
        ShaderObject* o = *i;
        i=_shaderObjectList.erase(i);
        delete o;
   }
}

// ----------------------------------------------------------------------------

ShaderObject* ShaderManager::loadfromFile(const std::string& vertexFile, const std::string& fragmentFile) {
	using namespace std;
	ShaderObject* o = new ShaderObject();
  
	VertexShader* tVertexShader = 0;
	FragmentShader* tFragmentShader = 0;

	if(!fragmentFile.empty()) {
		tFragmentShader = new FragmentShader;
		// Load fragment program
		if(0 != tFragmentShader->load(fragmentFile) ) {
			cerr << "error: can't load fragment shader:" << fragmentFile << endl;
			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			return 0;
		}

		// Compile fragment program 
		if( !tFragmentShader->compile() ) {
			cerr << "error: can't compile Fragment Shader:" << fragmentFile << endl <<
				tFragmentShader->getCompilerLog() << endl << endl;
			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			return 0;
		}
		// Add to ShaderObject
		o->addShader(tFragmentShader); 
	}

	if(!vertexFile.empty()) {
		tVertexShader = new VertexShader;
	    // load vertex program
		if(0 != tVertexShader->load(vertexFile) ) { 
			cerr << "error: can't load vertex shader:" << vertexFile << endl; 
			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			return 0;
		}
		// Compile vertex program
		if(!tVertexShader->compile() ) {
			cerr << "error: can't compile Vertex Shader:" << vertexFile << endl << 
				tVertexShader->getCompilerLog() << endl << endl;
			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			return 0;
		}
		// Add to ShaderObject
		o->addShader(tVertexShader);
	}
  
	// link 
	if( !o->link() ) {
		cerr << "error: can't link shaders:" << vertexFile << "(v), " << 
			fragmentFile << "(f)" << endl << 
			o->getLinkerLog() << endl << endl;
		delete o;
		delete tVertexShader;
		delete tFragmentShader;
		return 0;
	}
	else {
		cout << "successful linked shaders " << vertexFile << "(v), " << 
			fragmentFile << "(f)" << endl;
	}

	_shaderObjectList.push_back(o);
	o->manageMemory();
	return o;
}


// ----------------------------------------------------------------------------

ShaderObject* ShaderManager::loadfromMemory(const char* vertexMem, const char* fragmentMem) {
	using namespace std;
  ShaderObject* o = new ShaderObject();
  
  VertexShader* tVertexShader = new VertexShader;
  FragmentShader* tFragmentShader = new FragmentShader;

  // get vertex program
  if (vertexMem!=0)
     tVertexShader->loadFromMemory(vertexMem);
  
  // get fragment program
  if (fragmentMem!=0)
     tFragmentShader->loadFromMemory(fragmentMem);
 
  // Compile vertex program
  if (vertexMem!=0)
  if (!tVertexShader->compile())
  {
      cout << "***COMPILER ERROR (Vertex Shader):\n";
      cout << tVertexShader->getCompilerLog() << endl;
      delete o;
      delete tVertexShader;
      delete tFragmentShader;
      return 0;
  }
   
  // Compile fragment program 
  if (fragmentMem!=0)
  if (!tFragmentShader->compile())
  {
     cout << "***COMPILER ERROR (Fragment Shader):\n";
     cout << tFragmentShader->getCompilerLog() << endl;
     
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
     
  }
   
  // Add to object    
  if (vertexMem!=0) o->addShader(tVertexShader);
  if (fragmentMem!=0) o->addShader(tFragmentShader); 
  
  // link 
  if (!o->link())
  {
     cout << "**LINKER ERROR\n";
     cout << o->getLinkerLog() << endl;
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
  }

	_shaderObjectList.push_back(o);
	o->manageMemory();
	return o;
}

bool ShaderManager::free(ShaderObject* o) {
	std::vector<ShaderObject*>::iterator i = _shaderObjectList.begin();
	while( i != _shaderObjectList.end()) {
		if( (*i) == o) {
			_shaderObjectList.erase(i);
			delete o;
			return true;
		}
		i++;
	}   
	return false;
 }

ShaderObject* ShaderManager::getShaderObject(const unsigned id) const {
	if(id < _shaderObjectList.size()) {
		return _shaderObjectList[id];
	}
	else {
		return 0;
	}
}

