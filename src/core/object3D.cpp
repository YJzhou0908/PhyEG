#include "object3D.h"

namespace pe {

	Object3D::Object3D() noexcept {
		mID = Identity::generateID();
	};
	Object3D::~Object3D() noexcept {
		// 析构分发函数
	}

	// Position物体的中心或者一个有代表性的点
	void Object3D::setPosition(float x, float y, float z) noexcept {
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
	}

	void Object3D::setPosition(glm::vec3& position) noexcept {
		mPosition = position;

		//a b c d
		//e f g h
		//i j k l
		//m n o p
		// 第四列是位置 或者说是平移变量
		mLocalMatrix[3].x = position.x;
		mLocalMatrix[3].y = position.y;
		mLocalMatrix[3].z = position.z;

	}

	void Object3D::setQuaternion(float x, float y, float z, float w) noexcept {

		//a b c d
		//e f g h
		//i j k l
		//m n o p

		// 根据Quaternion生成旋转矩阵
		glm::quat qua(w, x, y, z);

		glm::mat4 rotateMatrix = glm::mat4_cast(qua);

		// 前三列是scale和rotate结合的
		float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
		float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
		float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));

		mLocalMatrix[0] = rotateMatrix[0] * scaleX;
		mLocalMatrix[1] = rotateMatrix[1] * scaleY;
		mLocalMatrix[2] = rotateMatrix[2] * scaleZ;
	}


	void Object3D::decompose() noexcept{
		
		glm::vec3 skew;
		glm::vec4 perspective;

		//是将变换矩阵当中的参数们，抽离出来 
		glm::decompose(mLocalMatrix, mScale, mQuaternion, mPosition, skew, perspective);
		
	}

	void Object3D::setScale(float x, float y, float z) noexcept {
		
		// 先把数据取出来，然后归一化，然后再乘
		auto xcol = glm::normalize(glm::vec3(mLocalMatrix[0])) * x;
		auto ycol = glm::normalize(glm::vec3(mLocalMatrix[1])) * y;
		auto zcol = glm::normalize(glm::vec3(mLocalMatrix[2])) * z;

		mLocalMatrix[0] = glm::vec4(xcol, 0.0f);
		mLocalMatrix[1] = glm::vec4(ycol, 0.0f);
		mLocalMatrix[2] = glm::vec4(zcol, 0.0f);

		// 处理之后，要将处理数据保存起来
		decompose();
	}

	// 这些旋转都是相对于localMatrix来做操作，最后会把这个localMatrix转移到摄像机坐标系下。
	void Object3D::rotateX(float angle) noexcept {
		glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[0]);

		glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f),glm::radians(angle), rotateAxis);

		mLocalMatrix = rotateMatrix * mLocalMatrix;

		decompose();
	}

	void Object3D::rotateY(float angle) noexcept {
		glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[1]);

		glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotateAxis);

		mLocalMatrix = rotateMatrix * mLocalMatrix;

		decompose();
	}

	void Object3D::rotateZ(float angle) noexcept {
		glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[2]);

		glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotateAxis);

		mLocalMatrix = rotateMatrix * mLocalMatrix;

		decompose();
	}


	// 绕轴重新旋转
	void Object3D::setRotateAroundAxis(const glm::vec3& axis, float angle) noexcept {
		glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), axis);

		//decompose scale
		float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
		float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
		float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(scaleX, scaleY, scaleZ));

		mLocalMatrix[0] = rotateMatrix[0];
		mLocalMatrix[1] = rotateMatrix[1];
		mLocalMatrix[2] = rotateMatrix[2];

		mLocalMatrix *= scaleMatrix;

		decompose();

	}

	void Object3D::rotateAroundAxis(const glm::vec3& axis, float angle) noexcept {

		mLocalMatrix = glm::rotate(mLocalMatrix, glm::radians(angle), axis);
		decompose();
	}

	// 看向的坐标 及 头顶的坐标
	void Object3D::lookAt(const glm::vec3& target, const glm::vec3& up) noexcept {
		// 确保scale不变
	
		float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
		float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
		float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));
		// 获取当前的位置计算-z方向
		// up向量是一个参考方向，该方向一般设置为glm::vec3(0.0f, +/-1.0f, 0.0f)
		glm::vec3 position = glm::vec3(mLocalMatrix[3]);

		glm::vec3 targetDirection = glm::normalize(glm::vec3(target - position));
		glm::vec3 rightDirection = glm::normalize(glm::cross(up, targetDirection));
		glm::vec3 upDirection = glm::normalize(glm::cross(rightDirection, targetDirection));

		// 设置localMatrix
		mLocalMatrix[0] = glm::vec4(-rightDirection, 0.0f) * scaleX;
		mLocalMatrix[1] = glm::vec4(upDirection, 0.0f) * scaleY;
		mLocalMatrix[2] = glm::vec4(-targetDirection, 0.0f) * scaleZ;
		mLocalMatrix[3] = glm::vec4(position, 1.0f);

		decompose();
		
	}

	void Object3D::updateMatrix() noexcept {
		if (mNeedsUpdateMatrix) {
			mNeedsUpdateMatrix = false;
			auto translateMatrix = glm::translate(glm::mat4(1.0f), mPosition);
			auto rotateMatrix = glm::mat4_cast(mQuaternion);
			auto scaleMatrix = glm::scale(glm::mat4(1.0f), mScale);

			mLocalMatrix = translateMatrix * rotateMatrix * scaleMatrix;
		}
	}

	glm::mat4 Object3D::updateWorldMatrix(bool updateParent = false, bool updateChildren = false) noexcept {
		//更新的时候要把父亲和孩子都更新了
			//检查有没有父节点
		if (!mParent.expired() && updateParent) {
			auto parent = mParent.lock();//拿到父节点的sharedPtr
			parent->updateWorldMatrix(true, false);//调用父节点的worldMatrix升级更新
		}

		//更新自己的localMatrix 
		updateMatrix();

		//初始化worldMatrix，如果没有父节点，那么二者相等
		mWorldMatrix = mLocalMatrix;

		//如果有父节点，那么需要做成父节点的worldMatrix，从而把上方所有节点的影响带入
		if (!mParent.expired()) {
			auto parent = mParent.lock();
			mWorldMatrix = parent->mWorldMatrix * mWorldMatrix;
		}

		//依次更新子节点的worldMatrix
		if (updateChildren) {
			for (auto& child : mChildren) {
				child->updateWorldMatrix(false, true);
			}
		}

		return mWorldMatrix;
	
	}

	glm::mat4 Object3D::updateModelViewMatrix(const glm::mat4& viewMatrix) noexcept {
		mModelViewMatrix = viewMatrix * mWorldMatrix;
		return mModelViewMatrix;
	}

	// 发现矩阵是MV矩阵转置的逆矩阵
	glm::mat3 Object3D::updateNormalMatrix() noexcept {
		//normalMatrix 由于存在scale的影响，不能直接变换，否则normal会无法保证垂直于表面
		mNormalMatrix = glm::transpose(glm::inverse(glm::mat3(mModelViewMatrix)));
		return mNormalMatrix;
	}


	void Object3D::setLocalMatrix(const glm::mat4& localMatrix) noexcept {
		mLocalMatrix = localMatrix;
	}

	void Object3D::setWorldMatrix(const glm::mat4& worldMatrix) noexcept {
		mWorldMatrix = worldMatrix;
	}

	//向当前的Object3D里面，加入子节点
	void Object3D::addChild(const Object3D::Ptr& child) noexcept {
		// 确认不是自己
		if (child == shared_from_this()) {
			return;
		}

		// week_ptr避免了孩子引用父亲的冲突
		child->mParent = shared_form_this();

		//查找当前节点下，是否已经加入该Child
		auto iter = std::find(mChildren.begin(), mChildren.end(), child);

		//iterator迭代器，如果找到，就会返回当前这个child对应到数组中相同值的iterator
		if (iter != mChildren.end()) return;

		mChildren.push_back(child);
	}



	// 相对于父亲的位置
	glm::vec3 Object3D::getPosition() const noexcept {
		return glm::vec3(mLocalMatrix[3]);
	}

	glm::vec3 Object3D::getWorldPosition() const noexcept {
		return glm::vec3(mWorldMatrix[3]);
	
	}

	glm::vec3 Object3D::getLocalDirection() const noexcept {
		// 看向自己的-z方向
		return glm::normalize(-glm::vec3(mLocalMatrix[2]));
	}

	glm::vec3 Object3D::getWorldDirection() const noexcept {
		//  
		return glm::normalize(-glm::vec3(mWorldMatrix[2]));
	}

	glm::vec3 Object3D::getUp() const noexcept {
		// 看向自己的-z，头顶默认是自己的y，获取头顶的世界坐标还得乘worldMatrix
		return glm::normalize(glm::vec3(mLocalMatrix[1]));
	}

	glm::vec3 Object3D::getRight() const noexcept {
		return glm::normalize(glm::vec3(mLocalMatrix[0]));
	}

	glm::mat4 Object3D::getLocalMatrix() noexcept {
		return mLocalMatrix;
	}

	glm::mat4 Object3D::getWorldMatrix() noexcept {
		return mWorldMatrix;
	}

	glm::mat4 Object3D::getModelViewMatrix() noexcept {
		return mModelViewMatrix;
	}

	glm::mat3 Object3D::getNormalMatrix() noexcept {
		return mNormalMatrix;
	}

	const std::vector<Object3D::Ptr>& Object3D::getChildren() const noexcept {
		return mChildren;
	
	}

	ID Object3D::getID() const noexcept { return mID; }


}


