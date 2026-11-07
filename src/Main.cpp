#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
static std::string loadShaderSource(const std::string& filePath);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderTwo = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgram = glCreateProgram();
    unsigned int shaderProgramTwo = glCreateProgram();
	std::string vertexSourceString = loadShaderSource("assets/shaders/vertex_shader.glsl");
	std::string fragmentSourceString = loadShaderSource("assets/shaders/fragment_shader.glsl");
	std::string fragmentSourceStringTwo = loadShaderSource("assets/shaders/fragment_shader_two.glsl");
	const char* vertexSource = vertexSourceString.c_str();
	const char* fragmentSource = fragmentSourceString.c_str();
	const char* fragmentSourceTwo = fragmentSourceStringTwo.c_str();


    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    // fragment shader
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    glShaderSource(fragmentShaderTwo, 1, &fragmentSourceTwo,nullptr);
    glCompileShader(fragmentShaderTwo);
    // link shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
	glAttachShader(shaderProgramTwo, vertexShader);
	glAttachShader(shaderProgramTwo, fragmentShaderTwo);
	glLinkProgram(shaderProgramTwo);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float verticesTriangleOne[] = {
		-0.33f, 0.33f, 0.0f,
		-0.66f, -0.33f, 0.0f,
		0.0f, -0.33f, 0.0f,
    }; 

	float verticesTriangleTwo[] = {
		0.0f, -0.33f, 0.0f,
		0.33f, 0.33f, 0.0f,
		0.66f, -0.33f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 4
	};

	// Create and bind buffers with data
    unsigned int EBO;
	unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
	glGenBuffers(1, &EBO);

	// Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(VAOs[0]);
	// Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangleOne), verticesTriangleOne, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangleTwo), verticesTriangleTwo, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	// Make sure to unbind VAO FIRST as it keeps track of EBO binds
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Wireframe mode if wanted
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// Activate the shader
        glUseProgram(shaderProgram);

		float timeValue = glfwGetTime();
		std::cout << sin(timeValue) << std::endl;
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColourLocation = glGetUniformLocation(shaderProgram, "ourColour");
		glUniform4f(vertexColourLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		// binding the VAO also binds the EBO because the EBO reference is stored in the VAO
        glBindVertexArray(VAOs[0]); 
        glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgramTwo);
		glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0); // no need to unbind it every time 
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
	glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static std::string loadShaderSource(const std::string& filePath) {
	std::ifstream fileStream(filePath);

	if (!fileStream.is_open()) {
		std::cerr << "Failed to open: " << filePath << '\n';
		return "";
	}

	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	return buffer.str();
}
