#include "ofApp.h"
#include "CameraMatrices.h"
#include "SimpleDrawNode.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	using namespace glm;
	ofDisableArbTex();
	ofEnableDepthTest();

	loadMesh();

	//Start the Scene Graph
	//Create the Root Node (This is non Drawing)
	sceneGraphRoot.childNodes.emplace_back(new SceneGraphNode{});

	//Set the most recent sceneGraph node equal to the cube body pointer
	cubeNode = sceneGraphRoot.childNodes.back();

	//Draw the Robits Body
	cubeNode->childNodes.emplace_back(new SimpleDrawNode(cubeMesh, shader));
	auto cubeMeshNode = cubeNode->childNodes.back();
	cubeMeshNode->localTransform = scale(vec3(0.75, 1.25, 0.5)); //Elongating the body for a more "body" like feel

	// Add non-drawing node as child of the body
	cubeNode->childNodes.emplace_back(new SceneGraphNode{});
	cubeNode->childNodes.back() // node just added, we are rotating her and moving her up a bit
		->localTransform = rotate(2.0f, vec3(1)) * translate(vec3(2, 0, 0)) * scale(vec3(0.8));
	cubeNode->childNodes.back() // node just added, we are adding a new draw node to it for the "face"
		->childNodes.emplace_back(new SimpleDrawNode(torusMesh, shader));
	auto faceMeshNode = cubeNode->childNodes.back(); 

	//Add non-drawing node as child of the "head"
	faceMeshNode->childNodes.emplace_back(new SceneGraphNode{});
	faceMeshNode->childNodes.back()
		->localTransform = translate(vec3(0, .75, 0)) * scale(vec3(1, 0.5 , 1));
	faceMeshNode->childNodes.back()
		->childNodes.emplace_back(new SimpleDrawNode(coneMesh, shader));
	
	//add non drawing node as child of cube node
	cubeMeshNode->childNodes.emplace_back(new SceneGraphNode{});
	cubeMeshNode->childNodes.back()
		->localTransform = translate(vec3(0, -1.75, 0)) * scale(vec3(0.8, 0.8, 0.8)); //Moving the node to the feet for the sphere spinny "wheel"
	cubeMeshNode->childNodes.back()// node just added, we are drawing the sphere 
		->childNodes.emplace_back(new SimpleDrawNode(sphereMesh, shader));
	wheelNode = cubeMeshNode->childNodes.back();

	//add non drawing node as child of cube node for the arm
	cubeMeshNode->childNodes.emplace_back(new SceneGraphNode{});
	cubeMeshNode->childNodes.back()
		->localTransform = translate(vec3(-1, 0.25, 0)) * scale(vec3(0.75));
	cubeMeshNode->childNodes.back()
		->childNodes.emplace_back(new SimpleDrawNode(cubeMesh, shader));

	//add non drawing node as child of cube node for the arm
	cubeMeshNode->childNodes.emplace_back(new SceneGraphNode{});
	cubeMeshNode->childNodes.back()
		->localTransform = translate(vec3(1, 0.25, 0)) * scale(vec3(0.75));
	cubeMeshNode->childNodes.back()
		->childNodes.emplace_back(new SimpleDrawNode(cubeMesh, shader));

	//PARTICLE SYSTEM HERE
	buildMeshFire(particleMeshFire, 2, 2, glm::vec3(1));
	buildMeshSmoke(particleMeshSmoke, 0.6, 0.8, glm::vec3(0, 0.1, 0));
	buildMeshFlame(particleMeshFlame, 0.3, 0.3, glm::vec3(0, -.1, 0));

	particleShaderFire.load("shaders/my.vert", "shaders/firstFrag.frag");
	particleShaderSmoke.load("shaders/my.vert", "shaders/firstFrag.frag");
	particleShaderFlame.load("shaders/my.vert", "shaders/firstFrag.frag");

	particleImgSmoke.load("textures/smoke_01.png");
	particleImgFire.load("textures/fire_02.png");
	particleImgFlame.load("textures/fire_01.png");
	
}

