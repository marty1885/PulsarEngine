#include <PulsarShader.hpp>
#include <PulsarTexture.hpp>
#include <Pulsar.hpp>
using namespace Pulsar;

Shader::Shader()
{
	program = glCreateProgram();
	if(program == 0)
		cout << "Failed to create shader" << endl;
}

Shader::~Shader()
{
	glDeleteProgram(program);

	int size = shaderList.size();
	for(int i=0;i<size;i++)
		glDeleteShader(shaderList[i]);
	shaderList.clear();
}

bool Shader::addVertexShader(string text)
{
	return addProgram(text, GL_VERTEX_SHADER);
}

bool Shader::addFragmentShader(string text)
{
	return addProgram(text, GL_FRAGMENT_SHADER);
}

bool Shader::compile()
{
	glLinkProgram(program);

	int result;
	glGetProgramiv(program,GL_LINK_STATUS,&result);
	if(result != GL_TRUE)
	{
		int size = -1;
		glGetProgramiv(program,GL_INFO_LOG_LENGTH,&size);
		char* data = new char[size];
		int length;
		glGetProgramInfoLog(program,size,&length,data);
		cout << "failed to link program" << endl << data << endl;
		delete [] data;
		return false;
	}

	glValidateProgram(program);
	glGetProgramiv(program,GL_VALIDATE_STATUS,&result);
	if(result == 0)
	{
		int size = -1;
		glGetProgramiv(program,GL_INFO_LOG_LENGTH,&size);
		char* data = new char[size];
		int length;
		glGetProgramInfoLog(program,size,&length,data);
		cout << "failed to validate program" << endl << data << endl;
		delete [] data;
		return false;
	}
	//cache uniforms in a hash table for fast access
	addAllUniform();
	return true;
}

void Shader::bind()
{
	glUseProgram(program);
}

void Shader::unbind()
{
	glUseProgram(0);
}

bool Shader::addProgram(string text, GLenum type)
{

	//Create vertex shader
	GLuint shader = glCreateShader(type);
	const char* str = text.c_str();

	if(shader == 0)
	{
		cout << "Failed to create shader" << endl;
		return false;
	}

	//Set vertex source
	glShaderSource(shader, 1, &str, NULL);

	//Compile vertex source
	glCompileShader(shader);

	//Check vertex shader for errors
	GLint result = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(result != GL_TRUE)
	{
		int size = -1;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&size);
		char* data = new char[size];
		int length;
		glGetShaderInfoLog(shader,size,&length,data);
		string shaderType = "Unknown";
		switch (type)
		{
			case GL_VERTEX_SHADER:
				shaderType = "vertex";
				break;
			case GL_FRAGMENT_SHADER:
				shaderType = "fragment";
				break;
			default:
				shaderType = "Unknown";
		}
		cout << "failed to compile " << shaderType << " shader" << endl << data << endl;
		delete [] data;
		return false;
	}
	//Attach vertex shader to program
	glAttachShader(program, shader);
	shaderList.push_back(shader);
	return true;
}

bool Shader::setParameter(string name,float val)
{
	//GLint uniformLocation = glGetUniformLocation(program,name.c_str());
	GLint uniformLocation = getUniform(name);
	if(uniformLocation == -1)
	{
		cout << "Error : Uniform \"" << name << "\" not found" << endl;
		return false;
	}
	glUniform1f(uniformLocation,val);
	return true;
}

bool Shader::setParameter(string name, vec2 val)
{
	GLint uniformLocation = getUniform(name);
	if(uniformLocation == -1)
	{
		cout << "Error : Uniform \"" << name << "\" not found" << endl;
		return false;
	}
	glUniform2fv(uniformLocation,1,(float*)&val);
	return true;
}

bool Shader::setParameter(string name, vec3 val)
{
	GLint uniformLocation = getUniform(name);
	if(uniformLocation == -1)
	{
		cout << "Error : Uniform \"" << name << "\" not found" << endl;
		return false;
	}
	glUniform3fv(uniformLocation,1,(float*)&val);
	return true;
}

