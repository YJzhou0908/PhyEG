#include "Render/core/attribute.h"
#include "Render/core/geometry.h"
#include "Render/objects/mesh.h"
#include "Render/scene/scene.h"
#include "Render/camera/perspectiveCamera.h"
#include "Render/cameraControl/gameCameraControl.h"
#include "Render/render/renderer.h"
#include "Render/material/meshBasicMaterial.h"
#include "Render/material/meshPhongMaterial.h"
#include "Render/material/depthMaterial.h"
#include "Render/logger/debugLog.h"
#include "Render/basic/constants.h"
#include "Render/geometry/boxGeometry.h"
#include "Render/textures/texture.h"
#include "Render/loader/textureLoader.h"
#include "Render/light/directionalLight.h"


uint32_t WIDTH = 800;
uint32_t HEIGHT = 600;

pe::Mesh::Ptr meshBasicCube = nullptr;
pe::Mesh::Ptr meshPhongCube = nullptr;
pe::Mesh::Ptr meshTransparentCube = nullptr;


static void onMouseMove(double xpos, double ypos) {
}

static void onMouseAction(pe::MouseAction action) {
}

static void onKeyboardAction(KeyBoardState action) {
}

static void onResize(int width, int height) {
}

float angle = 45.0f;
void rotateCube() {
	meshBasicCube->rotateAroundAxis(glm::vec3(1.0, 1.0, 1.0), angle);
	meshPhongCube->rotateAroundAxis(glm::vec3(1.0, 1.0, 1.0), angle);
	meshTransparentCube->rotateAroundAxis(glm::vec3(1.0, 1.0, 1.0), angle);
}

int main() {
	auto boxGeometry = pe::BoxGeometry::create(1.0, 1.0, 1.0);

	auto meshBasicMaterial = pe::MeshBasicMaterial::create();
	meshBasicMaterial->mDiffuseMap = pe::TextureLoader::load("assets/textures/defaultTexture.jpg");
	meshBasicCube = pe::Mesh::create(boxGeometry, meshBasicMaterial);
	meshBasicCube->setPosition(-2.0, 0.0, 0.0);

	//与光照有反应
	auto meshPhongMaterial = pe::MeshPhongMaterial::create();
	meshPhongMaterial->mDiffuseMap = pe::TextureLoader::load("assets/textures/defaultTexture.jpg");
	meshPhongCube = pe::Mesh::create(boxGeometry, meshPhongMaterial);
	meshPhongCube->setPosition(0.0, 0.0, 0.0);


	//用的还是基础材质，只不过启用了透明度
	auto meshTransparentMaterial = pe::MeshBasicMaterial::create();
	meshTransparentMaterial->mDiffuseMap = pe::TextureLoader::load("assets/textures/defaultTexture.jpg");
	meshTransparentMaterial->mTransparent = true;
	meshTransparentMaterial->mOpacity = 0.4;
	meshTransparentCube = pe::Mesh::create(boxGeometry, meshTransparentMaterial);
	meshTransparentCube->setPosition(2.0, 0.0, 0.0);

	//创建平行光
	auto directionalLight = pe::DirectionalLight::create();
	directionalLight->setPosition(2.0f, 2.0f, 2.0f);
	directionalLight->mColor = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight->mIntensity = 2.0;
	directionalLight->lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	auto scene = pe::Scene::create();
	scene->addChild(meshBasicCube);
	scene->addChild(meshPhongCube);
	scene->addChild(meshTransparentCube);


	scene->addChild(directionalLight);

	auto camera = pe::PerspectiveCamera::create(0.1f, 100.0f, (float)WIDTH / (float)(HEIGHT), 60.0f);
	camera->setPosition(0.0f, 0.0f, 4.0f);

	camera->lookAt(glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

	pe::Renderer::Descriptor rDc;
	rDc.mWidth = WIDTH;
	rDc.mHeight = HEIGHT;
	pe::Renderer::Ptr renderer = pe::Renderer::create(rDc);
	renderer->setClearColor(0.2, 0.2, 0.2, 1.0);

	renderer->setMouseActionCallback(onMouseAction);
	renderer->setKeyboardActionCallBack(onKeyboardAction);
	renderer->setFrameSizeCallBack(onResize);
	renderer->setMouseMoveCallBack(onMouseMove);


	rotateCube();
	while (true) {
		if (!renderer->render(scene, camera)) {
			break;
		}

		renderer->swap();

		//rotateCube();
	}

}