void ofApp::reloadShaders()
{
	shader.load("shaders/my.vert", "shaders/my.frag");
	needsReload = false;
}

void ofApp::updateCameraRotation(float dx, float dy)
{
	using namespace glm;

	cameraPitch -= dy;
	// -89 to +89 degrees, converted to radians
	cameraPitch = clamp(cameraPitch, radians(-89.0f), radians(89.0f));

	cameraHead -= dx;
}
void ofApp::updateJoint2Rotation(float dt)
{
	using namespace glm;
	mat3 currentRotation{ mat3(wheelNode->localTransform) };
	vec3 currentTranslation{ wheelNode->localTransform[3] };
	wheelNode->localTransform = translate(currentTranslation)
		* rotate(dt, vec3(1, 0, 0)) * mat4(currentRotation);
}
void ofApp::loadMesh()
{
	//Load all of the Mesh's
	sphereMesh.load("models/sphere.ply");
	sphereMesh.flatNormals();
	torusMesh.load("models/torus.ply");
	torusMesh.flatNormals();
	cylinderMesh.load("models/cylinder.ply");
	cylinderMesh.flatNormals();
	cubeMesh.load("models/cube.ply");
	cubeMesh.flatNormals();
	coneMesh.load("models/cone.ply");
	coneMesh.flatNormals();

	for (int i = 0; i < sphereMesh.getNumNormals(); i++)
	{
		sphereMesh.setNormal(i, -sphereMesh.getNormal(i));
	}
	for (int i = 0; i < torusMesh.getNumNormals(); i++)
	{
		torusMesh.setNormal(i, -torusMesh.getNormal(i));
	}
	for (int i = 0; i < cylinderMesh.getNumNormals(); i++)
	{
		cylinderMesh.setNormal(i, -cylinderMesh.getNormal(i));
	}
	for (int i = 0; i < cubeMesh.getNumNormals(); i++)
	{
		cubeMesh.setNormal(i, -cubeMesh.getNormal(i));
	}
	for (int i = 0; i < coneMesh.getNumNormals(); i++)
	{
		coneMesh.setNormal(i, -coneMesh.getNormal(i));
	}
}

void ofApp::buildMeshFire(ofMesh& mesh, float w, float h, glm::vec3 pos)
{
	using namespace glm;
	float verts[] = { -w + pos.x, -h + pos.y, pos.z,
					 -w + pos.x, h + pos.y, pos.z,
					 w + pos.x, h + pos.y, pos.z,
					 w + pos.x, -h + pos.y, pos.z, };
	float uvs[] = { 0,0,0,1,1,1,1,0 };
	for (int i = 0; i < 4; i++)
	{
		int idx = i * 3;
		int uvIdx = i * 2;

		mesh.addVertex(vec3(verts[idx], verts[idx + 1], verts[idx + 2]));
		mesh.addTexCoord(vec2(uvs[uvIdx], uvs[uvIdx + 1]));
	}

	ofIndexType indices[6] = { 0, 1, 2, 2, 3, 0 };
	mesh.addIndices(indices, 6);
}

void ofApp::buildMeshSmoke(ofMesh& mesh, float w, float h, glm::vec3 pos)
{
	using namespace glm;
	float verts[] = { -w + pos.x, -h + pos.y, pos.z,
					 -w + pos.x, h + pos.y, pos.z,
					 w + pos.x, h + pos.y, pos.z,
					 w + pos.x, -h + pos.y, pos.z, };
	float uvs[] = { 0,0,0,1,1,1,1,0 };
	for (int i = 0; i < 4; i++)
	{
		int idx = i * 3;
		int uvIdx = i * 2;

		mesh.addVertex(vec3(verts[idx], verts[idx + 1], verts[idx + 2]));
		mesh.addTexCoord(vec2(uvs[uvIdx], uvs[uvIdx + 1]));
	}

	ofIndexType indices[6] = { 0, 1, 2, 2, 3, 0 };
	mesh.addIndices(indices, 6);
}

