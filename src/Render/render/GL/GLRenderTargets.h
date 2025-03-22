#pragma once
#include "../../basic/common.h"
#include "../renderTarget.h"
#include "../../basic/eventDispatcher.h"

namespace pe {

	class GLRenderTarget {
	public:
		using Ptr = std::shared_ptr<GLRenderTarget>;
		static Ptr create() { return std::make_shared<GLRenderTarget>(); }

		GLRenderTarget() noexcept;

		~GLRenderTarget() noexcept;

		void generateFrameBuffer() noexcept;

	public:
		GLuint	mFrameBuffer{ 0 };

		//如果用户没有在外部生成DepthTexture，说明用户不想回读渲染后的深度数据
		//对于用户自定义的DepthTexture，管理权限归于DriverTextures
		// 
		//所以必须自己生成一个RenderBuffer，类型就是RenderBuffer类型，这个类型与Texture类型类似，也可以
		// 加入到mFrameBuffer当中作为一个附件
		// 
		// RenderBuffer其实就是一个数组，不用编码任何Texture的格式，在OpenGL管线当中会更加有效率
		//
		GLuint	mDepthRenderBuffer{ 0 };
	};

	//对于RenderTarget，其本质还是Texture相关的内容，只不过通过DriverRenderTarget生成FrameBufferObject，对于其
	//ColorAttachment or DepthAttachment的解析，仍然处于Texture相关的解析当中，所以解析RenderTarget的任务就交给了DriverTextures
	class GLRenderTargets {
	public:
		using Ptr = std::shared_ptr<GLRenderTargets>;
		static Ptr create() { return std::make_shared<GLRenderTargets>(); }

		GLRenderTargets() noexcept;

		~GLRenderTargets() noexcept;

		GLRenderTarget::Ptr get(const RenderTarget::Ptr& renderTarget) noexcept;

		void onRenderTargetDispose(const EventBase::Ptr& e);

	private:
		std::unordered_map<ID, GLRenderTarget::Ptr> mRenderTargets{};
	};
}
