#include "shaders.h"
#include <exception> // TODO remove
#include <iostream>

using std::string;

string SHADERS_FOLDER_PATH = PROJECT_SOURCE_DIR "/resources/shaders/";

// ------------------------------------------
// --- Shader Class ---

Shader::Shader(string filename) : m_filename(filename)
{
	loadAndCompile();
}


// Load the shader source code from the file, set the shader type and compile the shader
void Shader::loadAndCompile()
{
	// Determine shader type based on file extension
	string extension = (m_filename).substr((m_filename).size() - 4);

	if (extension == "vert") m_type = GL_VERTEX_SHADER;
	else if (extension == "frag") m_type = GL_FRAGMENT_SHADER;
	else if (extension == "geom") m_type = GL_GEOMETRY_SHADER;
	else if (extension == "comp") m_type = GL_COMPUTE_SHADER;
	else
	{
		// TODO raise exception
	}

	try {
		// Attempt to read shader file
		std::ifstream in_file_stream(SHADERS_FOLDER_PATH + m_filename);
		std::stringstream in_string_stream;
		in_string_stream << in_file_stream.rdbuf();
		in_file_stream.close();
		string shader_source_str = in_string_stream.str();
		const char *shader_source_c_str = shader_source_str.c_str();

		// Create OpenGL shader object
		m_id = glCreateShader(m_type);

		// Specify shader code
		glShaderSource(m_id, 1, &shader_source_c_str, nullptr);

		// Compile shader
		glCompileShader(m_id);

		// TODO check for compilat errors
		int success;
		char infoLog[512];
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(m_id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

	} // TODO catch exception if can't open file
	catch (std::runtime_error e) // TODO appropriate except
	{

	}
}

Shader::~Shader()
{
	//glDeleteShader(m_id); -- REMOVED s.t. can use vector<Shader> in ShaderProgram class
}

GLuint Shader::getHandle() const
{
	return m_id;
}


// ------------------------------------------
// --- ShaderProgram Class ---

ShaderProgram::ShaderProgram(const std::vector<Shader> &shaders) : m_shaders(shaders)
{
	ShaderProgram::createProgram();
}

// Create a new shader program object and link the shaders together
void ShaderProgram::createProgram()
{
	// Create shader program OpenGL object
	m_id = glCreateProgram();

	// Attach shaders
	for (const Shader &shader : m_shaders)
	{
		glAttachShader(m_id, shader.getHandle());
	}

	// Link program
	glLinkProgram(m_id);

	// TODO check for link errors
	int success;
	char infoLog[512];
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

ShaderProgram::~ShaderProgram()
{
	for (const Shader &shader : m_shaders)
	{
		glDeleteShader(shader.getHandle());
	}

	glDeleteProgram(m_id);
}

// Use this shader program in the OpenGL rendering pipeline
void ShaderProgram::use()
{
	glUseProgram(m_id);
}

// Return the GLuint program handle/id
GLuint ShaderProgram::getHandle() const
{
	return m_id;
}

// Tell shader it can find the data for the vertex attribute (with dimensionality 'attribute_size') 
// at 'location' by using the values in 'buffer'. 
void ShaderProgram::bindData(GLuint location, const Buffer &buffer, GLuint attribute_size) const
{
	// bind the given buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer.getHandle());
	// specify how openGL should interpret the data
	glVertexAttribPointer(location, attribute_size, GL_FLOAT, GL_FALSE, 0, 0);
	// enable attrib
	glEnableVertexAttribArray(location);
}


// --- methods for setting uniforms in shader program
// set uniform named 'target' to the given value 'v'

void ShaderProgram::setInt(const string &target, int v) const
{
	glUniform1i(glGetUniformLocation(m_id, target.c_str()), v);
}

void ShaderProgram::setFloat(const string &target, float v) const
{
	glUniform1f(glGetUniformLocation(m_id, target.c_str()), v);
}

void ShaderProgram::setVec2(const string &target, const glm::vec2 &v) const
{
	glUniform2fv(glGetUniformLocation(m_id, target.c_str()), 1, &v[0]);
}

void ShaderProgram::setVec3(const string &target, const glm::vec3 &v) const
{
	glUniform3fv(glGetUniformLocation(m_id, target.c_str()), 1, &v[0]);
}

void ShaderProgram::setVec4(const string &target, const glm::vec4 &v) const
{
	glUniform4fv(glGetUniformLocation(m_id, target.c_str()), 1, &v[0]);
}

void ShaderProgram::setMat3(const string &target, const glm::mat3 &v) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_id, target.c_str()), 1, GL_FALSE, &v[0][0]);
}

void ShaderProgram::setMat4(const string &target, const glm::mat4 &v) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, target.c_str()), 1, GL_FALSE, &v[0][0]);
}
