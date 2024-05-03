#pragma once
#include <random>
#include "utils.h"
#include "Billboard.h"
#include "BillboardRenderer.h"
#include "Texture.h"

namespace GE
{
    class Particle
    {
    public:
        Particle(glm::vec3 position, float speed, const char* texturePath, float lifeSpan);
        void Update(float DeltaTime, Camera* cam);
        void ResetParticle();
        ~Particle()
        {
            delete billboard;
        }
        //accessors
        inline Billboard* getBillboard()
        {
            return billboard;
        }
        inline glm::vec3 getPos()
        {
            return Position;
        }
        inline glm::vec3 getVelocity()
        {
            return Position;
        }
        inline float getLifeTime() const
        {
            return Life;
        }

        //modifiers
        inline void setScale(float x, float y)
        {
            billboard->setScaleX(billboard->getScaleX() + x);
            billboard->setScaleY(billboard->getScaleY() + y);
        }
        inline void setPosition(float x, float y, float z)
        {
            billboard->setPos(billboard->getX() + x,
                              billboard->getY() + y,
                              billboard->getY() + z);
        }


    private:
        float Life = 0.0f;
        float fade = 100.0f;
        float lastUpdate = 0;

        glm::vec3 Position = glm::vec3(1);
        glm::vec3 Velocity = glm::vec3(1);

        Billboard* billboard = nullptr;
        std::unique_ptr<BillboardRenderer> billboardRend = nullptr;
    };
}

