#pragma once
#include "common.h"

namespace pe {
	// 
	// 消息体结构
	class EventBase {
	public:
		using Ptr = std::shared_ptr<EventBase>;
		static Ptr create(const std::string& eventName) {
			return std::make_shared <EventBase>(eventName);
		}

		EventBase(const std::string& eventName) noexcept;

		~EventBase() noexcept;

		std::string mEventName;//消息的名字
		void* mTarget = nullptr;//消息的主体，哪个对象发出的,存储其指针
		void* mpUserData{ nullptr };//根据不同的需求，将userData设置成不同的对象指针
	};

	// 一个单例类
	class EventDispatcher {
	public:
		// T类型下的函数指针，参数为EventBase消息体，注册消息使用
		template<typename T>
		using TypedFunctionPointer = void(T::*)(const EventBase::Ptr&);

		using EventFunction = std::function<void(const EventBase::Ptr&)>;

		struct Listener {
			using Ptr = std::shared_ptr<Listener>;

			static Ptr create() {
				return std::make_shared<Listener>();
			}

			Listener() noexcept {}

			~Listener() noexcept {
				if (mFuncionPointerDescriptor.mAddr) {
					free(mFuncionPointerDescriptor.mAddr);
				}
			}

			struct FunctionPointerDescriptor {
				//描述函数指针的地址（存储函数指针的地址）
				void* mAddr = nullptr;
				size_t	mSize{ 0 };

				bool operator==(const FunctionPointerDescriptor& other) const {
					if (other.mSize != mSize) {
						return false;
					}

					//检查函数指针指向的对应的是不是同一个地址
					if (memcmp(mAddr, other.mAddr, mSize) != 0) {
						return false;
					}

					return true;
				}
			};
			void* mTarget = nullptr;//某一个对象的this指针

			FunctionPointerDescriptor mFuncionPointerDescriptor{};

			std::function<void(const EventBase::Ptr&)> mFunction = nullptr;


			// 给需要注册的函数构建一个描述结构
			template<typename T>
			static FunctionPointerDescriptor buildFunctionPointer(TypedFunctionPointer<T> functionPointer) noexcept {
				size_t s = sizeof(TypedFunctionPointer<T>);
				void* p = malloc(s);

				//placement New 先分配了一块内存，然后通过placementNew调用了其构造函数/默认构造
				new(p)TypedFunctionPointer<T>(functionPointer);

				Listener::FunctionPointerDescriptor fpd;
				fpd.mAddr = p;//p指向的内存当中，存储了functionPointer的指针值
				fpd.mSize = s;//functionPointer的指针值的大小 

				return fpd;
			}




		};

		static bool listenerIsEqual(const Listener::Ptr& l0, const Listener::Ptr& l1) {
			//调用对象的地址一致，且对应的函数地址一致即可 
			return l0->mTarget == l1->mTarget && l0->mFuncionPointerDescriptor == l1->mFuncionPointerDescriptor;
		}

		
		using ListenerQueue = std::vector<Listener::Ptr>;

		EventDispatcher() noexcept;

		~EventDispatcher() noexcept;

		static EventDispatcher* getInstance();

		template<typename T>
		void addEventListener(const std::string& name, T* target, TypedFunctionPointer<T> functionPointer) noexcept;

		template<typename T>
		void removeEventListener(const std::string& name, T* target, TypedFunctionPointer<T> functionPointer) noexcept;

		void dispatchEvent(const EventBase::Ptr& event);

	protected:
		//存储了监听事件名称——监听函数队列
		//同一个事件名称，可能会有多个Listener监听（Listener存储了事件发生是需要调用的函数）
		std::unordered_map<std::string, ListenerQueue> mListeners; 
		static EventDispatcher* mInstance;

	};

}