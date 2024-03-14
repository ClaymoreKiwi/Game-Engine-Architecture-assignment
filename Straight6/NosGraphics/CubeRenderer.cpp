#include "CubeRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "utils.h"

namespace GE {

	Vertex vertexCData[] =
	{
		//pos						//r g b a
		Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f),
		Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f),
		Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f),

		Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f),
		Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.5f),
		Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f),

		// Right side face
		Vertex(0.5f, 0.5f, -0.5f, 0.2f, 1.0f),
		Vertex(0.5f, 0.5f, 0.5f, 0.5f, 1.0f),
		Vertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f),

		Vertex(0.5f, -0.5f, 0.5f, 0.1f, 1.0f),
		Vertex(0.5f, -0.5f, -0.5f, 0.3f, 1.0f),
		Vertex(0.5f, 0.5f, -0.5f, 0.9f, 1.0f),

		//// Back face, note that points are in counter clockwise order
		Vertex(-0.5f, -0.5f, 0.5f, 0.5f, 0.0f),
		Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.2f),
		Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f),

		Vertex(0.5f, 0.5f, 0.5f, 0.4f, 0.1f),
		Vertex(-0.5f, 0.5f, 0.5f, 0.4f, 1.0f),
		Vertex(-0.5f, -0.5f, 0.5f, 0.4f, 0.4f),

		//// Left side face, note that points are in counter clockwise order
		Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.5f),
		Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f),
		Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 0.3f),

		Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f),
		Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 1.0f),
		Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f),

		//// Top face
		Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f),
		Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.1f),
		Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.3f),

		Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.6f),
		Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f),
		Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.4f),

		//// Bottom face, note that points are in counter clockwise order
		Vertex(-0.5f, -0.5f, -0.5f, 0.5f, 0.0f),
		Vertex(0.5f, -0.5f, -0.5f, 0.3f, 1.0f),
		Vertex(0.5f, -0.5f, 0.5f, 0.7f, 1.0f),

		Vertex(0.5f, -0.5f, 0.5f, 0.1f, 1.0f),
		Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.3f),
		Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f),
	};

	//create and compile shaders
	//creates the project and links it - also creates the vertex buffer
	void CubeRenderer::Init()
	{
		//create vertex shader first
		//shaders always come first before attaching the program
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//Vertext Shader code - lecture
		const GLchar* V_ShaderCode[] =
		{
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"in vec4 vColour;"
			"out vec4 fColour;"
			"uniform mat4 viewMat;\n"
			"uniform mat4 projMat;\n"
			"uniform mat4 transformMat;\n"
			"void main(){\n"
			"gl_Position = projMat * viewMat * transformMat * vec4(vertexPos3D,1);\n"
			"fColour = vColour;"
			"}\n"
		};
		//copy the source to openGL ready to compile
		glShaderSource(vertexShader, 1, V_ShaderCode, NULL);
		//compile shader
		glCompileShader(vertexShader);

		//check for any errors - assume it doesnt compile
		GLint isShaderCompiledOK = GL_FALSE;

		//get the compile status of the program
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiledOK);
		if (isShaderCompiledOK != GL_TRUE)
		{
			std::cerr << "Unable to compile vertex shader" << std::endl;
			DisplayShaderCompilerError(vertexShader);
			return;
		}

		//enable depth buffer
		glEnable(GL_DEPTH_TEST);
		//fragment shader creation
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//code from lecture - fragment shader
		const GLchar* F_ShaderCode[] =
		{
			"#version 140\n"
			"in vec4 fColour;"
			"out vec4 fragmentColour;\n"
			"void main()\n"
			"{\n"
			"fragmentColour = fColour;\n"
			"}\n"
		};

		//copy the source to openGL ready to compile
		glShaderSource(fragmentShader, 1, F_ShaderCode, NULL);

		//compile fragment shader
		glCompileShader(fragmentShader);


		//assume compilation error
		isShaderCompiledOK = GL_FALSE;

		//get error status
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);
		if (isShaderCompiledOK != GL_TRUE)
		{
			std::cerr << "Unable to compile fragment shader" << std::endl;
			DisplayShaderCompilerError(fragmentShader);
			return;
		}

		//create the program after shader creation
		programID = glCreateProgram();

		//attached both shaders
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);

		//link program to an exe - exists in graphics meemory
		glLinkProgram(programID);

		//check any linking errors
		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(programID, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE)
		{
			std::cerr << "Failed to link program" << std::endl;
		}

		//get link to vertexPos2D so linking attributes to verticies is possible
		vertexPos3DLocation = glGetAttribLocation(programID, "vertexPos3D");
		if (vertexPos3DLocation == -1)
		{
			std::cerr << "Problem getting vertexPos3D" << std::endl;
		}

		// linking the colour to the vertex shaderindicating what colour is represented in the pinpline
		vertexUVLocation = glGetAttribLocation(programID, "vColour");
		if (vertexUVLocation == -1)
		{
			std::cerr << "Problem getting vColour" << std::endl;
		}

		//get uniform ID in shader so the program can send data to it
		viewUniformID = glGetUniformLocation(programID, "viewMat");
		projectionUniformID = glGetUniformLocation(programID, "projMat");
		//transfrom matrix uniform
		transformUniformID = glGetUniformLocation(programID, "transformMat");

		//create buffer object
		glGenBuffers(1, &vboCube);
		glBindBuffer(GL_ARRAY_BUFFER, vboCube);

		//transfer to graphics memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCData), vertexCData, GL_STATIC_DRAW);
		//tidy up the buffer ofter setting up
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void CubeRenderer::Update()
	{
	}

	void CubeRenderer::Draw(Camera* cam)
	{
		using namespace glm;
		mat4 tranformationMat = mat4(1.0f);

		tranformationMat = translate(tranformationMat, vec3(posX, posY, posZ));
		tranformationMat = rotate(tranformationMat, radians(RotX), vec3(2.0f, 1.0f, 1.0f));
		tranformationMat = rotate(tranformationMat, radians(RotY), vec3(1.0f, 2.0f, 1.0f));
		tranformationMat = rotate(tranformationMat, radians(RotZ), vec3(1.0f, 1.0f, 2.0f));
		tranformationMat = scale(tranformationMat, vec3(ScaleX, ScaleY, ScaleZ));
		//get teh view and projection matricies
		mat4 viewMat	   = cam->getViewMatrix();
		mat4 projectionMat = cam->getProjectionMatrix();

		//use program created from init
		glUseProgram(programID);

		glUniformMatrix4fv(transformUniformID, 1, GL_FALSE, value_ptr(tranformationMat));
		glUniformMatrix4fv(viewUniformID, 1, GL_FALSE, value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformID, 1, GL_FALSE, value_ptr(projectionMat));


		//select the vertex buffer into context
		glBindBuffer(GL_ARRAY_BUFFER, vboCube);

		//enable the attributes that will be used for the buffer object
		glEnableVertexAttribArray(vertexPos3DLocation);

		//define the structure for OpenGL to select the values from the buffer
		//storing Vpos2D in attribute
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		//enable the attributes to pass in colours to the buffer object
		glEnableVertexAttribArray(vertexUVLocation);

		//define the structure for OpenGL to select the values from the buffer
		//storing Colour values in attribute
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		//clear depth buffer
		glClear(GL_DEPTH_BUFFER_BIT);
		//draw triangles base off its verticies
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexCData) / sizeof(Vertex));

		//unselect the attribute
		glDisableVertexArrayAttrib(GL_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(vertexPos3DLocation);

		glDisableVertexAttribArray(vertexUVLocation);

		//unselect the vertext buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//unselect the program from this context
		glUseProgram(0);
	}

	//release the object allocated for the program and vertex buffer object
	void CubeRenderer::Destroy()
	{

		glDeleteProgram(programID);

		glDeleteBuffers(1, &vboCube);
	}

}