#include "eventDispatch.h"

namespace pe{

	EventBase::EventBase(const std::string& eventName) noexcept {
		mEventName = eventName;
	}

	EventBase::~EventBase() noexcept {}

	EventDispatcher* EventDispatcher::mInstance = nullptr;
	EventDispatcher* EventDispatcher::getInstance() {
		if (mInstance == nullptr) {
			mInstance = new EventDispatcher();
		}
		return mInstance;
	}

	EventDispatcher::EventDispatcher() noexcept {}

	EventDispatcher::~EventDispatcher() noexcept {}

	void EventDispatcher::dispatchEvent(const EventBase::Ptr& event) {
		const auto& eventName = event->mEventName;

		auto listenerQueueIter = mListeners.find(eventName);
		if (listenerQueueIter == mListeners.end()) {
			return;
		}

		auto& queue = listenerQueueIter->second;
		for (const auto& listener : queue) {
			//mFunction���Ѿ�bind��target��function�ĺ�����,����ֱ��ִ��
			listener->mFunction(event);
		}
	}

	template<typename T>
	void EventDispatcher::addEventListener(const std::string& name, T* target, TypedFunctionPointer<T> functionPointer) noexcept {
		auto queueIter = mListeners.find(name);

		if (queueIter == mListeners.end()) {
			queueIter = (mListeners.insert(std::make_pair(name, ListenerQueue()))).first;
		}

		// ʹ�����ã�ȷ����������һ��vector
		auto& listenerQueue = queueIter->second;

		
		// �ҵ�target�µĸ�functionPointer����һ���ĺ���������function
		auto function = std::bind(functionPointer, target, std::placeholders::_1);

		// ����Listener
		Listener::Ptr listener = Listener::create();
		listener->mTarget = target;

		listener->mFuncionPointerDescriptor = Listener::buildFunctionPointer(functionPointer);
		listener->mFunction = function;

		// ����Ƿ����²���
		auto listenerIter = std::find_if(listenerQueue.begin(), listenerQueue.end(),
			[&](const Listener::Ptr& l) {
				return EventDispatcher::listenerIsEqual(listener, l);
			});
		if (listenerIter != listenerQueue.end()) {
			return;
		}

		listenerQueue.push_back(listener);
	}

	template<typename T>
	void EventDispatcher::removeEventListener(const std::string& name, T* target, TypedFunctionPointer<T> functionPointer) noexcept {
		auto queueIter = mListeners.find(name);

		if (queueIter == mListeners.end()) {
			return;
		}

		auto& listenerQueue = queueIter->second;

		auto function = std::bind(functionPointer, target, std::placeholders::_1);
		Listener::Ptr listener = Listener::create();
		listener->mTarget = target;
		listener->mFuncionPointerDescriptor = Listener::buildFunctionPointer(functionPointer);
		listener->mFunction = function;

		auto listenerIter = std::find_if(listenerQueue.begin(), listenerQueue.end(),
			[&](const Listener::Ptr& l) {
				return EventDispatcher::listenerIsEqual(listener, l);
			});
		if (listenerIter != listenerQueue.end()) {
			listenerQueue.erase(listenerIter);
		}
	}

}

