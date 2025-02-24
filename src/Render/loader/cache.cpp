#include "cache.h"
#include "../textures/texture.h"
#include "../textures/cubeTexture.h"

namespace pe {
	Cache* Cache::mInstance = nullptr;
	Cache* Cache::getInstance() {
		if (mInstance == nullptr) {
			mInstance = new Cache();
		}

		return mInstance;
	}

	Cache::Cache() noexcept {
		EventDispatcher::getInstance()->addEventListener("sourceRelease", this, &Cache::onSourceRelease);
	}

	Cache::~Cache() noexcept {}

	Source::Ptr Cache::getSource(const std::string& path) noexcept {
		std::hash<std::string> hasher;
		auto hashCode = hasher(path);

		Source::Ptr source = nullptr;

		auto iter = mSources.find(hashCode);
		if (iter != mSources.end()) {
			source = iter->second;

			//ÿ�ε���get����Ȼ��һ��texture��ʹ������refCount +1
			source->mRefCount++;
		}

		return source;
	}

	void Cache::cacheSource(const std::string& path, Source::Ptr source) noexcept {
		std::hash<std::string> hasher;
		auto hashCode = hasher(path);

		//Ѱ���Ƿ��ظ�
		auto iter = mSources.find(hashCode);
		if (iter != mSources.end()) {
			return;
		}

		//û�ҵ�ͬ��ͬ�յ�source
		source->mHashCode = hashCode;

		//ÿ��ֻҪ����source����һ������һ��texture��ʹ������refCount�ͱ�Ȼ+1
		source->mRefCount++;

		mSources.insert(std::make_pair(hashCode, source));
	}

	//cache�����sourceRelease
	void Cache::onSourceRelease(const EventBase::Ptr& e) {
		auto source = static_cast<Source*>(e->mTarget);
		auto hashCode = source->mHashCode;

		auto iter = mSources.find(hashCode);
		if (iter == mSources.end()) {
			return;
		}

		//���ȷʵ������cache���棬�����ü���-1
		source->mRefCount--;

		//������ü��� = 0��˵����������û������ʹ�����source����ô�ʹ�cache�����
		if (source->mRefCount == 0) {

			mSources.erase(iter);
		}
	}
}