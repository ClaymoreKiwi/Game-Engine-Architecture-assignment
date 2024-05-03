#include "Particle.h"

GE::Particle::Particle(glm::vec3 position, float speed, const char* texturePath, float lifeSpan)
	: Position(position), Velocity(speed/200), Life(lifeSpan)
{
	billboard = new Billboard(texturePath, position);
	billboardRend = std::make_unique<BillboardRenderer>();
	billboardRend->setPos(glm::vec3 (Position.x, Position.y, position.z));

}

void GE::Particle::Update(float DeltaTime, Camera* cam)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> randX(5, 10);//controlled speed variable
	float randScale = randX(gen);
	float scaleFactor = randScale /100;
	if (Life <= 0)
	{
		ResetParticle();
	}
	Life -= 0.05f;
	setScale(scaleFactor, scaleFactor);
	billboard->setY(billboard->getY() + Velocity.y);
	if (fade > 0)
		fade -= 0.5;
	else if (fade < 1)
	{
		fade = 0;
	}
	billboardRend->drawAsParticle(billboard, cam, fade);
}

void GE::Particle::ResetParticle()
{
	billboard->setScaleX(1);
	billboard->setScaleY(1);
	billboard->setY(Position.y);
	fade = 100;
	Life = 10;
}
