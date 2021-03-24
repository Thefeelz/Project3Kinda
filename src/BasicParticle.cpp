#include "BasicParticle.h"
#include "ofApp.h"
float BasicParticle::getLife() const
{
    return life;
}

void BasicParticle::setLife(float life)
{
    this->life = life;
}

void BasicParticle::update(float dt)
{
    //dt is OF get last frame ( not framerate dependant)
    //Life lasts 2 seconds here
    life -= dt * 0.5;
    pos += velocity * dt;
    updateBrightness(dt);
    //Update Position
    //Update Velocity
}

float BasicParticle::getBrightness() const
{
    return this->brightness;
}

void BasicParticle::setBrightness(float brightness)
{
    this->brightness = brightness;
}

void BasicParticle::updateBrightness(float dt)
{
    if (this->life > 0.8)
    {
        this->brightness += (this->brightness * dt) * 5;
    }
    else
    {
        this->brightness -= (this->brightness * dt);
    }
}


void BasicParticle::setPosition(glm::vec3 pos)
{
    this->pos = pos;
}

glm::vec3 BasicParticle::getPosition() const
{
    return this->pos;
}

void BasicParticle::setVelocity(glm::vec3 velocity)
{
    this->velocity = velocity;
}

glm::vec3 BasicParticle::getVelocity() const
{
    return this->velocity;
}

void BasicParticle::setColor(glm::vec4 color)
{
    this->color = color;
}

glm::vec4 BasicParticle::getColor() const
{
    return this->color;
}

