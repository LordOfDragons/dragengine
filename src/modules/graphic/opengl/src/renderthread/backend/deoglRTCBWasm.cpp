/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <dragengine/dragengine_configuration.h>

#ifdef OS_WEBWASM

#include "deoglRTCBWasm.h"

#include <dragengine/app/deOSWebWasm.h>

#include "../deoglRTContext.h"
#include "../deoglRenderThread.h"
#include "../deoglRTLogger.h"
#include "../../deGraphicOpenGl.h"
#include "../../extensions/wasmfix.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/resources/rendering/deRenderWindow.h>


// Class deoglRTCBWasm
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTCBWasm::deoglRTCBWasm(deoglRTContext &context) :
deoglRTCBackend(context),
pOSWebWasm(context.GetRenderThread().GetOgl().GetOS()->CastToOSWebWasm()),
pContext(0),
pLoaderContext(0),
pScreenWidth(0),
pScreenHeight(0)
{
	int i;
	for(i=0; i<MaxCompileContextCount; i++){
		pCompileContext[i] = 0;
	}
}

deoglRTCBWasm::~deoglRTCBWasm() = default;


// Management
///////////////

void deoglRTCBWasm::InitPhase2(){
	pCreateContext();
}

void deoglRTCBWasm::CleanUp(){
	pFreeContext();
}

void deoglRTCBWasm::DropCompileContexts(int count){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	int i;
	for(i=count; i<pCompileContextCount; i++){
		logger.LogInfoFormat("Drop compile context %d", i);
		if(pCompileContext[i]){
			emscripten_webgl_destroy_context(pCompileContext[i]);
			pCompileContext[i] = 0;
		}
	}
	pCompileContextCount = count;
}

void deoglRTCBWasm::ActivateContext(deoglRRenderWindow&){
	OGL_WASM_CHECK(pRTContext.GetRenderThread(),
		emscripten_webgl_make_context_current(pContext));
}

void deoglRTCBWasm::DeactivateContext(){
	OGL_WASM_CHECK(pRTContext.GetRenderThread(),
		emscripten_webgl_make_context_current(0));
}

bool deoglRTCBWasm::HasLoaderContext() const{
	return pLoaderContext != 0;
}

void deoglRTCBWasm::ActivateLoaderContext(){
	DEASSERT_NOTNULL(pLoaderContext)
	
	OGL_WASM_CHECK(pRTContext.GetRenderThread(),
		emscripten_webgl_make_context_current(pLoaderContext));
}

void deoglRTCBWasm::DeactivateLoaderContext(){
	OGL_WASM_CHECK(pRTContext.GetRenderThread(), emscripten_webgl_make_context_current(0));
}

void deoglRTCBWasm::ActivateCompileContext(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < MaxCompileContextCount)
	DEASSERT_NOTNULL(pCompileContext[index])
	
	OGL_WASM_CHECK(pRTContext.GetRenderThread(),
		emscripten_webgl_make_context_current(pCompileContext[index]));
}

void deoglRTCBWasm::DeactivateCompileContext(int){
	OGL_WASM_CHECK(pRTContext.GetRenderThread(), emscripten_webgl_make_context_current(0));
}

void deoglRTCBWasm::SwapBuffers(deoglRRenderWindow&){
	DEASSERT_TRUE(emscripten_webgl_commit_frame() == EMSCRIPTEN_RESULT_SUCCESS)
}

void *deoglRTCBWasm::GetFunctionPointer(const char *funcName){
	return (void*)emscripten_webgl_get_proc_address(funcName);
}


void deoglRTCBWasm::InitAppWindow(){
}

void deoglRTCBWasm::TerminateAppWindow(){
}

void deoglRTCBWasm::CheckConfigurationChanged(){
	const decBoundary &contentRect = pOSWebWasm->GetContentRect();
	pScreenWidth = contentRect.x2 - contentRect.x1;
	pScreenHeight = contentRect.y2 - contentRect.y1;
	
	OGL_WASM_CHECK(pRTContext.GetRenderThread(),
		emscripten_webgl_get_drawing_buffer_size(pContext, &pScreenWidth, &pScreenHeight));
}

