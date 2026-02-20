/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <string.h>

#include "deoglPipelineState.h"
#include "../extensions/deoglExtensions.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglPipelineState
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPipelineState::deoglPipelineState(deoglRenderThread &renderThread) :
pRenderThread(renderThread)
{
	Reset();
}



// Management
///////////////

#define ENABLE_GL_STATE(flag,state) \
	if(flag){ \
		OGL_CHECK(pRenderThread, glEnable(state)); \
	} else { \
		OGL_CHECK(pRenderThread, glDisable(state)); \
	}

void deoglPipelineState::Reset(){
	pColorMask[0] = true;
	pColorMask[1] = true;
	pColorMask[2] = true;
	pColorMask[3] = true;
	pDepthMask = true;
	pEnableScissorTest = false;
	pPolygonMode = GL_FILL;
	pEnableCullFace = false;
	pCullFace = GL_BACK;
	pEnablePolygonOffsetFill = false;
	pPolygonOffsetFactor = 0.0f;
	pPolygonOffsetBias = 0.0f;
	pEnableDepthTest = false;
	pDepthFunc = GL_LESS;
	pEnableStencilTest = false;
	pStencilOpFailFront = GL_KEEP;
	pStencilOpZFailFront = GL_KEEP;
	pStencilOpZPassFront = GL_KEEP;
	pStencilOpFailBack = GL_KEEP;
	pStencilOpZFailBack = GL_KEEP;
	pStencilOpZPassBack = GL_KEEP;
	pStencilFunc = GL_ALWAYS;
	pStencilRef = 0;
	pStencilRefMask = ~0;
	pStencilMask = ~0;
	pEnableBlend = false;
	pBlendColor[0] = 0.0f;
	pBlendColor[1] = 0.0f;
	pBlendColor[2] = 0.0f;
	pBlendColor[3] = 0.0f;
	pBlendFuncSource = GL_ONE;
	pBlendFuncDest = GL_ZERO;
	pClipControl = false;
	pEnableDepthClamp = false;
	
	OGL_CHECK(pRenderThread, glColorMask(pColorMask[0], pColorMask[1], pColorMask[2], pColorMask[3]));
	OGL_CHECK(pRenderThread, glDepthMask(pDepthMask));
	ENABLE_GL_STATE(pEnableScissorTest, GL_SCISSOR_TEST)
	OGL_CHECK(pRenderThread, glPolygonMode(GL_FRONT_AND_BACK, pPolygonMode));
	ENABLE_GL_STATE(pEnableCullFace, GL_CULL_FACE)
	OGL_CHECK(pRenderThread, glCullFace(pCullFace));
	ENABLE_GL_STATE(pEnablePolygonOffsetFill, GL_POLYGON_OFFSET_FILL)
	OGL_CHECK(pRenderThread, pglPolygonOffset(pPolygonOffsetFactor, pPolygonOffsetBias));
	ENABLE_GL_STATE(pEnableDepthTest, GL_DEPTH_TEST)
	OGL_CHECK(pRenderThread, glDepthFunc(pDepthFunc));
	ENABLE_GL_STATE(pEnableStencilTest, GL_STENCIL_TEST)
	OGL_CHECK(pRenderThread, glStencilOp(pStencilOpFailFront, pStencilOpZFailFront, pStencilOpZPassFront));
	OGL_CHECK(pRenderThread, glStencilFunc(pStencilFunc, pStencilRef, pStencilRefMask));
	OGL_CHECK(pRenderThread, glStencilMask(pStencilMask));
	ENABLE_GL_STATE(pEnableBlend, GL_BLEND)
	OGL_CHECK(pRenderThread, pglBlendColor(pBlendColor[0], pBlendColor[1], pBlendColor[2], pBlendColor[3]));
	OGL_CHECK(pRenderThread, glBlendFunc(pBlendFuncSource, pBlendFuncDest));
	if(pglClipControl){
		pglClipControl(GL_LOWER_LEFT, pClipControl ? GL_ZERO_TO_ONE : GL_NEGATIVE_ONE_TO_ONE);
	}
	if(pRenderThread.GetExtensions().GetHasDepthClamp()){
		ENABLE_GL_STATE(pEnableDepthClamp, GL_DEPTH_CLAMP)
	}
}

