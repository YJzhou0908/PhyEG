#include "GLBackground.h"
#include "../../geometry/boxGeometry.h"
#include "../../material/cubeMaterial.h"
#include "../../camera/camera.h"
#include "../renderer.h"

namespace pe {

	GLBackground::GLBackground(Renderer* renderer, const GLObjects::Ptr& objects) noexcept {
		mRenderer = renderer;
		mObjects = objects;
	}

	GLBackground::~GLBackground() noexcept {
	}

	void GLBackground::render(const GLRenderList::Ptr& renderList, const Scene::Ptr& scene) {
		//clear renderer
		if (mRenderer->mAutoClear) {
			mRenderer->clear();
		}

		//prepare background
		//将CubeTexture取出 ,赋值给background
		auto background = scene->mBackground;
		if (background == nullptr) {
			return;
		}

		//构造用于绘制天空盒（CubeMap）的立方体Mesh
		if (mBoxMesh == nullptr) {
			//构建一个1*1*1 的boxGeometry
			auto geometry = BoxGeometry::create(1.0f, 1.0f, 1.0f);
			geometry->deleteAttribute("normal");
			geometry->deleteAttribute("uv");

			//摄像机永远放在box的内部中心，所以只绘制内部即可
			auto material = CubeMaterial::create();
			material->mSide = Side::BackSide;
			material->mEnvMap = background;

			mBoxMesh = Mesh::create(geometry, material);

			mBoxMesh->mOnBeforeRenderCallback = [&](Renderer* render, Scene* scene, Camera* camera) {
				mBoxMesh->setPosition(camera->getWorldPosition());
				mBoxMesh->updateWorldMatrix();
			};

			//在这里要单独对这个mesh进行一次解析，创建其VBO等
			mObjects->update(mBoxMesh);
		}

		renderList->push(mBoxMesh, mBoxMesh->getGeometry(), mBoxMesh->getMaterial(), 0, 0);
	}

}