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


		// 使用dynamic_cast， 渲染对象需要继承Object3D，必须要求有虚函数
		virtual void fake() noexcept {}

		// Position物体的中心或者一个有代表性的点
		void setPosition(float x, float y, float z) noexcept;

		void setPosition(glm::vec3& position) noexcept;

		void setQuaternion(float x, float y, float z, float w) noexcept;

		void setScale(float x, float y, float z) noexcept;

		// 这些旋转都是相对于localMatrix来做操作，最后会把这个localMatrix转移到摄像机坐标系下。
		void rotateX(float angle) noexcept; 

		void rotateY(float angle) noexcept;

		void rotateZ(float angle) noexcept;

		void setRotateAroundAxis(const glm::vec3& axis, float angle) noexcept;

		void rotateAroundAxis(const glm::vec3& axis, float angle) noexcept;

		// 看向的坐标 及 头顶的坐标
		void lookAt(const glm::vec3& target, const glm::vec3& up) noexcept;

		void setLocalMatrix(const glm::mat4& localMatrix) noexcept;

		void setWorldMatrix(const glm::mat4& worldMatrix) noexcept;

		//向当前的Object3D里面，加入子节点
		void addChild(const Object3D::Ptr& child) noexcept;


		virtual void updateMatrix() noexcept;

		virtual glm::mat4 updateWorldMatrix(bool updateParent = false, bool updateChildren = false) noexcept;

		glm::mat4 updateModelViewMatrix(const glm::mat4& viewMatrix) noexcept;

		// 发现矩阵是逆矩阵
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

		// 本地坐标变化矩阵
		glm::mat4	mLocalMatrix = glm::mat4(1.0f);

		// 全局ModelMatrix
		glm::mat4	mWorldMatrix = glm::mat4(1.0f);

		bool	    mNeedsUpdate{ false };

		std::weak_ptr<Object3D> mParent; // 避免shared_ptr循环引用,week_ptr内部存储的就是shared_ptr

		std::vector<Object3D::Ptr> mChildren{};

		// ViewMatrix * ModelMatrix 
		glm::mat4 mModelViewMatrix = glm::mat4(1.0f);

		glm::mat3 mNormalMatrix = glm::mat3(1.0f);

	}

}