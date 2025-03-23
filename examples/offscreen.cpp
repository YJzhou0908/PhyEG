#include "Render/core/attribute.h"
#include "Render/core/geometry.h"
#include "Render/objects/mesh.h"
#include "Render/scene/scene.h"
#include "Render/camera/perspectiveCamera.h"
#include "Render/cameraControl/gameCameraControl.h"
#include "Render/render/renderer.h"
#include "Render/material/meshBasicMaterial.h"
#include "Render/logger/debugLog.h"
#include "Render/basic/constants.h"
#include "Render/geometry/boxGeometry.h"
#include "Render/textures/texture.h"
#include "Render/loader/textureLoader.h"
#include "Render/loader/cubeTextureLoader.h"
#include "Render/wrapper/glWrapper.hpp"

uint32_t WIDTH = 800;
uint32_t HEIGHT = 600;

//mesh���͵�����ָ��
pe::Mesh::Ptr cube = nullptr;

static void onMouseMove(double xpos, double ypos) {
}

static void onMouseAction(pe::MouseAction action) {
}

static void onKeyboardAction(KeyBoardState action) {
}

static void onResize(int width, int height) {
}

float angle = 0.3f;
void rotateCube() {
	cube->rotateAroundAxis(glm::vec3(1.0, 1.0, 1.0), angle);
}

int main() {

	//����Cube��Geometry
	auto boxGeometry = pe::BoxGeometry::create(1.0, 1.0, 1.0);

	//������������
	auto material = pe::MeshBasicMaterial::create();
	auto diffuseMap = pe::TextureLoader::load("assets/textures/floor.png");
	material->mDiffuseMap = diffuseMap;

	cube = pe::Mesh::create(boxGeometry, material);

	auto scene = pe::Scene::create();
	scene->addChild(cube);

	//sky box
	std::vector<std::string> cubePaths = {
		"assets/textures/skybox/posx.jpg",
		"assets/textures/skybox/negx.jpg",
		"assets/textures/skybox/posy.jpg",
		"assets/textures/skybox/negy.jpg",
		"assets/textures/skybox/posz.jpg",
		"assets/textures/skybox/negz.jpg",
	};

	pe::CubeTexture::Ptr cubeTexture = pe::CubeTextureLoader::load(cubePaths);
	scene->mBackground = cubeTexture;

	auto camera = pe::PerspectiveCamera::create(0.1f, 100.0f, (float)WIDTH / (float)(HEIGHT), 60.0f);
	camera->setPosition(0.0f, 0.0f, 2.0f);

	pe::Renderer::Descriptor rDc;
	rDc.mWidth = WIDTH;
	rDc.mHeight = HEIGHT;
	pe::Renderer::Ptr renderer = pe::Renderer::create(rDc);
	renderer->setClearColor(0.94, 1.0, 0.94, 1.0);

	renderer->setMouseActionCallback(onMouseAction);
	renderer->setKeyboardActionCallBack(onKeyboardAction);
	renderer->setFrameSizeCallBack(onResize);
	renderer->setMouseMoveCallBack(onMouseMove);

	pe::RenderTarget::Options options;
	pe::RenderTarget::Ptr renderTarget = pe::RenderTarget::create(WIDTH, HEIGHT, options);

	while (true) {
		//��һ��pass,��Ⱦ�����壬��Ⱦ��һ����ת��è������������������opescreen��Ⱦ��
		material->mDiffuseMap = diffuseMap;
		renderer->setRenderTarget(renderTarget);
		if (!renderer->render(scene, camera)) {
			break;
		}

		//�ڶ���pass������һ��pass��Ⱦ�Ľ����������Ⱦ��è�������壩����ΪdipeuseMap���͸��˵�ǰcubeʹ�õ�material
		material->mDiffuseMap = renderTarget->getTexture();

		std::vector<std::string> cubePaths = {
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/front.jpg",
			"assets/textures/skybox/back.jpg",
		};

		pe::CubeTexture::Ptr cubeTexture = pe::CubeTextureLoader::load(cubePaths);
		scene->mBackground = cubeTexture;

		//��Ϊ��Ĭ�ϵ�renderatarget������Ⱦ
		renderer->setRenderTarget(nullptr);
		if (!renderer->render(scene, camera)) {
			break;
		}

		renderer->swap();

		rotateCube();
	}

}