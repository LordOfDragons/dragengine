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

#include <stdlib.h>

#include "deoglDebugMemoryConsumption.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>


// Class deoglDebugMemoryConsumption
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugMemoryConsumption::deoglDebugMemoryConsumption(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pLastTotal(0){
}

deoglDebugMemoryConsumption::~deoglDebugMemoryConsumption(){
}



// Management
///////////////

void deoglDebugMemoryConsumption::Update(){
	const unsigned long long consumption = pRenderThread.GetMemoryManager().GetConsumption().Total();
	if(consumption == pLastTotal){
		return;
	}
	
	pLastTotal = consumption;
	LogConsumption();
}

void deoglDebugMemoryConsumption::LogConsumption(){
	const deoglMemoryConsumption &consumption = pRenderThread.GetMemoryManager().GetConsumption();
	
	pRenderThread.GetLogger().LogInfo("Memory Consumption:");
	
	pRenderThread.GetLogger().LogInfoFormat("Total: %" OGLPFLLU "MB (%" OGLPFLLU "B)", pLastTotal / 1024000ull, pLastTotal);
	
	pLog(consumption.texture2D);
	pLog(consumption.texture3D);
	pLog(consumption.textureArray);
	pLog(consumption.textureCube);
	
	pLog(consumption.texture2DRenderable);
	pLog(consumption.texture3DRenderable);
	pLog(consumption.textureArrayRenderable);
	pLog(consumption.textureCubeRenderable);
	
	pLog(consumption.skin);
	pLog(consumption.bufferObject);
	pLog(consumption.deferredRendering);
	pLog(consumption.shadow);
}



// Private Functions
//////////////////////

void deoglDebugMemoryConsumption::pLog(const deoglMemoryConsumptionGPU &consumption){
	if(consumption.GetConsumption() == 0){
		return;
	}
	
	pRenderThread.GetLogger().LogInfoFormat("- %s: %uMB (%dx, %" OGLPFLLU "B)",
		consumption.GetName().GetString(), consumption.GetConsumptionMB(),
		consumption.GetCount(), consumption.GetConsumption());
}

void deoglDebugMemoryConsumption::pLog(const deoglMemoryConsumptionTexture &consumption){
	pLog(consumption.all);
	pLog(consumption.allCompressed);
	pLog(consumption.allUncompressed);
	
	pLog(consumption.color);
	pLog(consumption.colorCompressed);
	pLog(consumption.colorUncompressed);
	
	pLog(consumption.depth);
	pLog(consumption.depthCompressed);
	pLog(consumption.depthUncompressed);
}

void deoglDebugMemoryConsumption::pLog(const deoglMemoryConsumptionSkin &consumption){
	pLog(consumption.all);
	pLog(consumption.allCompressed);
	pLog(consumption.allUncompressed);
}

void deoglDebugMemoryConsumption::pLog(const deoglMemoryConsumptionBufferObject &consumption){
	pLog(consumption.all);
	pLog(consumption.vbo);
	pLog(consumption.vboShared);
	pLog(consumption.ibo);
	pLog(consumption.iboShared);
	pLog(consumption.tbo);
	pLog(consumption.ubo);
	pLog(consumption.ssbo);
}

void deoglDebugMemoryConsumption::pLog(const deoglMemoryConsumptionDeferredRendering &consumption){
	pLog(consumption.target);
	pLog(consumption.texture);
}

void deoglDebugMemoryConsumption::pLog(const deoglMemoryConsumptionShadow &consumption){
	pLog(consumption.all);
	pLog(consumption.allStatic);
	pLog(consumption.allDynamic);
	
	pLog(consumption.skyStatic);
	pLog(consumption.skyDynamic);
	
	pLog(consumption.solidStaticMap);
	pLog(consumption.solidStaticCube);
	pLog(consumption.solidStaticArray);
	pLog(consumption.solidDynamicMap);
	pLog(consumption.solidDynamicCube);
	pLog(consumption.solidDynamicArray);
	
	pLog(consumption.transparentStaticMapDepth);
	pLog(consumption.transparentStaticMapColor);
	pLog(consumption.transparentStaticCubeDepth);
	pLog(consumption.transparentStaticCubeColor);
	pLog(consumption.transparentDynamicMapDepth);
	pLog(consumption.transparentDynamicMapColor);
	pLog(consumption.transparentDynamicCubeDepth);
	pLog(consumption.transparentDynamicCubeColor);
	
	pLog(consumption.ambientStaticMap);
	pLog(consumption.ambientStaticCube);
	pLog(consumption.ambientDynamicMap);
	pLog(consumption.ambientDynamicCube);
}
