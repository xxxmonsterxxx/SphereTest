#include <iostream>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <glew.h>

// GLFW
#include <glfw3.h>

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI glm::pi <GLfloat> ()
#define stacks 50
#define slices 50
#define radius 0.5

int numberOfPoints = 0;
int numberOfIndexes = 0;
GLfloat vertices[(stacks + 1) * slices * 3];
GLuint indices[stacks * slices * 10];

int dx = 0, dy = 0, dz = 0;

const GLchar* vertexShaderSource =
"#version 330 core\n"

"layout (location = 0) in vec3 position;\n"

"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"out vec3 ourColor;\n"

"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(position, 1.0f);\n"
"	ourColor = vec3(1.0f,0.0f,0.0f);\n"
"}\n\0";

const GLchar* fragmentShaderSource =
"#version 330 core\n"

"in vec3 ourColor;\n"

"out vec4 color;\n"

"void main()\n"
"{\n"
"	color = vec4(ourColor,1.0f);\n"
"}\n\0";


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void getsphereCoords();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	getsphereCoords();

	/////////////////ATRIB POINTER///////////////////////////
	GLuint VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndexes * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numberOfPoints * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/////////////////ATRIB POINTER///////////////////////////

	/////////////////SHADERS/////////////////////////////////
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/////////////////SHADERS/////////////////////////////////

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Game loop

	



	while (!glfwWindowShouldClose(window))
	{

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Create transformations
		glm::mat4 view;
		glm::mat4 projection;
		view = glm::translate(view, glm::vec3((float)dx * 0.15f + 0.0f, (float)dy * 0.15f + 0.0f, (float)dz * 0.15f + -1.5f));
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
		GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
		// Calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model;
		//GLfloat angle = 20.0f * i;
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawElements(GL_TRIANGLES, numberOfIndexes, GL_UNSIGNED_INT, 0);


		// Now with the uniform matrix being replaced with new transformations, draw it again.
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		dy--;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		dy++;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		dx++;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		dx--;
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		dz--;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		dz++;
	}
}

void getsphereCoords()
{
	// Calc The Vertices
	for (int i = 0; i <= slices; i++)
	{
		float phi = i * (glm::pi<float>() / slices) ;


		// Loop Through Slices
		for (int j = 0; j < stacks; j++)
		{
			float theta = j * (glm::pi <float>() * 2 / stacks);

			// Calc The Vertex Positions
			float x = cosf(theta) * sinf(phi);
			float y = cosf(phi);
			float z = sinf(theta) * sinf(phi);

			// Push Back Vertex Data
			vertices[numberOfPoints++] = x * radius;
			vertices[numberOfPoints++] = y * radius;
			vertices[numberOfPoints++] = z * radius;
		}
	}
	// Calc The Index Positions
	for (int i = 0; i < numberOfPoints / 3 - stacks; i++)
	{
		if ((i + 1) % stacks == 0)
		{
			indices[numberOfIndexes++] = i;
			indices[numberOfIndexes++] = i - stacks + 1;
			indices[numberOfIndexes++] = i + stacks;

			indices[numberOfIndexes++] = i - stacks + 1;
			indices[numberOfIndexes++] = i + stacks;
			if (i + 1 == numberOfPoints / 3)
				indices[numberOfIndexes++] = numberOfPoints - stacks;
			else
				indices[numberOfIndexes++] = i + 1;
		}
		else
		{
			indices[numberOfIndexes++] = i;
			indices[numberOfIndexes++] = i + 1;
			indices[numberOfIndexes++] = i + stacks;

			indices[numberOfIndexes++] = i + 1;
			indices[numberOfIndexes++] = i + stacks;
			indices[numberOfIndexes++] = i + stacks + 1;
		}
	}
}