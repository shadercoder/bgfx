/*
 * Copyright 2011-2013 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include "common.h"

#include <bgfx.h>
#include <bx/timer.h>
#include <bx/uint32_t.h>
#include "fpumath.h"
#include "imgui/imgui.h"

#include <stdio.h>
#include <string.h>

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;
};

static bgfx::VertexDecl s_PosColorDecl;

static PosColorVertex s_cubeVertices[8] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t s_cubeIndices[36] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

static const char* s_shaderPath = NULL;

static void shaderFilePath(char* _out, const char* _name)
{
	strcpy(_out, s_shaderPath);
	strcat(_out, _name);
	strcat(_out, ".bin");
}

long int fsize(FILE* _file)
{
	long int pos = ftell(_file);
	fseek(_file, 0L, SEEK_END);
	long int size = ftell(_file);
	fseek(_file, pos, SEEK_SET);
	return size;
}

static const bgfx::Memory* load(const char* _filePath)
{
	FILE* file = fopen(_filePath, "rb");
	if (NULL != file)
	{
		uint32_t size = (uint32_t)fsize(file);
		const bgfx::Memory* mem = bgfx::alloc(size+1);
		size_t ignore = fread(mem->data, 1, size, file);
		BX_UNUSED(ignore);
		fclose(file);
		mem->data[mem->size-1] = '\0';
		return mem;
	}

	return NULL;
}

static const bgfx::Memory* loadShader(const char* _name)
{
	char filePath[512];
	shaderFilePath(filePath, _name);
	return load(filePath);
}

int _main_(int /*_argc*/, char** /*_argv*/)
{
	uint32_t width = 1280;
	uint32_t height = 720;
	uint32_t debug = BGFX_DEBUG_TEXT;
	uint32_t reset = BGFX_RESET_NONE;

	bgfx::init();
	bgfx::reset(width, height, reset);

	// Enable debug text.
	bgfx::setDebug(debug);

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR_BIT|BGFX_CLEAR_DEPTH_BIT
		, 0x303030ff
		, 1.0f
		, 0
		);

	// Setup root path for binary shaders. Shader binaries are different 
	// for each renderer.
	switch (bgfx::getRendererType() )
	{
	default:
	case bgfx::RendererType::Direct3D9:
		s_shaderPath = "shaders/dx9/";
		break;

	case bgfx::RendererType::Direct3D11:
		s_shaderPath = "shaders/dx11/";
		break;

	case bgfx::RendererType::OpenGL:
		s_shaderPath = "shaders/glsl/";
		break;

	case bgfx::RendererType::OpenGLES2:
	case bgfx::RendererType::OpenGLES3:
		s_shaderPath = "shaders/gles/";
		break;
	}

	// Create vertex stream declaration.
	s_PosColorDecl.begin();
	s_PosColorDecl.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
	s_PosColorDecl.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true);
	s_PosColorDecl.end();

	const bgfx::Memory* mem;

	// Create static vertex buffer.
	mem = bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices) );
	bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(mem, s_PosColorDecl);

	// Create static index buffer.
	mem = bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices) );
	bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(mem);

	// Load vertex shader.
	mem = loadShader("vs_drawstress");
	bgfx::VertexShaderHandle vsh = bgfx::createVertexShader(mem);

	// Load fragment shader.
	mem = loadShader("fs_drawstress");
	bgfx::FragmentShaderHandle fsh = bgfx::createFragmentShader(mem);

	// Create program from shaders.
	bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh);

	// We can destroy vertex and fragment shader here since
	// their reference is kept inside bgfx after calling createProgram.
	// Vertex and fragment shader will be destroyed once program is
	// destroyed.
	bgfx::destroyVertexShader(vsh);
	bgfx::destroyFragmentShader(fsh);

	FILE* file = fopen("font/droidsans.ttf", "rb");
	uint32_t size = (uint32_t)fsize(file);
	void* data = malloc(size);
	size_t ignore = fread(data, 1, size, file);
	BX_UNUSED(ignore);
	fclose(file);

	imguiCreate(data, size);

	free(data);

	bool autoAdjust = true;
	int32_t scrollArea = 0;
	int32_t dim = 16;
	uint32_t transform = 0;

	entry::MouseState mouseState;

	int64_t timeOffset = bx::getHPCounter();

	int64_t deltaTimeNs = 0;
	int64_t deltaTimeAvgNs = 0;
	int64_t numFrames = 0;

	while (!entry::processEvents(width, height, debug, reset, &mouseState) )
	{
		int64_t now = bx::getHPCounter();
		static int64_t last = now;
		const int64_t frameTime = now - last;
		last = now;
		const double freq = double(bx::getHPFrequency() );
		const double toMs = 1000.0/freq;

		deltaTimeNs += frameTime*1000000/bx::getHPFrequency();

		if (deltaTimeNs > 1000000)
		{
			deltaTimeAvgNs = deltaTimeNs / numFrames;

			if (autoAdjust)
			{
				if (deltaTimeAvgNs < 1000000/65)
				{
					dim = bx::uint32_min(dim + 2, 40);
				}
				else if (deltaTimeAvgNs > 1000000/57)
				{
					dim = bx::uint32_max(dim - 1, 5);
				}
			}

			deltaTimeNs = deltaTimeAvgNs;
			numFrames = 1;
		}
		else
		{
			++numFrames;
		}

		float time = (float)( (now-timeOffset)/freq);

		imguiBeginFrame(mouseState.m_mx
			, mouseState.m_my
			, (mouseState.m_buttons[entry::MouseButton::Left  ] ? IMGUI_MBUT_LEFT  : 0)
			| (mouseState.m_buttons[entry::MouseButton::Right ] ? IMGUI_MBUT_RIGHT : 0)
			, 0
			, width
			, height
			);

		imguiBeginScrollArea("Settings", width - width / 4 - 10, 10, width / 4, height / 3, &scrollArea);
		imguiSeparatorLine();

		transform = imguiChoose(transform
			, "Rotate"
			, "No fragments"
			);
		imguiSeparatorLine();

		if (imguiCheck("Auto adjust", autoAdjust) )
		{
			autoAdjust ^= true;
		}

		imguiSlider("Dim", &dim, 5, 40);
		imguiLabel("Draw calls: %d", dim*dim*dim);
		imguiLabel("Avg Delta Time (1 second) [ms]: %0.4f", deltaTimeAvgNs/1000.0f);

		imguiEndScrollArea();
		imguiEndFrame();

		float at[3] = { 0.0f, 0.0f, 0.0f };
		float eye[3] = { 0.0f, 0.0f, -35.0f };

		float view[16];
		float proj[16];
		mtxLookAt(view, eye, at);
		mtxProj(proj, 60.0f, float(width)/float(height), 0.1f, 100.0f);

		// Set view and projection matrix for view 0.
		bgfx::setViewTransform(0, view, proj);

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, width, height);

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::submit(0);

		// Use debug font to print information about this example.
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/xx-drawstress");
		bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Draw stress, maximizing number of draw calls.");
		bgfx::dbgTextPrintf(0, 3, 0x0f, "Frame: % 7.3f[ms]", double(frameTime)*toMs);

		float mtxS[16];
		const float scale = 0 == transform ? 0.25f : 0.0f;
		mtxScale(mtxS, scale, scale, scale);

		const float step = 0.6f;
		float pos[3];
		pos[0] = -step*dim / 2.0f;
		pos[1] = -step*dim / 2.0f;
		pos[2] = -15.0;

		for (uint32_t zz = 0; zz < uint32_t(dim); ++zz)
		{
			for (uint32_t yy = 0; yy < uint32_t(dim); ++yy)
			{
				for (uint32_t xx = 0; xx < uint32_t(dim); ++xx)
				{
					float mtxR[16];
					mtxRotateXYZ(mtxR, time + xx*0.21f, time + yy*0.37f, time + yy*0.13f);

					float mtx[16];
					mtxMul(mtx, mtxS, mtxR);

					mtx[12] = pos[0] + float(xx)*step;
					mtx[13] = pos[1] + float(yy)*step;
					mtx[14] = pos[2] + float(zz)*step;

					// Set model matrix for rendering.
					bgfx::setTransform(mtx);

					// Set vertex and fragment shaders.
					bgfx::setProgram(program);

					// Set vertex and index buffer.
					bgfx::setVertexBuffer(vbh);
					bgfx::setIndexBuffer(ibh);

					// Set render states.
					bgfx::setState(BGFX_STATE_DEFAULT);

					// Submit primitive for rendering to view 0.
					bgfx::submit(0);
				}
			}
		}

		// Advance to next frame. Rendering thread will be kicked to 
		// process submitted rendering primitives.
		bgfx::frame();
	}

	// Cleanup.
	imguiDestroy();
	bgfx::destroyIndexBuffer(ibh);
	bgfx::destroyVertexBuffer(vbh);
	bgfx::destroyProgram(program);

	// Shutdown bgfx.
	bgfx::shutdown();

	return 0;
}