void deoglPipelineState::ColorMask(bool red, bool green, bool blue, bool alpha){
	if(red == pColorMask[0] && green == pColorMask[1] && blue == pColorMask[2] && alpha == pColorMask[3]){
		return;
	}
	
	pColorMask[0] = red;
	pColorMask[1] = green;
	pColorMask[2] = blue;
	pColorMask[3] = alpha;
	
	OGL_CHECK(pRenderThread, glColorMask(red, green, blue, alpha));
}

void deoglPipelineState::DepthMask(bool mask){
	if(mask == pDepthMask){
		return;
	}
	
	pDepthMask = mask;
	
	OGL_CHECK(pRenderThread, glDepthMask(mask));
}

void deoglPipelineState::EnableScissorTest(bool enable){
	if(enable == pEnableScissorTest){
		return;
	}
	
	pEnableScissorTest = enable;
	
	ENABLE_GL_STATE(enable, GL_SCISSOR_TEST)
}

void deoglPipelineState::PolygonMode(GLenum mode){
	if(mode == pPolygonMode){
		return;
	}
	
	pPolygonMode = mode;
	
	OGL_CHECK(pRenderThread, glPolygonMode(GL_FRONT_AND_BACK, mode));
}

void deoglPipelineState::EnableCullFace(bool enable){
	if(enable == pEnableCullFace){
		return;
	}
	
	pEnableCullFace = enable;
	
	ENABLE_GL_STATE(enable, GL_CULL_FACE)
}

void deoglPipelineState::CullFace(GLenum cullFace){
	if(cullFace == pCullFace){
		return;
	}
	
	pCullFace = cullFace;
	
	OGL_CHECK(pRenderThread, glCullFace(cullFace));
}

void deoglPipelineState::EnablePolygonOffsetFill(bool enable){
	if(enable == pEnablePolygonOffsetFill){
		return;
	}
	
	pEnablePolygonOffsetFill = enable;
	
	ENABLE_GL_STATE(enable, GL_POLYGON_OFFSET_FILL)
}

void deoglPipelineState::PolygonOffset(float factor, float bias){
	if(factor == pPolygonOffsetFactor && bias == pPolygonOffsetBias){
		return;
	}
	
	pPolygonOffsetFactor = factor;
	pPolygonOffsetBias = bias;
	
	OGL_CHECK(pRenderThread, pglPolygonOffset(factor, bias));
}

void deoglPipelineState::EnableDepthTest(bool enable){
	if(enable == pEnableDepthTest){
		return;
	}
	
	pEnableDepthTest = enable;
	
	ENABLE_GL_STATE(enable, GL_DEPTH_TEST)
}

void deoglPipelineState::DepthFunc(GLenum depthFunc){
	if(depthFunc == pDepthFunc){
		return;
	}
	
	pDepthFunc = depthFunc;
	
	OGL_CHECK(pRenderThread, glDepthFunc(depthFunc));
}

void deoglPipelineState::EnableStencilTest(bool enable){
	if(enable == pEnableStencilTest){
		return;
	}
	
	pEnableStencilTest = enable;
	
	ENABLE_GL_STATE(enable, GL_STENCIL_TEST)
}

