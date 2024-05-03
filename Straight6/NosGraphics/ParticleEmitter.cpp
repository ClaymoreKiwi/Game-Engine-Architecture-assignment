#include "ParticleEmitter.h"
#include <math.h>

GE::ParticleEmitter::ParticleEmitter(int ParticleRate,float lifeTime, glm::vec3 beginPos_, const char* particle_)
	:emissionsRate(ParticleRate), ParticleLife(lifeTime), pos(beginPos_), particlePath(particle_)
{
	billboard = new Billboard(particle_, pos);
	render = std::make_unique<BillboardRenderer>(true);
	SetupParticleData();
}

void GE::ParticleEmitter::Update(float DeltaTime, Camera* cam)
{
	UpdateParticles(DeltaTime);
	Draw(cam);
}

void GE::ParticleEmitter::SetupParticleData()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> speed(0.1f, 0.5f);
	std::uniform_real_distribution<float> randX(pos.x - 5, pos.x + 5);
	std::uniform_real_distribution<float> randZ(pos.z - 5, pos.z + 5);
	std::uniform_real_distribution<float> randL(ParticleLife, ParticleLife + 10);

	for (int i = 0; i < emissionsRate; ++i) {
		float rand = speed(gen);
		float posX = randX(gen);
		float posZ = randZ(gen);
		float randLife = randL(gen);
		ParticleData pd;
		pd.pTransform = glm::translate(glm::mat4(1),glm::vec3(posX, pos.y, posZ));
		pd.pTransform = glm::scale(pd.pTransform, glm::vec3(10, 10, 1));
		pd.life = randLife;
		pd.fade = 1;
		pd.speed = rand;
		particleData.push_back(pd);
		resetParticleData.push_back(glm::vec3(posX, pos.y, posZ));
	}
	GLCall(glGenBuffers(1, &ParticleDataBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, ParticleDataBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, emissionsRate * sizeof(ParticleData), particleData.data(), GL_DYNAMIC_DRAW));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void GE::ParticleEmitter::Draw(Camera* cam)
{
	render->drawAsParticleV2(billboard, cam, ParticleDataBuffer);
}

void GE::ParticleEmitter::UpdateParticles(float DeltaTime)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> randX(5, 10); // Controlled speed variable
	float randScale = randX(gen);
	float scaleFactor = randScale;

	for (int i = 0; i < particleData.size(); ++i)
	{
		if (particleData[i].life <= 0)
		{
			// Reset position to initial position
			particleData[i].pTransform = glm::translate(glm::mat4(1), resetParticleData[i]);
			particleData[i].pTransform = glm::scale(particleData[i].pTransform, glm::vec3(10, 10, 1));
			particleData[i].life = 10;
			particleData[i].fade = 1;
		}

		// Update particle position
		particleData[i].life -= 0.05f;
		particleData[i].pTransform = glm::translate(particleData[i].pTransform, glm::vec3(0, particleData[i].speed /5, 0));

		// Update fade
		if (particleData[i].fade > 0)
			particleData[i].fade -= 0.01;
		else if (particleData[i].fade < 1)
		{
			particleData[i].fade = 0;
		}
	}

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, ParticleDataBuffer));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, particleData.size() * sizeof(ParticleData), particleData.data()));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
