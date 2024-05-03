#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <cstring>
#include "Model.h"

GE::Model::Model(const GLuint* PID, const char* modelPath, const char* texturePath, const char* billboardTexture, glm::vec3& pos_)
	:PIDref(PID), pos(pos_)
{
	if (strcmp(billboardTexture, "../models/billboards/replace.png") == 0) {
		hasValidBillboard = false;
	}
	texture = std::make_unique<Texture>(texturePath);
	billboard = new Billboard(billboardTexture, pos);
	BindTexture();
	//LoadFromFile(modelPath, texturePath);
	LoadFromFileV2(modelPath);
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
				const aiVector3D* normals = &mesh->mNormals[face.mIndices[vertIDx]];
				//create a new object in the shape array based on the extracted vertices
				//shape will be used i nthe vertex buffer
				loadedVerticies.push_back(Vertex(pos->x, pos->y, pos->z, uv.x, uv.y, normals->x,normals->y, normals->z));
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

bool GE::Model::LoadFromFileV2(const char* fileName)
{
	//vertex
	std::vector<glm::vec3> v_pos;
	std::vector<glm::vec2> tx_coord;
	std::vector<glm::vec3> v_normal;

	//faces
	std::vector<GLint> vertexIndices;
	std::vector<GLint> vertex_texCoordIndices;
	std::vector<GLint> vertexNormalIndices;

	//vertexArray - use loaded verticies
	std::vector<Vertex> loadedVerticies;

	std::stringstream ss;
	std::ifstream infile(fileName);
	std::string line = "";
	std::string prefix = "";

	glm::vec3 Temp_vec3;
	glm::vec2 Temp_vec2;
	GLint temp_glint = 0;

	//check if open
	if (!infile.is_open())
	{
		std::cerr << "Unable to read file " << fileName << std::endl;
	}

	//read one lile
	while (std::getline(infile, line))
	{
		ss.clear();
		ss.str(line);
		ss >> prefix;
		if (prefix == "v")
		{
			ss >> Temp_vec3.x >> Temp_vec3.y >> Temp_vec3.z;
			v_pos.push_back(Temp_vec3);
		}
		else if (prefix == "vt")
		{
			ss >> Temp_vec2.x >> Temp_vec2.y;
			tx_coord.push_back(Temp_vec2);
		}
		else if (prefix == "vn")
		{
			ss >> Temp_vec3.x >> Temp_vec3.y >> Temp_vec3.z;
			v_normal.push_back(Temp_vec3);
		}
		else if (prefix == "f")
		{
			int count = 0;
			while (ss >> temp_glint)
			{
				//pushing indicied into arrays
				if (count == 0)
					vertexIndices.push_back(temp_glint);
				else if (count == 1)
					vertex_texCoordIndices.push_back(temp_glint);
				else if (count == 2)
					vertexNormalIndices.push_back(temp_glint);
				//character checking
				if (ss.peek() == '/')
				{
					ss.ignore(1, '/');
					++count;
				}
				else if (ss.peek() == ' ')
				{
					ss.ignore(1, ' ');
					++count;
				}
				//reset counter
				if (count > 2)
					count = 0;
			}
		}
	}
	//build vertex & vertex array
	loadedVerticies.resize(vertexIndices.size(), Vertex());

	//setup vetex data
	for (size_t i = 0; i < loadedVerticies.size(); ++i)
	{
		//v data
		loadedVerticies[i].x = v_pos[vertexIndices[i] - 1].x;
		loadedVerticies[i].y = v_pos[vertexIndices[i] - 1].y;
		loadedVerticies[i].z = v_pos[vertexIndices[i] - 1].z;
		//vt data
		loadedVerticies[i].u = tx_coord[vertex_texCoordIndices[i] - 1].x;
		loadedVerticies[i].v = 1.0 -tx_coord[vertex_texCoordIndices[i] - 1].y;
		//vn data
		loadedVerticies[i].nx = v_normal[vertexNormalIndices[i]-1].x;
		loadedVerticies[i].ny = v_normal[vertexNormalIndices[i]-1].y;
		loadedVerticies[i].nz = v_normal[vertexNormalIndices[i]-1].z;
	}

	numVertices = (int)loadedVerticies.size();

	GLCall(glGenBuffers(1, &vbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), loadedVerticies.data(), GL_STATIC_DRAW));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	return true;
}
