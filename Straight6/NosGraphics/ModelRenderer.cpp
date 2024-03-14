#include "ModelRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "utils.h"
#include "ShaderUtils.h"

namespace GE {

	//create and compile shaders
	//creates the project and links it - also creates the vertex buffer
	void ModelRenderer::Init()
	{
		glEnable(GL_DEPTH_TEST);
		mr_vShader = LoadShaderFile("../NosGraphics/shaders/vShader.shader");
		mr_fShader = LoadShaderFile("../NosGraphics/shaders/fShader.shader");
		SetupShaderProgram(mr_vShader, mr_fShader, &programID);

		//get link to vertexPos2D so linking attributes to verticies is possible
		vertexPos3DLocation = glGetAttribLocation(programID, "vertexPos3D");
		if (vertexPos3DLocation == -1)
		{
			std::cerr << "Problem getting vertexPos3D" << std::endl;
		}

		// linking the colour to the vertex shaderindicating what colour is represented in the pinpline
		vertexUVLocation = glGetAttribLocation(programID, "vUV");
		if (vertexUVLocation == -1)
		{
			std::cerr << "Problem getting vUV" << std::endl;
		}

		//get uniform ID in shader so the program can send data to it
		viewUniformID		= glGetUniformLocation(programID, "viewMat");
		projectionUniformID = glGetUniformLocation(programID, "projMat");
		transformUniformID  = glGetUniformLocation(programID, "transformMat");

		//tidy up the buffer other setting up
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void ModelRenderer::Update()
	{
	}

	void ModelRenderer::ManipulateVerticies(glm::mat4& tranformationMat)
	{
		using namespace glm;
		tranformationMat = translate(tranformationMat, vec3(posX, posY, posZ));
		tranformationMat = rotate(tranformationMat, radians(RotX), vec3(0.5f, 0.0f, 0.0f));
		tranformationMat = rotate(tranformationMat, radians(RotY), vec3(0.0f, 0.5f, 0.0f));
		tranformationMat = rotate(tranformationMat, radians(RotZ), vec3(0.0f, 0.0f, 0.5f));
		tranformationMat = scale(tranformationMat, vec3(ScaleX, ScaleY, ScaleZ));
	}


	void ModelRenderer::Draw(Camera* cam, Model* model)
	{
		glEnable(GL_CULL_FACE);
		glm::mat4 tranformationMat = glm::mat4(1.0f);

		ManipulateVerticies(tranformationMat);
		//get teh view and projection matricies
		glm::mat4 viewMat		= cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		//use program created from init
		glUseProgram(programID);

		glUniformMatrix4fv(transformUniformID, 1, GL_FALSE, value_ptr(tranformationMat));
		glUniformMatrix4fv(viewUniformID, 1, GL_FALSE, value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformID, 1, GL_FALSE, value_ptr(projectionMat));
		

		//select the vertex buffer into context
		glBindBuffer(GL_ARRAY_BUFFER, model->getVertices());

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

		model->BindTexture();
		//draw triangles base off its verticies
		glDrawArrays(GL_TRIANGLES, 0, model->getNumOfVertices());

		//unselect the attribute
		glDisableVertexArrayAttrib(GL_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(vertexPos3DLocation);

		glDisableVertexAttribArray(vertexUVLocation);

		//unselect the vertext buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//unselect the program from this context
		glUseProgram(0);

		glDisable(GL_CULL_FACE);
	}

	void ModelRenderer::Draw(Camera* cam, Terrain* terrain)
	{
		glm::mat4 tranformationMat = glm::mat4(1.0f);

		//get teh view and projection matricies
		glm::mat4 viewMat		= cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		//use program created from init
		glUseProgram(programID);

		glUniformMatrix4fv(transformUniformID, 1, GL_FALSE, value_ptr(tranformationMat));
		glUniformMatrix4fv(viewUniformID, 1, GL_FALSE, value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformID, 1, GL_FALSE, value_ptr(projectionMat));


		//select the vertex buffer into context
		glBindBuffer(GL_ARRAY_BUFFER, terrain->getVertices());

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

		glActiveTexture(GL_TEXTURE0);
		terrain->BindTexture(&programID);
		
		//draw triangles base off its verticies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->getIndices());

		glDrawElements(GL_TRIANGLES, terrain->getIndexCount(), GL_UNSIGNED_INT, nullptr);

		//unselect the attribute
		glDisableVertexArrayAttrib(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(vertexPos3DLocation);

		glDisableVertexAttribArray(vertexUVLocation);

		//unselect the vertext buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//unselect the program from this context
		glUseProgram(0);
	}

	void ModelRenderer::Draw(Camera* cam, SkyDome* sky)
	{
		glDisable(GL_DEPTH_TEST);
		glm::mat4 tranformationMat = glm::mat4(1.0f);
		//get teh view and projection matricies
		glm::mat4 viewMat		= cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		tranformationMat = glm::translate(tranformationMat, glm::vec3(cam->getPosX(), cam->getPosY(), cam->getPosZ()));
		//use program created from init
		glUseProgram(programID);

		glUniformMatrix4fv(transformUniformID, 1, GL_FALSE, value_ptr(tranformationMat));
		glUniformMatrix4fv(viewUniformID, 1, GL_FALSE, value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformID, 1, GL_FALSE, value_ptr(projectionMat));


		//select the vertex buffer into context
		glBindBuffer(GL_ARRAY_BUFFER, sky->getVertices());

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

		glActiveTexture(GL_TEXTURE0);
		sky->BindTexture(&programID);

		//draw triangles base off its verticies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky->getIndices());

		glDrawElements(GL_TRIANGLES, sky->getIndexCount(), GL_UNSIGNED_INT, nullptr);

		//unselect the attribute
		glDisableVertexArrayAttrib(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(vertexPos3DLocation);
		glDisableVertexAttribArray(vertexUVLocation);

		//unselect the vertext buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//unselect the program from this context
		glUseProgram(0);
		glEnable(GL_DEPTH_TEST);
	}

	//release the object allocated for the program and vertex buffer object
	void ModelRenderer::Destroy()
	{

		glDeleteProgram(programID);

		glDeleteBuffers(1, &vboTriangle);
	}

}