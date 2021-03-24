#include "BasicParticleGenerator.h"
#include "ofApp.h"

void BasicParticleGenerator::respawn(BasicParticle& particle) const
{
	int r1 = rand() % 1001;
	int r2 = rand() % 1001;
	int signX = rand() % 2;
	int signY = rand() % 2;
	if (signX == 0)
		signX = 1;
	else
		signX = -1;
	if (signY == 0)
		signY = 1;
	else
		signY = -1;
	float x = r1 / 10000.0;
	float y = r2 / 10000.0;
	particle.setLife(1.0f);
	particle.setBrightness(0.2f);
	particle.setPosition(glm::vec3(0, 0, 0));
	particle.setVelocity(glm::vec3(x * signX, y * signY, 0));
	//randomize things position/color/texture
}

