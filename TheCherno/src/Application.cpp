#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Za citanje shader programa iz file-a
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

/*#define ASSERT(x) if((!x)) __debugbreak();  
#define GLCall(x)  GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() 
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
};*/

/*
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum struct ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) //find ne vraca boolean nego poziciju tog stringa, ako ne nadje nista vrati npos
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); //&source[0]
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//TODO: Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

//Its static so it doesnt link in other translation units and c++ files
//Here we write code needed to compile these two shaders!
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//Linking shaders to one program so we can use both of them
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//Shaders have beent linked to a program so we can delete intermediates 
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
*/

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); //Odredimo brzinu zamene buffera za prikaz!

	//Ovo se radi nakon sto se napravi context
	if (glewInit() != GLEW_OK) {
		std::cout << "Error - GLEW NOT OK" << std::endl;
	}
	else {
		std::cout << glGetString(GL_VERSION) << std::endl;
	}

	//Generating a buffer
	float positions[] = {
		-0.5f, -0.5f, // 0 position (bottom left)
		0.0f, 0.0f, // 0 texture
		0.5f, -0.5f, // 1 position (bottom right)
		1.0f, 0.0f, // 1 texture
		0.5f, 0.5f, // 2 postion (top right)
		1.0f, 1.0f, // 2 texture
		-0.5f, 0.5f, // 3 position (top left)
		0.0f, 1.0f // 3 texture
	};

	unsigned int indeces[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	/*
	unsigned int vao; //vertex array object
	//Bitno da se pozove  i bajnduje pre pravljenja buffera i definisanje layouta, kasnije moze samo vao da se binduje + index buffer
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));
	*/
	//VertexArray --> sada je sav kod za vertex array i layout(vertexattribpinter u ovoj klasi
	VertexArray va;

	//VertexBuffer --> sada je sav kod u kontruktoru
	VertexBuffer vb(positions, 4 * 4 * sizeof(float)); // We have 4 flaots (2 for position and 2 for texture)

	//VertexBufferLayout --> 
	VertexBufferLayout layout;
	layout.Push<float>(2); // Pushing positions of vertex
	layout.Push<float>(2); // Pushing texture of vertex

	va.AddBUffer(vb, layout);
	/*
	//Potrebno prvo bind-ovati pa tek onda moze da se poziva ova funkcija
	//Za svaki atribut vretex-a treba pozvati ovo (u nasem slucju samo jednom -> pozicija)
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0));
	GLCall(glEnableVertexAttribArray(0)); //Enable-uje vertex atribut na indexu 0 odnosno ovo gore sto smo pisali
	*/

	//IndexBuffer --> sada je sav kod u kontruktoru
	IndexBuffer ib(indeces, 6);

	/*
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource); //returns program
	//Binding shaders
	GLCall(glUseProgram(shader));
	*/
	//Shader and progrma --> sve vezano za shader i ceo program koji sadrzi sveshadere se nalazi u ovoj klasi
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	/*
	//Uniform creation and sending  => JAKO BUTNO DA SE PRVO BIND-UJE SHADER SA glUseProgram(program)!
	int location = glGetUniformLocation(shader, "u_Color"); 
	ASSERT((location != -1)); //Ako je -1 znaci da nije nasao nas Uniform (moze da vrati -1 ako se taj uniform posotji u shaderu ali se ne koristi
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));
	*/
	//shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

	//Texture
	Texture texture("res/textures/ChernoLogo.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0); // Set to 0 because we bound our texture to slot 0!

	// Clear everything before starting the loop
	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	Renderer renderer;

	//Color changer
	float r = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);
		renderer.Clear();

		/*
		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
		*/
		shader.Bind();
		shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		/*
		//GLCall(glBindVertexArray(vao));
		va.Bind();
		ib.Bind();

		//glDrawArrays(GL_TRIANGLES, 0, 6); //Kad nemamo index buffer
		//GLClearError();
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //--> Kada imamo index buffer 
		//ASSERT(GLLogCall());
		*/
		renderer.Draw(va, ib, shader);

		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	//glDeleteProgram(shader);  --> When we reach and of the scope it will be automaticlly deleted by the destructor of shader

	glfwTerminate();
	return 0;
}	