#pragma once
#include "common.h"

namespace pe {
	// 
	// ��Ϣ��ṹ
	class EventBase {
	public:
		using Ptr = std::shared_ptr<EventBase>;
		static Ptr create(const std::string& eventName) {
			return std::make_shared <EventBase>(eventName);
		}

		EventBase(const std::string& eventName) noexcept;

		~EventBase() noexcept;

		std::string mEventName;//��Ϣ������
		void* mTarget = nullptr;//��Ϣ�����壬�ĸ����󷢳���,�洢��ָ��
		void* mpUserData{ nullptr };//���ݲ�ͬ�����󣬽�userData���óɲ�ͬ�Ķ���ָ��
	};

	// һ��������
	class EventDispatcher {
	public:
		// T�����µĺ���ָ�룬����ΪEventBase��Ϣ�壬ע����Ϣʹ��
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
				//��������ָ��ĵ�ַ���洢����ָ��ĵ�ַ��
				void* mAddr = nullptr;
				size_t	mSize{ 0 };

				bool operator==(const FunctionPointerDescriptor& other) const {
					if (other.mSize != mSize) {
						return false;
					}

					//��麯��ָ��ָ��Ķ�Ӧ���ǲ���ͬһ����ַ
					if (memcmp(mAddr, other.mAddr, mSize) != 0) {
						return false;
					}

					return true;
				}
			};
			void* mTarget = nullptr;//ĳһ�������thisָ��

			FunctionPointerDescriptor mFuncionPointerDescriptor{};

			std::function<void(const EventBase::Ptr&)> mFunction = nullptr;


			// ����Ҫע��ĺ�������һ�������ṹ
			template<typename T>
			static FunctionPointerDescriptor buildFunctionPointer(TypedFunctionPointer<T> functionPointer) noexcept {
				size_t s = sizeof(TypedFunctionPointer<T>);
				void* p = malloc(s);

				//placement New �ȷ�����һ���ڴ棬Ȼ��ͨ��placementNew�������乹�캯��/Ĭ�Ϲ���
				new(p)TypedFunctionPointer<T>(functionPointer);

				Listener::FunctionPointerDescriptor fpd;
				fpd.mAddr = p;//pָ����ڴ浱�У��洢��functionPointer��ָ��ֵ
				fpd.mSize = s;//functionPointer��ָ��ֵ�Ĵ�С 

				return fpd;
			}




		};

		static bool listenerIsEqual(const Listener::Ptr& l0, const Listener::Ptr& l1) {
			//���ö���ĵ�ַһ�£��Ҷ�Ӧ�ĺ�����ַһ�¼��� 
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
		//�洢�˼����¼����ơ���������������
		//ͬһ���¼����ƣ����ܻ��ж��Listener������Listener�洢���¼���������Ҫ���õĺ�����
		std::unordered_map<std::string, ListenerQueue> mListeners; 
		static EventDispatcher* mInstance;

	};

}