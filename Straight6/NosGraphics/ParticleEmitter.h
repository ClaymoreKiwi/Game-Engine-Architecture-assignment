#pragma once
#include <vector>
#include <math.h>
#include <list>
#include "utils.h"
#include "BillboardRenderer.h"

namespace GE
{
	struct ParticleData 
	{
		glm::mat4 pTransform = glm::mat4(1);
		float fade = 100;
		float life = 10;
		float speed = 1;
	};

	class ParticleEmitter
	{
	public:
		ParticleEmitter(int emissionsRate, float lifetime, glm::vec3 beginPos_, const char* particle_);
			
		void Update(float DeltaTime, Camera* cam);
		void SetupParticleData();
		void Draw(Camera* cam);
		void UpdateParticles(float DeltaTime);
		~ParticleEmitter()
		{
		}

	private:
		int emissionsRate = 0;

		const float ParticleLife = 0;
		const char* particlePath;

		glm::vec3 pos = glm::vec3(0);

		std::vector<ParticleData> particleData;
		std::vector<glm::vec3> resetParticleData;
		std::unique_ptr<BillboardRenderer> render;
		Billboard* billboard = nullptr;

		GLuint ParticleDataBuffer;
	};

}

