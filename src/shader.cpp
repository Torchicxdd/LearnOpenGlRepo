#include <graphics_learning/shader.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode = loadShaderSource(vertexPath);
	std::string fragmentCode = loadShaderSource(fragmentPath);
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = createShader(vShaderCode, true);
	fragment = createShader(fShaderCode, false);

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::deleteShader() const {
	glDeleteProgram(ID);
}

std::string Shader::loadShaderSource(const std::string &filePath) {
	std::ifstream fileStream(filePath);

	if (!fileStream.is_open()) {
		std::cerr << "Failed to open: " << filePath << '\n';
		return "";
	}

	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	return buffer.str();
}

unsigned int Shader::createShader(const char* shaderCode, const bool isVertex) {
	unsigned int shader;
	int success;
	char infoLog[512];

	shader = isVertex ? glCreateShader(GL_VERTEX_SHADER) : glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);

	// print compiler errors if any
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::" << (isVertex ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}