#pragma once
#include "../basic/common.h"
#include "../tools/identity.h"

namespace pe {
	class ObjectType {
	public:
		bool mIsRenderableObject{ false };
		bool mIsMesh{ false };
		bool mIsScene{ false };
		bool mIsCamera{ false };
		bool mIsPerspectiveCamera{ false };
		bool mIsOrthographicCamera{ false };
		bool mIsGroup{ false };
		bool mIsLight{ false };
		bool mIsAmbientLight{ false };
		bool mIsDirectionalLight{ false };
	};

	class Object3D : public enable_shared_from_this<Object3D>, public ObjectType{
	public:
		using Ptr = std::shared_ptr<Object3D>;
		static Ptr create() {
			return std::make_shared<Object3D>();
		}

		Object3D() noexcept;
		~Object3D() noexcept;


		// ʹ��dynamic_cast�� ��Ⱦ������Ҫ�̳�Object3D������Ҫ�����麯��
		virtual void fake() noexcept {}

		// Position��������Ļ���һ���д����Եĵ�
		void setPosition(float x, float y, float z) noexcept;

		void setPosition(glm::vec3& position) noexcept;

		void setQuaternion(float x, float y, float z, float w) noexcept;

		void setScale(float x, float y, float z) noexcept;

		// ��Щ��ת���������localMatrix������������������localMatrixת�Ƶ����������ϵ�¡�
		void rotateX(float angle) noexcept; 

		void rotateY(float angle) noexcept;

		void rotateZ(float angle) noexcept;

		void setRotateAroundAxis(const glm::vec3& axis, float angle) noexcept;

		void rotateAroundAxis(const glm::vec3& axis, float angle) noexcept;

		// ��������� �� ͷ��������
		void lookAt(const glm::vec3& target, const glm::vec3& up) noexcept;

		void setLocalMatrix(const glm::mat4& localMatrix) noexcept;

		void setWorldMatrix(const glm::mat4& worldMatrix) noexcept;

		//��ǰ��Object3D���棬�����ӽڵ�
		void addChild(const Object3D::Ptr& child) noexcept;


		virtual void updateMatrix() noexcept;

		virtual glm::mat4 updateWorldMatrix(bool updateParent = false, bool updateChildren = false) noexcept;

		glm::mat4 updateModelViewMatrix(const glm::mat4& viewMatrix) noexcept;

		// ���־����������
		glm::mat3 updateNormalMatrix() noexcept;

		glm::vec3 getPosition() const noexcept;

		glm::vec3 getWorldPosition() const noexcept;

		glm::vec3 getLocalDirection() const noexcept;

		glm::vec3 getWorldDirection() const noexcept;

		glm::vec3 getUp() const noexcept;

		glm::vec3 getRight() const noexcept;

		glm::mat4 getLocalMatrix() noexcept;

		glm::mat4 getWorldMatrix() noexcept;

		glm::mat4 getModelViewMatrix() noexcept;

		glm::mat3 getNormalMatrix() noexcept;

		const std::vector<Object3D::Ptr>& getChildren() const noexcept;

		ID getID() const noexcept;

	protected:
		void decompose() noexcept;

	public:
		bool	mVisible{ true };

		bool	mCastShadow{ true };

		std::string mName;

		bool	mNeedsUpdateMatrix{ true };


	protected:
		ID mID{ 0 };

		glm::vec3	mPosition{ glm::vec3(0.0f) };

		glm::quat	mQuaternion{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f) };

		glm::vec3	mScale{ glm::vec3(1.0f) };

		// ��������仯����
		glm::mat4	mLocalMatrix = glm::mat4(1.0f);

		// ȫ��ModelMatrix
		glm::mat4	mWorldMatrix = glm::mat4(1.0f);

		bool	    mNeedsUpdate{ false };

		std::weak_ptr<Object3D> mParent; // ����shared_ptrѭ������,week_ptr�ڲ��洢�ľ���shared_ptr

		std::vector<Object3D::Ptr> mChildren{};

		// ViewMatrix * ModelMatrix 
		glm::mat4 mModelViewMatrix = glm::mat4(1.0f);

		glm::mat3 mNormalMatrix = glm::mat3(1.0f);

	}

}