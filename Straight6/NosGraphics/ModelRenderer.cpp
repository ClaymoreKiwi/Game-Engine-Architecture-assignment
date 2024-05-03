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
		mr_vShader = LoadShaderFile("../NosGraphics/shaders/vShader.shader");
		mr_fShader = LoadShaderFile("../NosGraphics/shaders/fShader.shader");
		SetupShaderProgram(mr_vShader, mr_fShader, &programID);

		billboardRend= std::make_unique<BillboardRenderer>();

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

		vertexNormal = glGetAttribLocation(programID, "vertexNormal");
		if (vertexNormal == -1)
		{
			std::cerr << "Problem getting vertex Normal" << std::endl;
		}

		//get uniform ID in shader so the program can send data to it
		viewUniformID		= glGetUniformLocation(programID, "viewMat");
		projectionUniformID = glGetUniformLocation(programID, "projMat");
		transformUniformID  = glGetUniformLocation(programID, "transformMat");
		viewPosID			= glGetUniformLocation(programID, "viewPos");

		//light info
		lightColour= glGetUniformLocation(programID,"lightColour");
		lightPos   = glGetUniformLocation(programID,"lightPos");
		ambient	   = glGetUniformLocation(programID,"ambient");
		//tidy up the buffer other setting up
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void ModelRenderer::Update()
	{
	}

	void ModelRenderer::ManipulateVerticies(glm::mat4& tranformationMat, Model* model)
	{
		using namespace glm;
		tranformationMat = translate(tranformationMat, pos + model->getPos());
		tranformationMat = rotate(tranformationMat, radians(RotX +model->getRotX()), vec3(0.5f, 0.0f, 0.0f));
		tranformationMat = rotate(tranformationMat, radians(RotY +model->getRotY()), vec3(0.0f, 0.5f, 0.0f));
		tranformationMat = rotate(tranformationMat, radians(RotZ +model->getRotZ()), vec3(0.0f, 0.0f, 0.5f));
		tranformationMat = scale(tranformationMat, vec3(ScaleX + model->getScaleX(), ScaleY + model->getScaleY(), ScaleZ + model->getScaleZ()));
	}

	void ModelRenderer::DrawWithIndecies(GLuint vertex, GLuint index, GLuint indexNumber)
	{
		//select the vertex buffer into context
		glBindBuffer(GL_ARRAY_BUFFER, vertex);

		//enable the attributes that will be used for the buffer object
		//location x,y,z and its appropriate jumps
		glEnableVertexAttribArray(vertexPos3DLocation);
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		//enable the attributes to pass in colours to the buffer object
		//location u,v and its appropriate jumps
		glEnableVertexAttribArray(vertexUVLocation);
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		glEnableVertexAttribArray(vertexNormal);
		glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//draw triangles base off its indicies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		glDrawElements(GL_TRIANGLES, indexNumber, GL_UNSIGNED_INT, nullptr);

		//unselect the attribute
		glDisableVertexArrayAttrib(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(vertexPos3DLocation);
		glDisableVertexAttribArray(vertexUVLocation);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//unselect the program from this context
		glUseProgram(0);
	}

	void ModelRenderer::Fog(float density)
	{
		fogColourID = glGetUniformLocation(programID, "fog_colour");
		fogDensityID = glGetUniformLocation(programID, "fog_density");

		GLCall(glUniform1f(fogDensityID, density));
		glm::vec3 fog_colour = glm::vec3(0.5f, 0.5f, 0.5f);
		GLCall(glUniform3fv(fogColourID, 1, glm::value_ptr(fog_colour)));
	}


	void ModelRenderer::Draw(Camera* cam, Model* model, Light* light)
	{
		float distance = glm::length(cam->getPos() - model->getPos());
		float billboardDistance = 200.0f;
		if (distance > billboardDistance && model->HasValidBillboard())
		{
			model->getBillboard()->setScaleX(20.0f);
			model->getBillboard()->setScaleY(20.0f);
			billboardRend->setPos(model->getPos());
			billboardRend->draw(model->getBillboard(), cam);
		}
		else
		{
			glEnable(GL_CULL_FACE);
			glm::mat4 tranformationMat = glm::mat4(1.0f);
			ManipulateVerticies(tranformationMat, model);
			//get teh view and projection matricies
			glm::mat4 viewMat = cam->getViewMatrix();
			glm::mat4 projectionMat = cam->getProjectionMatrix();

			//use program created from init
			GLCall(glUseProgram(programID));
			GLCall(glUniformMatrix4fv(transformUniformID, 1, GL_FALSE, value_ptr(tranformationMat)));
			GLCall(glUniformMatrix4fv(viewUniformID, 1, GL_FALSE, value_ptr(viewMat)));
			GLCall(glUniformMatrix4fv(projectionUniformID, 1, GL_FALSE, value_ptr(projectionMat)));

			glUniformMatrix4fv(glGetUniformLocation(programID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(light->getProjectionMatrix()));
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, light->GetShadowMap());
			glUniform1i(glGetUniformLocation(programID, "shadowMap"), 2);

			GLCall(glUniform3f(lightColour, light->getColour().x, light->getColour().y, light->getColour().z));
			GLCall(glUniform3f(ambient, light->getAmbient().x, light->getAmbient().y, light->getAmbient().z));
			GLCall(glUniform3f(lightPos,light->getPos().x, light->getPos().y, light->getPos().z));

			glUniform3f(viewPosID, cam->getPos().x, cam->getPos().y, cam->getPos().z);
			model->BindTexture();
			Fog(0.0060f);
			DrawWithVerticies(model->getVertices(), vertexPos3DLocation, vertexUVLocation, vertexNormal, model->getNumOfVertices());

			glDisable(GL_CULL_FACE);
		}
	}

	void ModelRenderer::Draw(Model* model, Light* light)
	{
		glEnable(GL_FRONT);
		glm::mat4 tranformationMat = glm::mat4(1.0f);
		ManipulateVerticies(tranformationMat, model);

		//use program created from init
		GLCall(glUseProgram(light->GetShadowProgram()));
		
		GLCall(glUniformMatrix4fv(glGetUniformLocation(light->GetShadowProgram(), "model"), 1, GL_FALSE, value_ptr(tranformationMat)));
		GLCall(glUniformMatrix4fv(light->GetGetLightLoc(), 1, GL_FALSE, glm::value_ptr(light->getProjectionMatrix())));

		DrawWithVerticies(model->getVertices(), vertexPos3DLocation, vertexUVLocation, vertexNormal, model->getNumOfVertices());
		glDisable(GL_FRONT);
	}

	void ModelRenderer::Draw(Camera* cam, Terrain* terrain, Light* light)
	{
		glEnable(GL_CULL_FACE);
		glm::mat4 tranformationMat = glm::mat4(1.0f);

		//get teh view and projection matricies
		glm::mat4 viewMat		= cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		//use program created from init
		GLCall(glUseProgram(programID));
		GLCall(glUniformMatrix4fv(transformUniformID, 1, GL_FALSE, value_ptr(tranformationMat)));
		GLCall(glUniformMatrix4fv(viewUniformID, 1, GL_FALSE, value_ptr(viewMat)));
		GLCall(glUniformMatrix4fv(projectionUniformID, 1, GL_FALSE, value_ptr(projectionMat)));
		
		glUniformMatrix4fv(glGetUniformLocation(programID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(light->getProjectionMatrix()));
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, light->GetShadowMap());
		glUniform1i(glGetUniformLocation(programID, "shadowMap"), 2);

		GLCall(glUniform3f(lightColour, light->getColour().x, light->getColour().y, light->getColour().z));
		GLCall(glUniform3f(ambient, light->getAmbient().x, light->getAmbient().y, light->getAmbient().z));
		GLCall(glUniform3f(lightPos, light->getPos().x, light->getPos().y, light->getPos().z));

		terrain->BindTexture(&programID);
		Fog(0.0055f);
		DrawWithIndecies(terrain->getVertices(), terrain->getIndices(), terrain->getIndexCount());
		glDisable(GL_CULL_FACE);
	}

	void ModelRenderer::Draw(Camera* cam, SkyDome* sky, Light* light)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glm::mat4 tranformationMat = glm::mat4(1.0f);
		//get teh view and projection matricies
		glm::mat4 viewMat		= cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		tranformationMat = glm::translate(tranformationMat, glm::vec3(cam->getPosX(), cam->getPosY(), cam->getPosZ()));
		//use program created from init
		GLCall(glUseProgram(programID));
		GLCall(glUniformMatrix4fv(transformUniformID, 1, GL_FALSE, value_ptr(tranformationMat)));
		GLCall(glUniformMatrix4fv(viewUniformID, 1, GL_FALSE, value_ptr(viewMat)));
		GLCall(glUniformMatrix4fv(projectionUniformID, 1, GL_FALSE, value_ptr(projectionMat)));

		GLCall(glUniform3f(lightColour, light->getColour().x, light->getColour().y, light->getColour().z));
		GLCall(glUniform3f(ambient, light->getAmbient().x, light->getAmbient().y, light->getAmbient().z));
		GLCall(glUniform3f(lightPos, light->getPos().x, light->getPos().y, light->getPos().z));

		sky->BindTexture(&programID);
		Fog(0.0055f);
		DrawWithIndecies(sky->getVertices(), sky->getIndices(), sky->getIndexCount());

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	//release the object allocated for the program and vertex buffer object
	void ModelRenderer::Destroy()
	{
		GLCall(glDeleteProgram(programID));
	}

}