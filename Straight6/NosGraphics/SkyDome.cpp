#include "SkyDome.h"
#include "Model.h"
#include <vector>
#include <cmath>

GE::SkyDome::SkyDome(const char* textPath, int resolution, int r)
{
	// Used to determine extent of dome in terms
	// of 90 degrees (radians)
	const float PIDIV2 = M_PI / 2.0f;

	// Define extents of skydome in terms of numbers
	// of vertices horizontally and vertically
	int horizResolution = resolution;
	int vertResolution = resolution;

	int radius = r;
	std::vector<Vertex> vertices;

	// Angle between each horizontal vertex in radians
	float azimuthStep = 2.0f * M_PI / horizResolution;

	// Texture extent. range in % that represents the amount of texture shown
	float texturePercentage = 0.5f;

	//% of sphere shown 2 being a whole sphere and anything below being a portion
	float spherePercentage = 1.0f;

	// Starting point for vertices.  Vertices start from top
	float elevation = PIDIV2;

	// Calculate the vertical step between vertices
	// based 90 degree * sphere percentage
	float elevationStep = spherePercentage * PIDIV2/ vertResolution;

	// Steps horizontally and vertically across uv range 0 to 1
	float uStep = 1.0f / horizResolution;
	float vStep = 1.0f / vertResolution;

	// Starting point for v texture co-ord.  Bottom of texture
	float v = 0.0f;

	 for (int vert = 0; vert < vertResolution + 1;vert++)
	 {
		// Calculate horizontal length for elevation, 0 at top to radius at 0 degrees
		 float length = radius * cos(elevation);

		// Calculate y position
		 float y = radius * sin(elevation);

		 float u = 0.0f;
		 float angle = 0;
		// Calculate horizontal vertices for elevation
		 for (int horiz = 0; horiz < horizResolution + 1; horiz++)
		 {
			 // Calculate xz position of vertex based on length
			 // and angle horizontally
			 float x = length * sin(angle);
			 float z = length * cos(angle);

			 // Calculate normals
			 glm::vec3 normal(x, y, z);
			 normal = glm::normalize(normal);
			 // Add new vertex
			 vertices.push_back(Vertex(x, y, z, u, v * texturePercentage, -normal.x, -normal.y, -normal.z));

			 angle += azimuthStep;
			 u += uStep;
		 }
		 elevation -= elevationStep;
		 v += vStep;
	}

		// 3. Determine the indices
	 std::vector<int> indices;
	 for (int vert = 0; vert < vertResolution; vert++)
	 {
		 int offset = vert * (horizResolution + 1);
		 for (int horiz = 0; horiz < horizResolution; horiz++)
		 {
				// Left triangle of quad
			 indices.push_back(offset + horiz);
			 indices.push_back(offset + horiz + 1);
			 indices.push_back(offset + (horizResolution + 1) + (horiz + 1));
				// Right triangle
			 indices.push_back(offset + (horizResolution + 1) + (horiz + 1));
			 indices.push_back(offset + horizResolution + horiz + 1);
			 indices.push_back(offset + horiz);
		 }
	 }

	//buffers for drawing
	GLCall(glGenBuffers(1, &vbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	indexCount = indices.size();
	texture = std::make_unique<Texture>(textPath);
}

	void GE::SkyDome::BindTexture(GLuint* PIDref)
	{
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLuint samplerID = glGetUniformLocation(*PIDref, "sampler");
		GLCall(glUniform1i(samplerID, 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture.get()->getTexture()));
	}
