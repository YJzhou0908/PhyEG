#include "GLRenderList.h"

namespace pe {

	RenderItem::RenderItem() noexcept {}

	RenderItem::~RenderItem() noexcept {}

	GLRenderList::GLRenderList() {}

	GLRenderList::~GLRenderList() {}

	//ÿһ֡��ʼ��ʱ����Ⱦ�б��ᱻ���
	void GLRenderList::init() noexcept {
		mRenderItemIndex = 0;
		mOpaques.clear();
		mTransparents.clear();
	}

	//����������push��ȥһ������Ⱦ�������ز���������ķ�ʽ
	//Ϊʲô��Ҫ�������,�п��ܻ������,����������objectӵ��һ��material������sceneҲӵ��һ��overrideMaterial
	//��ô�Ͳ���ʹ��objectԭ����material
	void GLRenderList::push(
		const RenderableObject::Ptr& object,
		const Geometry::Ptr& geometry,
		const Material::Ptr& material,
		const uint32_t& groupOrder,
		float z
	) noexcept {
		//ÿһ֡�������¹���renderList�����Ա�����5�����壬�������renderItem�Ļ��棬��ô
		//ÿһ֡��Ҫ����new 5��renderItem
		//������һ֡���������Ѿ�������10��renderItem����ô�Ὣ�仺����renderItemCache���棬����Ҫ��ʱ�򣬾�
		//����ȡ��һ������renderListʹ��
		const auto renderItem = getNextRenderItem(object, geometry, material, groupOrder, z);

		//����Ƿ���͸��
		if (material->mTransparent) {
			mTransparents.push_back(renderItem);
		}
		else {
			mOpaques.push_back(renderItem);
		}
	}

	RenderItem::Ptr GLRenderList::getNextRenderItem(
		const RenderableObject::Ptr& object,
		const Geometry::Ptr& geometry,
		const Material::Ptr& material,
		const uint32_t& groupOrder,
		float z
	) noexcept {
		RenderItem::Ptr renderItem = nullptr;

		//�����ǰrenderItem�������Ѿ������˻�����������ô����������
		//����ֱ�Ӵӻ��浱�г��۵�mRenderItemIndex��item
		if (mRenderItemIndex >= mRenderItemCache.size()) {
			renderItem = RenderItem::create();
			mRenderItemCache.push_back(renderItem);
		}
		else {
			renderItem = mRenderItemCache[mRenderItemIndex];
		}

		renderItem->mID = object->getID();
		renderItem->mObject = object;
		renderItem->mGeometry = geometry;
		renderItem->mMaterial = material;
		renderItem->mGroupOrder = groupOrder;
		renderItem->mZ = z;

		mRenderItemIndex++;

		return renderItem;
	}

	void GLRenderList::sort(
		const RenderListSortFunction& opaqueSort,
		const RenderListSortFunction& transparentSort) noexcept {

		if (!mOpaques.empty()) std::sort(mOpaques.begin(), mOpaques.end(), opaqueSort);

		if (!mTransparents.empty()) std::sort(mTransparents.begin(), mTransparents.end(), transparentSort);
	}

	//������浱�д���10��item����ֻ֡��5��������Ҫ��Ⱦ����ô�ͻ�ʹ�õ�10��item��������
	void GLRenderList::finish() noexcept {
		auto listSize = mRenderItemCache.size();
		for (uint32_t i = mRenderItemIndex; i < listSize; ++i) {
			auto renderItem = mRenderItemCache[i];
			if (renderItem == nullptr) {
				break;
			}

			//������е�����ָ�������Լ�����
			renderItem->mID = 0;
			renderItem->mObject = nullptr;
			renderItem->mGeometry = nullptr;
			renderItem->mGroupOrder = 0;
			renderItem->mMaterial = nullptr;
			renderItem->mZ = 0;
		}
	}
}