void deoglPipelineState::StencilOp(GLenum fail, GLenum zfail, GLenum zpass){
	if(fail == pStencilOpFailFront && zfail == pStencilOpZFailFront && zpass == pStencilOpZPassFront
	&& fail == pStencilOpFailBack && zfail == pStencilOpZFailBack && zpass == pStencilOpZPassBack){
		return;
	}
	
	pStencilOpFailFront = fail;
	pStencilOpZFailFront = zfail;
	pStencilOpZPassFront = zpass;
	pStencilOpFailBack = fail;
	pStencilOpZFailBack = zfail;
	pStencilOpZPassBack = zpass;
	
	OGL_CHECK(pRenderThread, glStencilOp(fail, zfail, zpass));
}

void deoglPipelineState::StencilOpFront(GLenum fail, GLenum zfail, GLenum zpass){
	if(fail == pStencilOpFailFront && zfail == pStencilOpZFailFront && zpass == pStencilOpZPassFront){
		return;
	}
	
	pStencilOpFailFront = fail;
	pStencilOpZFailFront = zfail;
	pStencilOpZPassFront = zpass;
	
	OGL_CHECK(pRenderThread, pglStencilOpSeparate(GL_FRONT, fail, zfail, zpass));
}

void deoglPipelineState::StencilOpBack(GLenum fail, GLenum zfail, GLenum zpass){
	if(fail == pStencilOpFailBack && zfail == pStencilOpZFailBack && zpass == pStencilOpZPassBack){
		return;
	}
	
	pStencilOpFailBack = fail;
	pStencilOpZFailBack = zfail;
	pStencilOpZPassBack = zpass;
	
	OGL_CHECK(pRenderThread, pglStencilOpSeparate(GL_FRONT, fail, zfail, zpass));
}

void deoglPipelineState::StencilFunc(GLenum func, GLint ref, GLuint mask){
	if(func == pStencilFunc && ref == pStencilRef && mask == pStencilRefMask){
		return;
	}
	
	pStencilFunc = func;
	pStencilRef = ref;
	pStencilRefMask = mask;
	
	OGL_CHECK(pRenderThread, glStencilFunc(func, ref, mask));
}

void deoglPipelineState::StencilMask(GLuint mask){
	if(mask == pStencilMask){
		return;
	}
	
	pStencilMask = mask;
	
	OGL_CHECK(pRenderThread, glStencilMask(mask));
}

void deoglPipelineState::EnableBlend(bool enable){
	if(enable == pEnableBlend){
		return;
	}
	
	pEnableBlend = enable;
	
	ENABLE_GL_STATE(enable, GL_BLEND)
}

void deoglPipelineState::BlendColor(float red, float green, float blue, float alpha){
	if(red == pBlendColor[0] && green == pBlendColor[1] && blue == pBlendColor[2] && alpha == pBlendColor[3]){
		return;
	}
	
	pBlendColor[0] = red;
	pBlendColor[1] = green;
	pBlendColor[2] = blue;
	pBlendColor[3] = alpha;
	
	OGL_CHECK(pRenderThread, pglBlendColor(red, green, blue, alpha));
}

void deoglPipelineState::BlendFunc(GLenum sfactor, GLenum dfactor){
	if(sfactor == pBlendFuncSource && dfactor == pBlendFuncDest){
		return;
	}
	
	pBlendFuncSource = sfactor;
	pBlendFuncDest = dfactor;
	
	OGL_CHECK(pRenderThread, glBlendFunc(sfactor, dfactor));
}

void deoglPipelineState::ClipControl(bool clipControl){
	if(clipControl == pClipControl){
		return;
	}
	
	pClipControl = clipControl;
	
	if(pglClipControl){
		pglClipControl(GL_LOWER_LEFT, clipControl ? GL_ZERO_TO_ONE : GL_NEGATIVE_ONE_TO_ONE);
	}
}

void deoglPipelineState::EnableDepthClamp(bool enable){
	if(enable == pEnableDepthClamp){
		return;
	}
	
	pEnableDepthClamp = enable;
	
	if(pRenderThread.GetExtensions().GetHasDepthClamp()){
		ENABLE_GL_STATE(enable, GL_DEPTH_CLAMP)
	}
}
