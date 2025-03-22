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
		//��CubeTextureȡ�� ,��ֵ��background
		auto background = scene->mBackground;
		if (background == nullptr) {
			return;
		}

		//�������ڻ�����պУ�CubeMap����������Mesh
		if (mBoxMesh == nullptr) {
			//����һ��1*1*1 ��boxGeometry
			auto geometry = BoxGeometry::create(1.0f, 1.0f, 1.0f);
			geometry->deleteAttribute("normal");
			geometry->deleteAttribute("uv");

			//�������Զ����box���ڲ����ģ�����ֻ�����ڲ�����
			auto material = CubeMaterial::create();
			material->mSide = Side::BackSide;
			material->mEnvMap = background;

			mBoxMesh = Mesh::create(geometry, material);

			mBoxMesh->mOnBeforeRenderCallback = [&](Renderer* render, Scene* scene, Camera* camera) {
				mBoxMesh->setPosition(camera->getWorldPosition());
				mBoxMesh->updateWorldMatrix();
			};

			//������Ҫ���������mesh����һ�ν�����������VBO��
			mObjects->update(mBoxMesh);
		}

		renderList->push(mBoxMesh, mBoxMesh->getGeometry(), mBoxMesh->getMaterial(), 0, 0);
	}

}