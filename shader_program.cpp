#include "shader_program.hpp"

ShaderProgram ShaderProgram::DEFAULT_SHADER;

/**
 * Loads the source code for the vertex shader and fragment shader, compiles them, and links them.
 * 
 * @param vertexShaderPath the path to the vertex shader.
 * @param fragmentShaderPath the path to the fragment shader.
 */
void ShaderProgram::loadSources(const GLchar *vertexShaderPath, const GLchar *fragmentShaderPath){
	// Read source code
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	std::string vertexShaderSource;
	std::string fragmentShaderSource;

	try{
		fileStream.open(vertexShaderPath);

		std::stringstream ss;
		ss << fileStream.rdbuf();

		fileStream.close();
		vertexShaderSource = ss.str();
	}
	catch(std::ifstream::failure e){
		std::cout << "Failed to open file at " << vertexShaderPath << std::endl;
	}

	try{
		fileStream.open(fragmentShaderPath);

		std::stringstream ss;
		ss << fileStream.rdbuf();

		fileStream.close();
		fragmentShaderSource = ss.str();
	}
	catch(std::ifstream::failure e){
		std::cout << "Failed to open file at " << fragmentShaderPath << std::endl;
	}

	const char *vertexShaderSourceCStr = vertexShaderSource.c_str();
	const char *fragmentShaderSourceCStr = fragmentShaderSource.c_str();


	// Compile
	this->id = glCreateProgram();
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	try{
		glShaderSource(vertexShaderID, 1, &vertexShaderSourceCStr, NULL);
		glCompileShader(vertexShaderID);
		checkCompilation(vertexShaderID);
	}
	catch(ShaderException &e){
		std::cout << "Vertex shader compilation failed:" << std::endl;
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	try{
		glShaderSource(fragmentShaderID, 1, &fragmentShaderSourceCStr, NULL);
		glCompileShader(fragmentShaderID);
		checkCompilation(fragmentShaderID);
	}
	catch(ShaderException &e){
		std::cout << "Fragment shader compilation failed:" << std::endl;
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	// Link
	try{
		glAttachShader(this->id, vertexShaderID);
		glAttachShader(this->id, fragmentShaderID);
		glLinkProgram(this->id);
		checkLinking();
	}
	catch(ShaderException &e){
		std::cout << "Shader linking failed:" << std::endl;
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * Checks whether a shader compiled correctly.
 * 
 * @param shaderID the OpenGL id of the shader in question.
 */
void ShaderProgram::checkCompilation(unsigned int shaderID){
	int success;
	int logSize;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
	char infoLog[logSize];
	glGetShaderInfoLog(shaderID, logSize, &logSize, &infoLog[0]);

	if(!success){
		throw ShaderException(infoLog);
	}
}

/**
 * Checks whether this shader program linked correctly.
 */
void ShaderProgram::checkLinking(){
	int success;
	int logSize;
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);

	glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &logSize);
	char infoLog[logSize];
	glGetProgramInfoLog(this->id, logSize, &logSize, &infoLog[0]);

	if(!success){
		throw ShaderException(infoLog);
	}
}

/**
 * Tells OpenGL to use this shader program from now on.
 */
void ShaderProgram::use(){
	glUseProgram(this->id);
}

unsigned int ShaderProgram::getID(){
	return this->id;
}
