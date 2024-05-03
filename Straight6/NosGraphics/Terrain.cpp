#include "Terrain.h"
#include "Model.h"
#include <SDL_image.h>
#include <vector>

GE::Terrain::Terrain(const char* texturePath, const char* heightMapPath, float HightScale, float Scale)
{
	SDL_Surface* heightMap = IMG_Load(heightMapPath);

	int height = heightMap->h;
	int width = heightMap->w;

	unsigned char* imageData = (unsigned char*)heightMap->pixels;

	int channels = heightMap->format->BytesPerPixel;

	float heightScaling = HightScale;
	float Scaling = Scale;

	std::vector<Vertex> verticies;
	std::vector<GLuint> indices;

	// Size of terrain is based on heightmap resolution width and height
	float x, y, z, u, v;
	for (int row = 0; row < height; row++)
	{
		// For each vertex in a row
		for (int column = 0; column < width; column++)
		{
			// Find the red pixel for the vertex.  (row * width) + column works out
			// the vertex and * channels finds the byte corresponding to red for the
			// vertex.
			unsigned char redPixel = imageData[((row * width) + column) * channels];

			// Calculate the vertex x, y, z based on column and row
			x = (float)column / (float)(width - 1) * Scaling;
			z = (float)row / (float)(height - 1) * Scaling;
			y = redPixel / 255.0f * heightScaling;

			// Calculate the uv based where the vertex is within the terrain in terms of its column and row
			u = (float)column / (float)(width - 1);
			v = (float)(height - row) / (float)(height - 1);
			// Add the vertex to the vector.  Vector will be used
			// to create the vertex buffer
			//calculate normals
			glm::vec3 left = glm::vec3(x - Scaling / (float)(width - 1), redPixel / 255.0f * heightScaling, z);
			glm::vec3 right = glm::vec3(x + Scaling / (float)(width - 1), redPixel / 255.0f * heightScaling, z);
			glm::vec3 up = glm::vec3(x, redPixel / 255.0f * heightScaling, z - Scaling / (float)(height - 1));
			glm::vec3 down = glm::vec3(x, redPixel / 255.0f * heightScaling, z + Scaling / (float)(height - 1));

			glm::vec3 normal = glm::normalize(glm::cross(left - right, up - down));

			verticies.push_back(Vertex(x, y, z, u, v, -normal.x,-normal.y, -normal.z));
		}
	}

	unsigned int c, n;
	for (int row = 0; row < height - 1; row++)
	{
		for (int column = 0; column < width - 1; column++)
		{
			c = row * width + column;
			n = (row + 1) * width + column;

			indices.push_back(c);
			indices.push_back(n);
			indices.push_back(c +1);
			indices.push_back(c+1);
			indices.push_back(n);
			indices.push_back(n+1);
 		}
	}

	//Create the vertex buffer
	GLCall(glGenBuffers(1, &vbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), verticies.data(), GL_STATIC_DRAW));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	//create index buffer
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	indexCount = (unsigned int)indices.size();

	SDL_FreeSurface(heightMap);
	texture = std::make_unique<GE::Texture>(texturePath);
}

void GE::Terrain::BindTexture(const GLuint* PIDref)
{
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLuint samplerID = glGetUniformLocation(*PIDref, "sampler");
	GLCall(glUniform1i(samplerID, 0));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture.get()->getTexture()));
}


