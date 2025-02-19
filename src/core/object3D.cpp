#include "object3D.h"

namespace pe {

	Object3D::Object3D() noexcept {
		mID = Identity::generateID();
	};
	Object3D::~Object3D() noexcept {
		// �����ַ�����
	}

	// Position��������Ļ���һ���д����Եĵ�
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
		// ��������λ�� ����˵��ƽ�Ʊ���
		mLocalMatrix[3].x = position.x;
		mLocalMatrix[3].y = position.y;
		mLocalMatrix[3].z = position.z;

	}

	void Object3D::setQuaternion(float x, float y, float z, float w) noexcept {

		//a b c d
		//e f g h
		//i j k l
		//m n o p

		// ����Quaternion������ת����
		glm::quat qua(w, x, y, z);

		glm::mat4 rotateMatrix = glm::mat4_cast(qua);

		// ǰ������scale��rotate��ϵ�
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

		//�ǽ��任�����еĲ����ǣ�������� 
		glm::decompose(mLocalMatrix, mScale, mQuaternion, mPosition, skew, perspective);
		
	}

	void Object3D::setScale(float x, float y, float z) noexcept {
		
		// �Ȱ�����ȡ������Ȼ���һ����Ȼ���ٳ�
		auto xcol = glm::normalize(glm::vec3(mLocalMatrix[0])) * x;
		auto ycol = glm::normalize(glm::vec3(mLocalMatrix[1])) * y;
		auto zcol = glm::normalize(glm::vec3(mLocalMatrix[2])) * z;

		mLocalMatrix[0] = glm::vec4(xcol, 0.0f);
		mLocalMatrix[1] = glm::vec4(ycol, 0.0f);
		mLocalMatrix[2] = glm::vec4(zcol, 0.0f);

		// ����֮��Ҫ���������ݱ�������
		decompose();
	}

	// ��Щ��ת���������localMatrix������������������localMatrixת�Ƶ����������ϵ�¡�
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


	// ����������ת
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

	// ��������� �� ͷ��������
	void Object3D::lookAt(const glm::vec3& target, const glm::vec3& up) noexcept {
		// ȷ��scale����
	
		float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
		float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
		float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));
		// ��ȡ��ǰ��λ�ü���-z����
		// up������һ���ο����򣬸÷���һ������Ϊglm::vec3(0.0f, +/-1.0f, 0.0f)
		glm::vec3 position = glm::vec3(mLocalMatrix[3]);

		glm::vec3 targetDirection = glm::normalize(glm::vec3(target - position));
		glm::vec3 rightDirection = glm::normalize(glm::cross(up, targetDirection));
		glm::vec3 upDirection = glm::normalize(glm::cross(rightDirection, targetDirection));

		// ����localMatrix
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
		//���µ�ʱ��Ҫ�Ѹ��׺ͺ��Ӷ�������
			//�����û�и��ڵ�
		if (!mParent.expired() && updateParent) {
			auto parent = mParent.lock();//�õ����ڵ��sharedPtr
			parent->updateWorldMatrix(true, false);//���ø��ڵ��worldMatrix��������
		}

		//�����Լ���localMatrix 
		updateMatrix();

		//��ʼ��worldMatrix�����û�и��ڵ㣬��ô�������
		mWorldMatrix = mLocalMatrix;

		//����и��ڵ㣬��ô��Ҫ���ɸ��ڵ��worldMatrix���Ӷ����Ϸ����нڵ��Ӱ�����
		if (!mParent.expired()) {
			auto parent = mParent.lock();
			mWorldMatrix = parent->mWorldMatrix * mWorldMatrix;
		}

		//���θ����ӽڵ��worldMatrix
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

	// ���־�����MV����ת�õ������
	glm::mat3 Object3D::updateNormalMatrix() noexcept {
		//normalMatrix ���ڴ���scale��Ӱ�죬����ֱ�ӱ任������normal���޷���֤��ֱ�ڱ���
		mNormalMatrix = glm::transpose(glm::inverse(glm::mat3(mModelViewMatrix)));
		return mNormalMatrix;
	}


	void Object3D::setLocalMatrix(const glm::mat4& localMatrix) noexcept {
		mLocalMatrix = localMatrix;
	}

	void Object3D::setWorldMatrix(const glm::mat4& worldMatrix) noexcept {
		mWorldMatrix = worldMatrix;
	}

	//��ǰ��Object3D���棬�����ӽڵ�
	void Object3D::addChild(const Object3D::Ptr& child) noexcept {
		// ȷ�ϲ����Լ�
		if (child == shared_from_this()) {
			return;
		}

		// week_ptr�����˺������ø��׵ĳ�ͻ
		child->mParent = shared_form_this();

		//���ҵ�ǰ�ڵ��£��Ƿ��Ѿ������Child
		auto iter = std::find(mChildren.begin(), mChildren.end(), child);

		//iterator������������ҵ����ͻ᷵�ص�ǰ���child��Ӧ����������ֵͬ��iterator
		if (iter != mChildren.end()) return;

		mChildren.push_back(child);
	}



	// ����ڸ��׵�λ��
	glm::vec3 Object3D::getPosition() const noexcept {
		return glm::vec3(mLocalMatrix[3]);
	}

	glm::vec3 Object3D::getWorldPosition() const noexcept {
		return glm::vec3(mWorldMatrix[3]);
	
	}

	glm::vec3 Object3D::getLocalDirection() const noexcept {
		// �����Լ���-z����
		return glm::normalize(-glm::vec3(mLocalMatrix[2]));
	}

	glm::vec3 Object3D::getWorldDirection() const noexcept {
		//  
		return glm::normalize(-glm::vec3(mWorldMatrix[2]));
	}

	glm::vec3 Object3D::getUp() const noexcept {
		// �����Լ���-z��ͷ��Ĭ�����Լ���y����ȡͷ�����������껹�ó�worldMatrix
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