void ofApp::buildMeshFlame(ofMesh& mesh, float w, float h, glm::vec3 pos)
{
	using namespace glm;
	float verts[] = { -w + pos.x, -h + pos.y, pos.z,
					 -w + pos.x, h + pos.y, pos.z,
					 w + pos.x, h + pos.y, pos.z,
					 w + pos.x, -h + pos.y, pos.z, };
	float uvs[] = { 0,0,0,1,1,1,1,0 };
	for (int i = 0; i < 4; i++)
	{
		int idx = i * 3;
		int uvIdx = i * 2;

		mesh.addVertex(vec3(verts[idx], verts[idx + 1], verts[idx + 2]));
		mesh.addTexCoord(vec2(uvs[uvIdx], uvs[uvIdx + 1]));
	}

	ofIndexType indices[6] = { 0, 1, 2, 2, 3, 0 };
	mesh.addIndices(indices, 6);
}


//--------------------------------------------------------------
void ofApp::update()
{
	if (needsReload)
	{
		reloadShaders();
	}

	float dt{ static_cast<float>(ofGetLastFrameTime()) };

	// Update position using velocity and dt.
	using namespace glm;
	camera.position += mat3(rotate(cameraHead, vec3(0, 1, 0)))
		* velocity * dt;
	camera.rotation = rotate(cameraHead, vec3(0, 1, 0)) *
		rotate(cameraPitch, vec3(1, 0, 0));
	updateJoint2Rotation(dt);

	//PARTICLE SYSTEM UPDATE
	float speed = 0.2f * static_cast<float>(ofGetLastFrameTime());
	particlePos += glm::vec3(0, speed, 0);

	particleSystemFire.update(ofGetLastFrameTime());
	particleSystemSmoke.update(ofGetLastFrameTime());
	particleSystemFlame.update(ofGetLastFrameTime());
	
}
glm::mat4 screenAlignedBillboard(glm::mat4 modelView)
{
	using namespace glm;
	vec3 camSpacePos{ modelView * vec4(0, 0, 0, 1) };
	return translate(camSpacePos);
}

