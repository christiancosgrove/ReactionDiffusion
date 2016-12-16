#pragma once
#include <map>
#include <GL/glew.h>
#include <SFML\OpenGL.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <glm\glm.hpp>

class GLProgram
{
public:
	GLuint fragmentShader, vertexShader, programID;

	GLProgram(const std::string& fragName, const std::string& vertName);
	//This function reads from a local file and compiles it into an OpenGL shader
	static GLuint CompileShader(const std::string& shaderName, GLenum type);
	inline void Use();
	inline void SetMatrix4(const std::string& name, const GLfloat* value);
	inline void SetMatrix4(const std::string& name, const GLdouble* value);
	inline void SetFloat(const std::string& name, GLfloat value);
	inline void SetVector3(const std::string& name, const GLfloat* value);
	inline void SetVector2(const std::string& name, glm::vec2& value);
	inline void SetBool(const std::string& name, bool value);

	//    inline void SetVector3(const std::string& name, const glm::vec3& value);
	//    inline void SetVector3(const std::string& name, const GLdouble* value);
	//    inline void SetVector3(const std::string& name, const glm::dvec3& value);
	//    inline void SetVector2(const std::string& name, const glm::ivec2& value);
	//    inline void SetVector2(const std::string& name, const glm::vec2& value);
	inline void SetTexture(const std::string& name, GLuint texture); //for sampler2D object
};

void GLProgram::Use()
{
	glUseProgram(programID);
}
void GLProgram::SetMatrix4(const std::string& name, const GLfloat *value)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
}
void GLProgram::SetMatrix4(const std::string& name, const GLdouble *value)
{
	glUniformMatrix4dv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
}
void GLProgram::SetFloat(const std::string& name, GLfloat value)
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}
void GLProgram::SetVector3(const std::string& name, const GLfloat *value)
{
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, value);
}
void GLProgram::SetVector2(const std::string& name, glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}
inline void GLProgram::SetBool(const std::string & name, bool value)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}
//void GLProgram::SetVector3(const std::string& name, const glm::vec3 &value)
//{
//    SetVector3(name, glm::value_ptr(value));
//}
//void GLProgram::SetVector3(const std::string& name, const GLdouble* value)
//{
//    glUniform3dv(glGetUniformLocation(programID, name.c_str()), 1, value);
//}
//void GLProgram::SetVector3(const std::string& name, const glm::dvec3& value)
//{
//    SetVector3(name, glm::value_ptr(value));
//}
//void GLProgram::SetVector2(const std::string &name, const glm::ivec2 &value)
//{
//    glUniform2iv(glGetUniformLocation(programID, name.c_str()),1,  glm::value_ptr(value));
//}
//void GLProgram::SetVector2(const std::string &name, const glm::vec2 &value)
//{
//    glUniform2fv(glGetUniformLocation(programID, name.c_str()),1,  glm::value_ptr(value));
//}
void GLProgram::SetTexture(const std::string &name, GLuint texture)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), texture);
}


//this class further abstracts the GLProgram class, allowing for additional OpenGL configuration
class GLManager
{
public:
	std::vector<GLProgram> Programs;
	GLManager(const std::string& fragName, const std::string& vertName);
	int AddProgram(const std::string fragmentShaderName, const std::string& vertexShaderName);
	void AddUniformBuffer(const std::string& name, std::size_t size, std::initializer_list<GLuint> programs);
	void UpdateBuffer(const std::string& name, const void* value, std::size_t size);
private:
	std::unordered_map<std::string, GLuint> uniformBuffers;
	//some OpenGL configuration
	void initGL();
};