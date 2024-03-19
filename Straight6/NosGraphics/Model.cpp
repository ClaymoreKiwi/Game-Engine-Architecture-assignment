#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Model.h"

GE::Model::Model(const GLuint* PID, const char* modelPath, const char* texturePath)
	:PIDref(PID)
{
	texture = std::make_unique<Texture>(texturePath);
	BindTexture();
	LoadFromFile(modelPath, texturePath);
}

void GE::Model::BindTexture()
{
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLuint textID = glGetUniformLocation(*PIDref, "sampler");
	GLCall(glUniform1i(textID, 0));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture->getTexture()));
}

bool GE::Model::LoadFromFile(const char* fileName, const char* shaderTex)
{
	std::vector<Vertex> loadedVerticies;
	std::vector<GLuint> indices;

	Assimp::Importer imp;

	const aiScene* pScene = imp.ReadFile(fileName, aiProcessPreset_TargetRealtime_Quality | 
		aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices);

	if (!pScene)
	{
		std::cerr << "Unable to read file " << fileName << " " << imp.GetErrorString() << std::endl;
		return false;
	}
	//get vertices from the model and put them in a temp vector
	for (GLuint meshIDx = 0; meshIDx < pScene->mNumMeshes; meshIDx++)
	{
		const aiMesh* mesh = pScene->mMeshes[meshIDx];

		for (GLuint faceIDx = 0; faceIDx < mesh->mNumFaces; faceIDx++)
		{
			const aiFace& face = mesh->mFaces[faceIDx];
			//extract a vertex from the main array
			//there are 3 to represent a triangulated face

			for (int vertIDx = 0; vertIDx < 3; vertIDx++)
			{
				//extract the position and texture co-ord on its indext number
				//and not directly from the mesh array
				const aiVector3D* pos = &mesh->mVertices[face.mIndices[vertIDx]];
				const aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[vertIDx]];
				//create a new object in the shape array based on the extracted vertices
				//shape will be used i nthe vertex buffer
				loadedVerticies.push_back(Vertex(pos->x, pos->y, pos->z, uv.x, uv.y));
			}
		}
	}

	numVertices = static_cast<int>(loadedVerticies.size());

	GLCall(glGenBuffers(1, &vbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), loadedVerticies.data(), GL_STATIC_DRAW));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	return true;
}