bool Shader::setParameter(string name, vec4 val)
{
	GLint uniformLocation = getUniform(name);
	if(uniformLocation == -1)
	{
		cout << "Error : Uniform \"" << name << "\" not found" << endl;
		return false;
	}
	glUniform4fv(uniformLocation,1,(float*)&val);
	return true;
}

bool Shader::setParameter(string name, mat3x3 val)
{
	GLint uniformLocation = getUniform(name);
	if(uniformLocation == -1)
	{
		cout << "Error : Uniform \"" << name << "\" not found" << endl;
		return false;
	}
	glUniformMatrix3fv(uniformLocation,1,GL_FALSE,(float*)&val);
	return true;
}

bool Shader::setParameter(string name, mat4x4 val)
{
	GLint uniformLocation = getUniform(name);
	if(uniformLocation == -1)
	{
		cout << "Error : Uniform \"" << name << "\" not found" << endl;
		return false;
	}
	glUniformMatrix4fv(uniformLocation,1,GL_FALSE,(float*)&val);
	return true;
}

bool Shader::setParameter(string name, bool val)
{
	GLint uniformLocation = getUniform(name);
	if(uniformLocation == -1)
	{
		cout << "Error : Uniform \"" << name << "\" not found" << endl;
		return false;
	}
	glUniform1i(uniformLocation,val);
	return true;
}

bool Shader::setParameter(string name, int val)
{
	GLint uniformLocation = getUniform(name);
	if(uniformLocation == -1)
	{
		cout << "Error : Uniform \"" << name << "\" not found" << endl;
		return false;
	}
	glUniform1i(uniformLocation,val);
	return true;
}

void Shader::addAllUniform()
{
	int total = -1;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &total);

	int maxLength;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
	char* name = new char[maxLength];
	for(int i=0; i<total; i++)
	{
		int nameLen=-1, num=-1;
		GLenum type = GL_ZERO;
		glGetActiveUniform(program, (GLuint)i, maxLength,
			&nameLen, &num, &type, name);
		GLuint location = glGetUniformLocation(program, name);
		uniforms.insert({name,location});
	}
	delete [] name;
}

GLint Shader::getUniform(string name)
{
	unordered_map<string,GLint>::const_iterator search = uniforms.find(name);
	if(search == uniforms.end())
		return -1;
	return search->second;
}



void ThreeDShader::setTexture(Texture* tex)
{
	texture = tex;
}

bool ThreeDShader::compile()
{
	bool success = true;
	success = Shader::compile();
	if(success == false)
		return false;

	//Check if all uniforms we need are avliable
	string requiredUniform [] = {"transformMatrix","cameraMatrix","projectionMatrix"};
	for(string& str : requiredUniform)
	{
		if(getUniform(str) == -1)
		{
			cout << "Warrning : Missing uniform \"" << str << "\" in ThreeDShader. Maybe some programming error has occurred." << endl;
			success = false;
		}
	}

	if(success == false)
		return false;

	return true;
}

void ThreeDShader::bind()
{
	if(texture != NULL)
		texture->bind();

	Shader::bind();
	updateInternalParametes();
}

void ThreeDShader::unbind()
{
	if(texture != NULL)
		texture->unbind();
	Shader::unbind();
}

void ThreeDShader::setTransformation(mat4 transformation)
{
	transformMatrix = transformation;
}

void ThreeDShader::setCamera(Camera* cam)
{
	camera = cam;
}

void ThreeDShader::setProjection(Projection* project)
{
	projection = project;
}

void ThreeDShader::updateInternalParametes()
{
	setParameter("transformMatrix",transformMatrix);
	setParameter("cameraMatrix",camera->getCameraMatrix());
	setParameter("projectionMatrix",projection->getProjectionMatrix());
}