glm::mat4 viewpointOrientedBillboard(glm::mat4 modelView, glm::vec3 up)
{
	using namespace glm;
	vec3 camSpacePos{ modelView * vec4(0, 0, 0, 1) };
	vec3 n{ -normalize(camSpacePos) };
	vec3 r{ normalize(cross(up, n)) };
	vec3 u{ normalize(cross(n, r)) };
	return mat4(vec4(r, 0), vec4(u, 0), vec4(n, 0), vec4(camSpacePos, 1))
		* scale(vec3(sqrt(determinant(mat3(modelView)))));
}
//--------------------------------------------------------------
void ofApp::draw()
{
	using namespace glm;

	float width{ static_cast<float>(ofGetViewportWidth()) };
	float height{ static_cast<float>(ofGetViewportHeight()) };
	float aspect = width / height;

	// Calculate the view and projection matrices for the camera.
	CameraMatrices camMatrices{ camera, aspect, 0.01f, 10.0f, };

	shader.begin(); // make shader active just to set uniform values
	shader.setUniform3f("lightDir", normalize(vec3(1, 1, 1)));
	shader.setUniform3f("lightColor", vec3(1, 1, 1)); // white light
	shader.setUniform3f("meshColor", vec3(1, .6, .3)); // red material
	shader.setUniform3f("ambientColor", vec3(0.1, 0.1, 0.1));
	shader.end();

	sceneGraphRoot.drawSceneGraph(camMatrices);

	/*particleShaderFire.begin();
	particleShaderFire.setUniformMatrix4f("mvp",
		camMatrices.getProj() * 
		viewpointOrientedBillboard(camMatrices.getView() * translate(vec3(0, 0, 5)), vec3(0,1,0)));
	particleShaderFire.setUniformTexture("fireTex", particleImgFire, 0);
	particleMeshFire.draw();
	particleShaderFire.end();*/
	//PARTICLE SYSTEM DRAW
	particleShaderSmoke.begin();
	for (const BasicParticle& p : particleSystemSmoke)
	{

		p.getLife();
		particleShaderSmoke.setUniform1f("brightness", p.getBrightness());
		particleShaderSmoke.setUniform4f("color", glm::vec4(1, 1, 1, 0.5));
		particleShaderSmoke.setUniform3f("velocity", p.getPosition());
		particleShaderSmoke.setUniformTexture("fireTex", particleImgSmoke, 0);
		particleMeshSmoke.draw();
	}
	particleShaderSmoke.end();

	particleShaderFire.begin();

	for (const BasicParticle& p : particleSystemFire)
	{

		p.getLife();

		particleShaderFire.setUniform1f("brightness", p.getBrightness());
		particleShaderFire.setUniform4f("color", glm::vec4(1, 0.5, 0, 1));
		particleShaderFire.setUniform3f("velocity", p.getPosition());
		particleShaderFire.setUniformMatrix4f("mvp",
			camMatrices.getProj() *
			viewpointOrientedBillboard(camMatrices.getView() * translate(vec3(0, 0, 5)), vec3(0, 1, 0)));
		particleShaderFire.setUniformTexture("fireTex", particleImgFire, 0);
		particleMeshFire.draw();
	}
	particleShaderFire.end();



	particleShaderFlame.begin();
	for (const BasicParticle& p : particleSystemFlame)
	{

		p.getLife();
		particleShaderFlame.setUniform1f("brightness", p.getBrightness());
		particleShaderFlame.setUniform4f("color", glm::vec4(1, 1, 0, 1));
		particleShaderFlame.setUniform3f("velocity", p.getPosition());
		particleShaderFlame.setUniformTexture("fireTex", particleImgFlame, 0);
		particleMeshFlame.draw();
	}

	particleShaderFlame.end();
	
	/*particleShaderFire.begin();
	particleShaderFire.setUniform1f("brightness", 1);
	particleShaderFire.setUniform4f("color", glm::vec4(1, 0.5, 0, 1));
	particleShaderFire.setUniform3f("velocity", glm::vec3(0));
	particleShaderFire.setUniformTexture("fireTex", particleImgFire, 0);
	particleMeshFire.draw();
	particleShaderFire.end();*/
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	using namespace glm;

	if (key == OF_KEY_UP || key == 'w')
	{
		// Forward motion
		// Converting from camera space velocity to world space velocity
		velocity.z = -1;
	}
	else if (key == OF_KEY_DOWN || key == 's')
	{
		// Backwards motion
		// Converting from camera space velocity to world space velocity
		velocity.z = 1;
	}
	else if (key == OF_KEY_LEFT || key == 'a')
	{
		// Forward motion
		// Converting from camera space velocity to world space velocity
		velocity.x = -1;
	}
	else if (key == OF_KEY_RIGHT || key == 'd')
	{
		// Backwards motion
		// Converting from camera space velocity to world space velocity
		velocity.x = 1;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	if (key == '`')
	{
		// Reload shaders
		needsReload = true;
	}
	else if (key == OF_KEY_LEFT || key == 'a' || key == OF_KEY_RIGHT || key == 'd')
	{
		// Reset velocity when a key is released
		velocity.x = 0;
	}
	else if (key == OF_KEY_UP || key == 'w' || key == OF_KEY_DOWN || key == 's')
	{
		// Reset velocity when a key is released
		velocity.z = 0;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
	if (prevX != 0 && prevY != 0)
	{
		// Previous mouse position has been initialized.
		// Calculate dx and dy
		int dx = x - prevX;
		int dy = y - prevY;

		updateCameraRotation(mouseSensitivity * dx, mouseSensitivity * dy);
	}

	prevX = x;
	prevY = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
