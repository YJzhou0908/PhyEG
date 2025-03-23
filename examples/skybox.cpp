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
#include "Render/loader/textureLoader.h"
#include "Render/loader/cubeTextureLoader.h"
#include "Render/tools/timer.h"
#include "Render/logger/debugLog.h"

using namespace pe;

Scene::Ptr	scene = nullptr;
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

Scene::Ptr makeScene() {

	Scene::Ptr scene = Scene::create();

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

	return scene;
}

int main() {
	try {
		scene = makeScene();

		Renderer::Descriptor rDc;
		rDc.mWidth = 1200;
		rDc.mHeight = 800;

		camera = PerspectiveCamera::create(0.1, 10000, static_cast<float>(rDc.mWidth) / static_cast<float>(rDc.mHeight), 45.0f);
		camera->setPosition(0, 4, 20);

		cameraControl = GameCameraControl::create(camera);
		cameraControl->setSpeed(0.05f);


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
		}
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}


	return 0;
}