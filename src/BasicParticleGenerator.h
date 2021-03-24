#pragma once
#include "Particle.h"
#include "BasicParticle.h"

class BasicParticleGenerator : public ParticleGenerator<BasicParticle>
{
public:
    /*BasicParticleGenerator(ofImage& texture) :
        texture{ texture }
    {}*/
    
    // Called when a new particle needs to be generated. When the function returns, the particle 
    // referenced by the parameter should be reinitialied as a new, random particle.
    void respawn(BasicParticle& particle) const override;

private:
    //ofImage& texture;

};