EMSCRIPTEN_WEBGL_CONTEXT_HANDLE deoglRTCBWasm::GetLoaderContext(){
	if(!pLoaderContext){
		EmscriptenWebGLContextAttributes attrs{};
		emscripten_webgl_init_context_attributes(&attrs);
		attrs.alpha = false;
		attrs.depth = false;
		attrs.stencil = false;
		attrs.antialias = false;
		attrs.premultipliedAlpha = false;
		attrs.preserveDrawingBuffer = false;
		attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_LOW_POWER;
		attrs.failIfMajorPerformanceCaveat = false;
		attrs.majorVersion = 2;
		attrs.minorVersion = 0;
		attrs.enableExtensionsByDefault = false;
		attrs.explicitSwapControl = true;
		attrs.renderViaOffscreenBackBuffer = false;
		
		pLoaderContext = emscripten_webgl_create_context(pOSWebWasm->GetCanvasId(), &attrs);
		DEASSERT_NOTNULL(pLoaderContext)
	}
	return pLoaderContext;
}

EMSCRIPTEN_WEBGL_CONTEXT_HANDLE deoglRTCBWasm::GetCompileContextAt(int index){
	if(!pCompileContext[index]){
		EmscriptenWebGLContextAttributes attrs{};
		emscripten_webgl_init_context_attributes(&attrs);
		attrs.alpha = false;
		attrs.depth = false;
		attrs.stencil = false;
		attrs.antialias = false;
		attrs.premultipliedAlpha = false;
		attrs.preserveDrawingBuffer = false;
		attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_LOW_POWER;
		attrs.failIfMajorPerformanceCaveat = false;
		attrs.majorVersion = 2;
		attrs.minorVersion = 0;
		attrs.enableExtensionsByDefault = false;
		attrs.explicitSwapControl = true;
		attrs.renderViaOffscreenBackBuffer = false;
		
		pCompileContext[index] = emscripten_webgl_create_context(pOSWebWasm->GetCanvasId(), &attrs);
		DEASSERT_NOTNULL(pCompileContext[index])
	}
	return pCompileContext[index];
}


// Protected Functions
////////////////////////

void deoglRTCBWasm::pCreateContext(){
	pRTContext.GetRenderThread().GetLogger().LogInfoFormat("RTContext CreateContext %lu", pContext);
	if(pContext){
		return;
	}
	
	pCompileContextCount = decMath::min(MaxCompileContextCount,
		pRTContext.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing().GetCoreCount());
	
	EmscriptenWebGLContextAttributes attrs{};
	emscripten_webgl_init_context_attributes(&attrs);
	attrs.alpha = false;
	attrs.depth = true;
	attrs.stencil = true;
	attrs.antialias = false;
	attrs.premultipliedAlpha = false;
	attrs.preserveDrawingBuffer = false;
	attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_HIGH_PERFORMANCE;
	attrs.failIfMajorPerformanceCaveat = false;
	attrs.majorVersion = 2;
	attrs.minorVersion = 0;
	attrs.enableExtensionsByDefault = false;
	attrs.explicitSwapControl = true;
	attrs.renderViaOffscreenBackBuffer = true;
	
	const char * const canvasId = pOSWebWasm->GetCanvasId();
	pRTContext.GetRenderThread().GetLogger().LogInfoFormat("Create context using id '%s'", canvasId);
	pContext = emscripten_webgl_create_context(canvasId, &attrs);
	DEASSERT_NOTNULL(pContext)
	
	// loader and compiler contexts have to be created by the thread using them
	
	OGL_WASM_CHECK(pRTContext.GetRenderThread(), emscripten_webgl_make_context_current(pContext));
	
	const decPoint &canvasSize = pOSWebWasm->GetCanvasSize();
	pScreenWidth = canvasSize.x;
	pScreenHeight = canvasSize.y;
	
	pRTContext.GetRenderThread().GetLogger().LogInfoFormat("Display size %ix%i", pScreenWidth, pScreenHeight);
}

void deoglRTCBWasm::pFreeContext(){
	int i;
	for(i=0; i<pCompileContextCount; i++){
		if(pCompileContext[i]){
			emscripten_webgl_destroy_context(pCompileContext[i]);
			pCompileContext[i] = 0;
		}
	}
	
	if(pLoaderContext){
		emscripten_webgl_destroy_context(pLoaderContext);
		pLoaderContext = 0;
	}
	
	if(pContext){
		emscripten_webgl_destroy_context(pContext);
		pContext = 0;
	}
}

#endif // OS_WEBWASM
