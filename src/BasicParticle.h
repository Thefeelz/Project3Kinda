#pragma once
#include "Particle.h"
#include <ofMain.h>


class BasicParticle : public Particle
{

public:
    // Gets how much life the particle has left.  
    // When a particle's life is reduced to zero, it will be removed from the particle system.
    float getLife() const override;

    void setLife(float life);
    // Updates the particle for a specified time increment (dt).
    void update(float dt) override;

    float getBrightness() const;

    void setBrightness(float brightness);

    void updateBrightness(float dt);

    void setPosition(glm::vec3 pos);

    glm::vec3 getPosition() const;

    void setVelocity(glm::vec3 velocity);

    glm::vec3 getVelocity() const;

    void setColor(glm::vec4 color);

    glm::vec4 getColor() const;

private:
    float life;
    float brightness;
    int textureID;
    glm::vec3 pos;
    glm::vec3 velocity;
    glm::vec4 color;
};