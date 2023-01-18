#include"shaderClass.h"


static ShaderProgramSource ParseShader(const std::string& filepath) {

	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {

			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;

			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };
}


// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const std::string& filepath)
{
	// Read vertexFile and fragmentFile and store the strings
	ShaderProgramSource source = ParseShader(filepath);
	std::cout << "VerteX" << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "Fragment" << std::endl;
	std::cout << source.FragmentSource << std::endl;

	// Convert the shader source strings into character arrays
	 

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	 //converting string shader to char
	const char* vertex_source = source.VertexSource.c_str();
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertex_source, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	 //Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragment_source = source.FragmentSource.c_str();
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragment_source, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	  
	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	//glValidateProgram(ID);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}

// Checks if the different Shaders have compiled properly

void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}