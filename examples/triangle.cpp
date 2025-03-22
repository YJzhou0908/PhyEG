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


//窗体的大小
uint32_t WIDTH = 800;
uint32_t HEIGHT = 600;

//响应鼠标移动，收到的参数为鼠标当前位置
static void onMouseMove(double xpos, double ypos) {
}

//响应鼠标事件，比如点击或者抬起
static void onMouseAction(pe::MouseAction action) {
	if (action == pe::MouseAction::LeftDown) {
		std::cout << "LeftDown" << std::endl;
	}
}

//响应键盘事件，并且回传键盘状态
static void onKeyboardAction(KeyBoardState action) {
}

//响应窗口大小变化，并且回传变化后的窗体宽高
static void onResize(int width, int height) {
}

int main() {
	std::vector<float> positions = {
		-0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	std::vector<float> colors = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	std::vector<uint32_t> indices = {
		0, 1, 2
	};

	//创造几何数据，并且为mesh的顶点们，设置属性数据（Attribute，其中itemSize，
	//是指，每个顶点，相关属性，有多少个数字）
	auto geometry = pe::Geometry::create();
	geometry->setAttribute("position", pe::Attributef::create(positions, 3));
	geometry->setAttribute("color", pe::Attributef::create(colors, 3));
	geometry->setIndex(pe::Attributei::create(indices, 1));

	//生成基础材质
	auto material = pe::MeshBasicMaterial::create();

	//一个Mesh，包括它的几何信息，以及材质信息
	auto triangle = pe::Mesh::create(geometry, material);

	//创建场景，并且将三角形Mesh加入其中
	auto scene = pe::Scene::create();
	scene->addChild(triangle);

	//创建透视投影摄像机near far 宽高比，视角 位置
	auto camera = pe::PerspectiveCamera::create(0.1f, 100.0f, (float)WIDTH / (float)(HEIGHT), 60.0f);
	camera->setPosition(0.0f, 0.0f, 1.0f);

	//创建渲染器
	pe::Renderer::Descriptor rDc;
	rDc.mWidth = WIDTH;
	rDc.mHeight = HEIGHT;
	pe::Renderer::Ptr renderer = pe::Renderer::create(rDc);

	//设置背景清除色，类似与glClearColor
	renderer->setClearColor(0.94, 1.0, 0.94, 1.0);

	renderer->setMouseActionCallback(onMouseAction);
	renderer->setKeyboardActionCallBack(onKeyboardAction);
	renderer->setFrameSizeCallBack(onResize);
	renderer->setMouseMoveCallBack(onMouseMove);

	while (true) {
		if (!renderer->render(scene, camera)) {
			break;
		}

		renderer->swap();
	}

}