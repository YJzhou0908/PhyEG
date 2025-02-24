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
			//mFunction是已经bind好target跟function的函数体,可以直接执行
			listener->mFunction(event);
		}
	}

	template<typename T>
	void EventDispatcher::addEventListener(const std::string& name, T* target, TypedFunctionPointer<T> functionPointer) noexcept {
		auto queueIter = mListeners.find(name);

		if (queueIter == mListeners.end()) {
			queueIter = (mListeners.insert(std::make_pair(name, ListenerQueue()))).first;
		}

		// 使用引用，确保操作的是一个vector
		auto& listenerQueue = queueIter->second;

		
		// 找到target下的跟functionPointer类型一样的函数，传给function
		auto function = std::bind(functionPointer, target, std::placeholders::_1);

		// 构建Listener
		Listener::Ptr listener = Listener::create();
		listener->mTarget = target;

		listener->mFuncionPointerDescriptor = Listener::buildFunctionPointer(functionPointer);
		listener->mFunction = function;

		// 检查是否被重新插入
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

