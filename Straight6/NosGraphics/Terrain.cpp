#include "Terrain.h"
#include "Model.h"
#include <SDL_image.h>
#include <vector>

GE::Terrain::Terrain(const char* texturePath, const char* heightMapPath, float HightScale, float Scale)
{
	SDL_Surface* heightMap = IMG_Load(heightMapPath);

	int height = heightMap->h;
	int width = heightMap->w;

	// Pixel colour defines the Y of a vertex
	unsigned char* imageData = (unsigned char*)heightMap->pixels;

	int channels = heightMap->format->BytesPerPixel;

	// 5. Define height and size scales of the terrain.
	float heightScaling = HightScale;
	float Scaling = Scale;

	// 6. Vectors for storing the vertices and indices (of vertices)
	std::vector<Vertex> verticies;
	std::vector<GLuint> indices;

	// 7. These loops create the vertices for the terrain
	// Size of terrain is based on heightmap resolution width and height
	float x, y, z, u, v;
	for (int row = 0; row < height; row++)
	{
		// For each vertex in a row
		for (int column = 0; column < width; column++)
		{
			// Find the red pixel for the vertex.  (row * width) + column works out
			// the vertex and * channels finds the byte corresponding to red for the
			// vertex.  A pixel may be made up of more than one byte for example
			// each channel could a byte so a pixel colour would be three bytes
			unsigned char redPixel = imageData[((row * width) + column) * channels];

			// Calculate the vertex x, y, z based on column and row
			// (x and z value), y is determined from the amount of red
			// no red means height would be 0, all red means height would
			// be maximum
			// Note x,y,z are scaled valuess
			x = (float)column / (float)(width - 1) * Scaling;
			z = (float)row / (float)(height - 1) * Scaling;
			y = redPixel / 255.0f * heightScaling;

			// Calculate the uv based where the vertex is within the 
			// terrain in terms of its column and row
			// Remember uv values are in the range of 0 to 1
			u = (float)column / (float)(width - 1);
			v = (float)(height - row) / (float)(height - 1);
			// Add the vertex to the vector.  Vector will be used
			// to create the vertex buffer
			verticies.push_back(Vertex(x, y, z, u, v));
		}
	}

// 8. These loops create the indices for the index buffer
// See slides 16 and 17 in the lecture notes
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

// 9 .Create the vertex buffer.  Same as creating vertex buffer for model
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), verticies.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

// 10. Create the index buffer.  Same way as creating a vertex buffer except
// type is GL_ELEMENT_ARRAY_BUFFER and use the indices vector

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
// 11. Store the number of indices as need this for rendering
// the terrain
	indexCount = (unsigned int)indices.size();

// 12. Tidy up
	SDL_FreeSurface(heightMap);

	texture = new GE::Texture(texturePath);
}

void GE::Terrain::BindTexture(const GLuint* PIDref)
{
	GLuint samplerID = glGetUniformLocation(*PIDref, "sampler");
	glUniform1i(samplerID, 0);
	glBindTexture(GL_TEXTURE_2D, texture->getTexture());
}


