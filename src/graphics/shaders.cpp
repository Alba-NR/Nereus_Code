#include "shaders.h"
#include <exception> // TODO remove

using std::string;

string SHADERS_FOLDER_PATH = "../../resources/shaders";

// ------------------------------------------
// --- Shader Class ---

Shader::Shader(string filename) : filename(filename)
{
	loadAndCompile();
}


// Load the shader source code from the file, set the shader type and compile the shader
void Shader::loadAndCompile()
{
	// Determine shader type based on file extension
	string extension = (this->filename).substr((this->filename).size() - 4);

	if (extension == "vert") this->type = GL_VERTEX_SHADER;
	else if (extension == "frag") this->type = GL_FRAGMENT_SHADER;
	else if (extension == "geom") this->type = GL_GEOMETRY_SHADER;
	else if (extension == "comp") this->type = GL_COMPUTE_SHADER;
	else
	{
		// TODO raise exception
	}

	try {
		// Attempt to read shader file
		string filepath = SHADERS_FOLDER_PATH + filename;
		std::ifstream in_file_stream(filepath);
		std::stringstream in_string_stream;
		in_string_stream << in_file_stream.rdbuf();
		string shader_source_str = in_string_stream.str();
		const char *shader_source_c_str = shader_source_str.c_str();;

		// Create OpenGL shader object
		this->id = glCreateShader(this->type);

		// Specify shader code
		glShaderSource(this->id, 1, &shader_source_c_str, nullptr);

		// Compile shader
		glCompileShader(this->id);

		// TODO check for compilat errors

	} // TODO catch exception if can't open file
	catch (std::runtime_error e) // TODO appropriate except
	{

	}
}

Shader::~Shader()
{
	glDeleteShader(this->id);
}

GLuint Shader::getHandle()
{
	return this->id;
}


// ------------------------------------------
// --- ShaderProgram Class ---

ShaderProgram::ShaderProgram(std::vector<Shader> shaders) : shaders(shaders)
{
	createProgram();
}

// Create a new shader program object and link the shaders together
void ShaderProgram::createProgram()
{
	// Create shader program OpenGL object
	this->id = glCreateProgram();

	// Attach shaders
	for (Shader shader : this->shaders)
	{
		glAttachShader(this->id, shader.getHandle());
	}

	// Link program
	glLinkProgram(this->id);

	// TODO check for link errors
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->id);
}

// Use this shader program in the OpenGL rendering pipeline
void ShaderProgram::use()
{
	glUseProgram(this->id);
}

// Return the GLuint program handle/id
GLuint ShaderProgram::getHandle()
{
	return this->id;
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
	glUniform1i(glGetUniformLocation(this->id, target.c_str()), v);
}

void ShaderProgram::setFloat(const string &target, float v) const
{
	glUniform1f(glGetUniformLocation(this->id, target.c_str()), v);
}

void ShaderProgram::setVec2(const string &target, const glm::vec2 &v) const
{
	glUniform2fv(glGetUniformLocation(this->id, target.c_str()), 1, &v[0]);
}

void ShaderProgram::setVec3(const string &target, const glm::vec3 &v) const
{
	glUniform3fv(glGetUniformLocation(this->id, target.c_str()), 1, &v[0]);
}

void ShaderProgram::setVec4(const string &target, const glm::vec4 &v) const
{
	glUniform4fv(glGetUniformLocation(this->id, target.c_str()), 1, &v[0]);
}

void ShaderProgram::setMat3(const string &target, const glm::mat3 &v) const
{
	glUniformMatrix3fv(glGetUniformLocation(this->id, target.c_str()), 1, GL_FALSE, &v[0][0]);
}

void ShaderProgram::setMat4(const string &target, const glm::mat4 &v) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->id, target.c_str()), 1, GL_FALSE, &v[0][0]);
}
