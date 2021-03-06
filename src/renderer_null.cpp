/*
 * Copyright 2011-2014 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include "bgfx_p.h"

#if BGFX_CONFIG_RENDERER_NULL

namespace bgfx
{
	void ConstantBuffer::commit()
	{
	}

	void TextVideoMemBlitter::setup()
	{
	}

	void TextVideoMemBlitter::render(uint32_t /*_numIndices*/)
	{
	}

	void Context::rendererFlip()
	{
	}

	void Context::rendererInit()
	{
	}

	void Context::rendererShutdown()
	{
	}

	void Context::rendererCreateIndexBuffer(IndexBufferHandle /*_handle*/, Memory* /*_mem*/)
	{
	}

	void Context::rendererDestroyIndexBuffer(IndexBufferHandle /*_handle*/)
	{
	}

	void Context::rendererCreateVertexDecl(VertexDeclHandle /*_handle*/, const VertexDecl& /*_decl*/)
	{
	}

	void Context::rendererDestroyVertexDecl(VertexDeclHandle /*_handle*/)
	{
	}

	void Context::rendererCreateVertexBuffer(VertexBufferHandle /*_handle*/, Memory* /*_mem*/, VertexDeclHandle /*_declHandle*/)
	{
	}

	void Context::rendererDestroyVertexBuffer(VertexBufferHandle /*_handle*/)
	{
	}

	void Context::rendererCreateDynamicIndexBuffer(IndexBufferHandle /*_handle*/, uint32_t /*_size*/)
	{
	}

	void Context::rendererUpdateDynamicIndexBuffer(IndexBufferHandle /*_handle*/, uint32_t /*_offset*/, uint32_t /*_size*/, Memory* /*_mem*/)
	{
	}

	void Context::rendererDestroyDynamicIndexBuffer(IndexBufferHandle /*_handle*/)
	{
	}

	void Context::rendererCreateDynamicVertexBuffer(VertexBufferHandle /*_handle*/, uint32_t /*_size*/)
	{
	}

	void Context::rendererUpdateDynamicVertexBuffer(VertexBufferHandle /*_handle*/, uint32_t /*_offset*/, uint32_t /*_size*/, Memory* /*_mem*/)
	{
	}

	void Context::rendererDestroyDynamicVertexBuffer(VertexBufferHandle /*_handle*/)
	{
	}

	void Context::rendererCreateShader(ShaderHandle /*_handle*/, Memory* /*_mem*/)
	{
	}

	void Context::rendererDestroyShader(ShaderHandle /*_handle*/)
	{
	}

	void Context::rendererCreateProgram(ProgramHandle /*_handle*/, ShaderHandle /*_vsh*/, ShaderHandle /*_fsh*/)
	{
	}

	void Context::rendererDestroyProgram(ProgramHandle /*_handle*/)
	{
	}

	void Context::rendererCreateTexture(TextureHandle /*_handle*/, Memory* /*_mem*/, uint32_t /*_flags*/, uint8_t /*_skip*/)
	{
	}

	void Context::rendererUpdateTextureBegin(TextureHandle /*_handle*/, uint8_t /*_side*/, uint8_t /*_mip*/)
	{
	}

	void Context::rendererUpdateTexture(TextureHandle /*_handle*/, uint8_t /*_side*/, uint8_t /*_mip*/, const Rect& /*_rect*/, uint16_t /*_z*/, uint16_t /*_depth*/, uint16_t /*_pitch*/, const Memory* /*_mem*/)
	{
	}

	void Context::rendererUpdateTextureEnd()
	{
	}

	void Context::rendererDestroyTexture(TextureHandle /*_handle*/)
	{
	}

	void Context::rendererCreateFrameBuffer(FrameBufferHandle /*_handle*/, uint8_t /*_num*/, const TextureHandle* /*_textureHandles*/)
	{
	}

	void Context::rendererDestroyFrameBuffer(FrameBufferHandle /*_handle*/)
	{
	}

	void Context::rendererCreateUniform(UniformHandle /*_handle*/, UniformType::Enum /*_type*/, uint16_t /*_num*/, const char* /*_name*/)
	{
	}

	void Context::rendererDestroyUniform(UniformHandle /*_handle*/)
	{
	}

	void Context::rendererSaveScreenShot(const char* /*_filePath*/)
	{
	}

	void Context::rendererUpdateViewName(uint8_t /*_id*/, const char* /*_name*/)
	{
	}

	void Context::rendererUpdateUniform(uint16_t /*_loc*/, const void* /*_data*/, uint32_t /*_size*/)
	{
	}

	void Context::rendererSetMarker(const char* /*_marker*/, uint32_t /*_size*/)
	{
	}

	void Context::rendererSubmit()
	{
	}
}

#endif // BGFX_CONFIG_RENDERER_NULL
