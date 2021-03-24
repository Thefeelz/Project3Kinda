#pragma once

#include "ofMain.h"
#include "Camera.h"
#include "SceneGraphNode.h"
#include "BasicParticleGenerator.h"
#include "ParticleSystem.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void loadMesh();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

private:
	//All the Meshes for the scene
	ofMesh cylinderMesh, torusMesh, cubeMesh, coneMesh, sphereMesh;

	//Shaders
	ofShader shader;
		
	// Do shaders need to be reloaded?
	bool needsReload{ true };

	// Load the shaders for this app
	void reloadShaders();

	// Keep track of previous mouse position
	int prevX{ 0 }, prevY{ 0 };

	// Allows us to configure how much moving the mouse affects the rotation
	float mouseSensitivity{ 0.01f };

	// Euler transformation
	float cameraHead{ 0 };
	float cameraPitch{ 0 };

	// Camera position
	Camera camera{ glm::vec3(1, 1, 2) };
	glm::vec3 velocity{};

	// Root node of the scene graph
	SceneGraphNode sceneGraphRoot{};
	std::shared_ptr<SceneGraphNode> torusNode{};
	std::shared_ptr<SceneGraphNode> cubeNode{};
	std::shared_ptr<SceneGraphNode> jointNode{};
	std::shared_ptr<SceneGraphNode> wheelNode{};

	// Called to update rotation of the camera from mouse movement
	void updateCameraRotation(float dx, float dy);
	void updateJoint2Rotation(float dt);

	
	//PARTICLE GENERATOR START
	
	BasicParticleGenerator particleGeneratorFire{ /*particleImgFire*/ };
	BasicParticleGenerator particleGeneratorSmoke{ /*particleImgSmoke*/ };  //x has to be at least twice as big as y
															   //42 has to be at least twice as big as 20
	BasicParticleGenerator particleGeneratorFlame{ /*particleImgFlame*/ };
	
	ParticleSystem<BasicParticle> particleSystemFire{ particleGeneratorFire, 42, 20.0f };
	ParticleSystem<BasicParticle> particleSystemSmoke{ particleGeneratorSmoke, 42, 20.0f };
	ParticleSystem<BasicParticle> particleSystemFlame{ particleGeneratorFlame, 42, 20.0f };
	
	ofMesh particleMeshFire;
	ofMesh particleMeshSmoke;
	ofMesh particleMeshFlame;
	
	ofShader particleShaderFire;
	ofShader particleShaderSmoke;
	ofShader particleShaderFlame;

	ofImage particleImgFire;
	ofImage particleImgSmoke;
	ofImage particleImgFlame;

	glm::vec3 particlePos{};
	
	void buildMeshFire(ofMesh& mesh, float w, float h, glm::vec3 pos);
	void buildMeshSmoke(ofMesh& mesh, float w, float h, glm::vec3 pos);
	void buildMeshFlame(ofMesh& mesh, float w, float h, glm::vec3 pos);	
};
