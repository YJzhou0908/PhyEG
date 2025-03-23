#include "Render/basic/common.h"
#include "Render/core/attribute.h"
#include "Render/core/geometry.h"
#include "Render/core/object3D.h"
#include "Render/objects/mesh.h"
#include "Render/scene/scene.h"
#include "Render/camera/perspectiveCamera.h"
#include "Render/camera/orthographicCamera.h"
#include "Render/cameraControl/gameCameraControl.h"
#include "Render/render/renderer.h"
#include "Render/material/meshBasicMaterial.h"
#include "Render/material/meshPhongMaterial.h"
#include "Render/material/depthMaterial.h"
#include "Render/loader/textureLoader.h"
#include "Render/loader/cubeTextureLoader.h"
#include "Render/geometry/boxGeometry.h"
#include "Render/light/directionalLight.h"
#include "Render/light/ambientLight.h"
#include "Render/render/renderTarget.h"
#include "Render/geometry/planeGeometry.h"
#include "Render/tools/timer.h"
#include "Render/logger/debugLog.h"

using namespace pe;

Scene::Ptr	scene = nullptr;
Mesh::Ptr	mesh = nullptr;
Mesh::Ptr	phongMesh = nullptr;
Mesh::Ptr	depthMesh = nullptr;
Mesh::Ptr	plane = nullptr;
DirectionalLight::Ptr directionalLight0 = nullptr;
DirectionalLight::Ptr directionalLight1 = nullptr;
DirectionalLight::Ptr directionalLight2 = nullptr;
RenderTarget::Ptr renderTarget = nullptr;
Camera::Ptr camera = nullptr;
GameCameraControl::Ptr cameraControl = nullptr;


static void onMouseMove(double xpos, double ypos) {
	cameraControl->onMouseMove(xpos, ypos);
}

static void onMouseAction(MouseAction action) {
	cameraControl->onMouseAction(action);
}

static void onKeyboardAction(KeyBoardState action) {
	cameraControl->onKeyboard(action);
}

void onResize(int width, int height) {
}

float t = 0.0;
float radius = 5.0f;
void rotateLight() {

	glm::vec3 position;
	position.x = 0.0f;
	position.y = 10.0f;
	position.z = std::sin(t) * radius;


	directionalLight0->setPosition(position);


	t += 0.005f;
}

Scene::Ptr makeScene() {


	auto texture = TextureLoader::load("assets/textures/floor.jpg");

	MeshBasicMaterial::Ptr material = MeshBasicMaterial::create();
	material->mFrontFace = FrontFace::FrontCounterClockWise;
	material->mSide = Side::DoubleSide;
	material->mDiffuseMap = texture;
	material->mDepthWrite = true;
	material->mDrawMode = DrawMode::Triangles;

	auto phongGeometry = BoxGeometry::create(5.0, 5.0, 0.5);

	MeshPhongMaterial::Ptr phongMaterial = MeshPhongMaterial::create();
	phongMaterial->mDiffuseMap = texture;
	phongMaterial->mSide = Side::FrontSide;

	phongMesh = Mesh::create(phongGeometry, phongMaterial);
	phongMesh->mCastShadow = true;
	phongMesh->setPosition(0.0f, 0.0f, 0.0f);

	Scene::Ptr scene = Scene::create();

	scene->addChild(phongMesh);

	auto planeGeometry = PlaneGeometry::create(10.0f, 10.0f, 1, 1);
	plane = Mesh::create(planeGeometry, phongMaterial);
	plane->rotateX(-90.0f);
	plane->setPosition(0.0, -1.0, 0.0);
	scene->addChild(plane);
	//sky box


	std::vector<std::string> cubePaths = {
		"assets/textures/skybox/posx.jpg",
		"assets/textures/skybox/negx.jpg",
		"assets/textures/skybox/posy.jpg",
		"assets/textures/skybox/negy.jpg",
		"assets/textures/skybox/posz.jpg",
		"assets/textures/skybox/negz.jpg",
	};

	CubeTexture::Ptr cubeTexture = CubeTextureLoader::load(cubePaths);
	scene->mBackground = cubeTexture;
	
	float projectSize = 14.0;
	directionalLight0 = DirectionalLight::create();
	directionalLight0->setPosition(2.5f, 3.0f, -5.0f);
	directionalLight0->mColor = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight0->mIntensity = 1.0;
	directionalLight0->lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	directionalLight0->mCastShadow = true;
	directionalLight0->mShadow->mMapSize = glm::vec2(4096.0, 4096.0);
	directionalLight0->mShadow->mRadius = 3.0f;
	directionalLight0->mShadow->mCamera = OrthographicCamera::create(-projectSize, projectSize, -projectSize, projectSize, -20, 20);

	directionalLight1 = DirectionalLight::create();
	directionalLight1->setPosition(4.0f, 4.0f, -4.0f);
	directionalLight1->mIntensity = 0.8;
	directionalLight1->mColor = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight1->lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	directionalLight1->mCastShadow = true;
	directionalLight1->mShadow->mMapSize = glm::vec2(4096.0, 4096.0);
	directionalLight1->mShadow->mBias = -0.03;
	directionalLight1->mShadow->mCamera = OrthographicCamera::create(-projectSize, projectSize, -projectSize, projectSize, -20, 20);

	auto ambientLight = AmbientLight::create();
	ambientLight->mIntensity = 0.2f;

	scene->addChild(directionalLight0);
	//scene->addChild(directionalLight1);
	scene->addChild(ambientLight);


	return scene;
}

int main() {
	try {
		//DebugLog::getInstance()->mEnableDebug = true;
		scene = makeScene();

		Renderer::Descriptor rDc;
		rDc.mWidth = 1200;
		rDc.mHeight = 800;

		camera = PerspectiveCamera::create(0.1, 10000, static_cast<float>(rDc.mWidth) / static_cast<float>(rDc.mHeight), 45.0f);
		camera->setPosition(0, 4, 20);

		cameraControl = GameCameraControl::create(camera);
		cameraControl->setSpeed(0.05f);

		renderTarget = RenderTarget::create(rDc.mWidth, rDc.mHeight, RenderTarget::Options());

		Renderer::Ptr renderer = Renderer::create(rDc);
		renderer->setMouseActionCallback(onMouseAction);
		renderer->setKeyboardActionCallBack(onKeyboardAction);
		renderer->setFrameSizeCallBack(onResize);
		renderer->setMouseMoveCallBack(onMouseMove);

		float lastTime = glfwGetTime();
		while (true) {

			float currentTime = glfwGetTime();
			float deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			cameraControl->update();
			if (!renderer->render(scene, camera)) {
				break;
			}

			renderer->swap();
			//rotateLight();
		}
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}


	return 0;
}