#pragma once
#include "../basic/common.h"
#include "../basic/constants.h"
#include "../textures/source.h"
#include "../basic/eventDispatcher.h"

namespace pe {

	//���ж�ȡ���Ѿ��������source
	//���µ�source ���浽���������
	class Cache {
	public:
		static Cache* getInstance();

		Cache() noexcept;

		~Cache() noexcept;

		Source::Ptr getSource(const std::string& path) noexcept;

		void cacheSource(const std::string& path, Source::Ptr source) noexcept;

		void onSourceRelease(const EventBase::Ptr& e);

	private:
		static Cache* mInstance;

		//hashType = size_t
		std::unordered_map<HashType, Source::Ptr> mSources{};
	};
}