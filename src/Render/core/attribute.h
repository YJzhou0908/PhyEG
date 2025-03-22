#pragma once
#include "../basic/common.h"
#include "../basic/constants.h"
#include "../tools/identity.h"
#include "../basic/eventDispatcher.h"

namespace pe {

	// class Attribute，VBO的前端
	template <typename T>
	class Attribute : public std::enable_shared_from_this<Attribute<T>> {
	public:
		using Ptr = std::shared_ptr<Attribute<T>>; //这是一种属性类型

		static Ptr create(const std::vector<T>& data, uint32_t itemSize, BufferAllocType bufferAllocType = BufferAllocType::StaticDrawBuffer) {
			return std::make_shared<Attribute<T>>(data, itemSize, bufferAllocType);
		
		}

		Attribute(const std::vector<T> &data, uint32_t itemSize, BufferAllocType  bufferAllocType = BufferAllocType::StaticDrawBuffer) noexcept;
		~Attribute() noexcept;

		auto getID() const noexcept { return mID; }

		auto getData() const noexcept { return mData; }

		auto getCount() const noexcept { return mCount; }

		auto getItemSize() const noexcept { return mItemSize; }

		bool getNeedsUpdate() const noexcept { return mNeedsUpdate; }

		void clearNeedsUpdate() noexcept { mNeedsUpdate = false; }

		auto getBufferAllocType() const noexcept { return mBufferAllocType; }

		Range getUpdateRange() const noexcept { return mUpdateRange; }

		void clearUpdateRange() noexcept { mUpdateRange.mOffset = 0; mUpdateRange.mCount = -1; }

		auto getDataType() const noexcept { return mDataType; }

		void setX(const uint32_t& index, T value) noexcept;

		void setY(const uint32_t& index, T value) noexcept;

		void setZ(const uint32_t& index, T value) noexcept;

		T getX(const uint32_t& index) noexcept;

		T getY(const uint32_t& index) noexcept;

		T getZ(const uint32_t& index) noexcept;


	private:
		ID mID{ 0 }; // 所有的对象被创建的时候只有一个Id

		std::vector<T> mData{};

		uint32_t mItemSize{ 0 };

		uint32_t mCount{ 0 }; // 

		BufferAllocType mBufferAllocType{ BufferAllocType::StaticDrawBuffer };

		DataType mDataType{ DataType::FloatType };

		bool mNeedsUpdate{ true };

		Range mUpdateRange{};

	};

	template<typename T>
	Attribute<T>::Attribute(const std::vector<T>& data, uint32_t itemSize, BufferAllocType  bufferAllocType) noexcept {
		mID = Identity::generateID();
		mData = data;
		mItemSize = itemSize;
		mCount = static_cast<uint32_t>(mData.size()) / itemSize;
		mBufferAllocType = bufferAllocType;
		mDataType = toDataType<T>();
	}
	
	template<typename T>
	Attribute<T>::~Attribute() noexcept {
		EventBase::Ptr e = EventBase::create("attributeDispose");
		e->mTarget = this;
		e->mpUserData = &mID;

		EventDispatcher::getInstance()->dispatchEvent(e);
	}

	template<typename T>
	void Attribute<T>::setX(const uint32_t& index, T value) noexcept {
		assert(index < mCount);
		mData[index * mItemSize] = value;
		mNeedsUpdate = true;
	}

	template<typename T>
	void Attribute<T>::setY(const uint32_t& index, T value) noexcept {
		assert(index < mCount);
		mData[index * mItemSize + 1] = value;
		mNeedsUpdate = true;
	}

	template<typename T>
	void Attribute<T>::setZ(const uint32_t& index, T value) noexcept {
		assert(index < mCount);
		mData[index * mItemSize + 2] = value;
		mNeedsUpdate = true;
	}

	template<typename T>
	T Attribute<T>::getX(const uint32_t& index) noexcept {
		assert(index < mCount);
		return mData[index * mItemSize];
	}

	template<typename T>
	T Attribute<T>::getY(const uint32_t& index) noexcept {
		assert(index < mCount);
		return mData[index * mItemSize + 1];
	}

	template<typename T>
	T Attribute<T>::getZ(const uint32_t& index) noexcept {
		assert(index < mCount);
		return mData[index * mItemSize + 2];
	}


	using Attributef = Attribute<float>;
	using Attributei = Attribute<uint32_t>;
}