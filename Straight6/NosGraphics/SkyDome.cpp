#include "SkyDome.h"
#include "Model.h"
#include <vector>
#include <cmath>

GE::SkyDome::SkyDome(const char* textPath, int resolution, int r)
{
	// 1. Define the dome in terms of resolution
	// and size (radius).  Note, some of these
	// constants would be better as parameters
	// to the skydome constructor to configure
	// skydome at runtime, for example

	// Used to determine extent of dome in terms
	// of 90 degrees (radians)
	const float PIDIV2 = M_PI / 2.0f;

	// Define extents of skydome in terms of numbers
	// of vertices horizontally and vertically
	int horizResolution = resolution;
	int vertResolution = resolution;

	// Radius for sphere
	int radius = r;

	// Store the generated vertices
	std::vector<Vertex> vertices;

	// Angle between each horizontal vertex in radians
	// Angles in radians are expressed in terms of PI
	// This code makes use of math.h 
	float azimuthStep = 2.0f * M_PI / horizResolution;

	// Texture extent.  This would be changed depending
	// on the skydome texture.  For example, some textures
	// are meant to be wrapped on a sphere, not a dome.
	// If you use a sphere percentage of 1.0, then the 
	// bottom part of the texture appears at the based 
	// of the dome.  In this case, change texturePercentage
	// to 0.5f so v ranges from 0 to 0.5 and only
	// top half of texture is shown
	float texturePercentage = 1.0f;

	// Determines the extent of the dome in terms of a
	// percentage of PIDIV2.  1.0 means 100% of PIDIV2
	// or 90 degrees.  2.0 means 200% of PIDIV2 which
	// results in a sphere
	float spherePercentage = 2.0f;

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

	// 2. Determine the vertices horizontally for each elevation
	// from the top to bottom.

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
			 // Add new vertex
			 vertices.push_back(Vertex(x, y, z, u, v * texturePercentage));

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

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		indexCount = indices.size();
		texture = new Texture(textPath);
	}

	void GE::SkyDome::BindTexture(GLuint* PIDref)
	{
		GLuint samplerID = glGetUniformLocation(*PIDref, "sampler");
		glUniform1i(samplerID, 0);
		glBindTexture(GL_TEXTURE_2D, texture->getTexture());
	}
