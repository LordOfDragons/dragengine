/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

deoglDebugMemoryConsumption::deoglDebugMemoryConsumption( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pLastTotal( 0 ){
}

deoglDebugMemoryConsumption::~deoglDebugMemoryConsumption(){
}



// Management
///////////////

void deoglDebugMemoryConsumption::Update(){
	const unsigned long long consumption = pRenderThread.GetMemoryManager().GetConsumption().Total();
	if( consumption == pLastTotal ){
		return;
	}
	
	pLastTotal = consumption;
	LogConsumption();
}

void deoglDebugMemoryConsumption::LogConsumption(){
	const deoglMemoryConsumption &consumption = pRenderThread.GetMemoryManager().GetConsumption();
	
	pRenderThread.GetLogger().LogInfo( "Memory Consumption:" );
	
	pRenderThread.GetLogger().LogInfoFormat( "Total: %lluMB (%lluB)", pLastTotal / 1024000ull, pLastTotal );
	
	pLog( consumption.texture1D );
	pLog( consumption.texture2D );
	pLog( consumption.texture3D );
	pLog( consumption.textureArray );
	pLog( consumption.textureCube );
	
	pLog( consumption.texture1DRenderable );
	pLog( consumption.texture2DRenderable );
	pLog( consumption.texture3DRenderable );
	pLog( consumption.textureArrayRenderable );
	pLog( consumption.textureCubeRenderable );
	
	pLog( consumption.renderbuffer );
	pLog( consumption.skin );
	pLog( consumption.bufferObject );
	pLog( consumption.deferredRendering );
	pLog( consumption.shadow );
}



// Private Functions
//////////////////////

void deoglDebugMemoryConsumption::pLog( const deoglMemoryConsumptionGPU &consumption ){
	if( consumption.GetConsumption() == 0 ){
		return;
	}
	
	pRenderThread.GetLogger().LogInfoFormat( "- %s: %uMB (%dx, %lluB)",
		consumption.GetName().GetString(), consumption.GetConsumptionMB(),
		consumption.GetCount(), consumption.GetConsumption() );
}

void deoglDebugMemoryConsumption::pLog( const deoglMemoryConsumptionTexture &consumption ){
	pLog( consumption.all );
	pLog( consumption.allCompressed );
	pLog( consumption.allUncompressed );
	
	pLog( consumption.color );
	pLog( consumption.colorCompressed );
	pLog( consumption.colorUncompressed );
	
	pLog( consumption.depth );
	pLog( consumption.depthCompressed );
	pLog( consumption.depthUncompressed );
}

void deoglDebugMemoryConsumption::pLog( const deoglMemoryConsumptionRenderBuffer &consumption ){
	pLog( consumption.all );
	pLog( consumption.color );
	pLog( consumption.depth );
}

void deoglDebugMemoryConsumption::pLog( const deoglMemoryConsumptionSkin &consumption ){
	pLog( consumption.all );
	pLog( consumption.allCompressed );
	pLog( consumption.allUncompressed );
}

void deoglDebugMemoryConsumption::pLog( const deoglMemoryConsumptionBufferObject &consumption ){
	pLog( consumption.all );
	pLog( consumption.vbo );
	pLog( consumption.vboShared );
	pLog( consumption.ibo );
	pLog( consumption.iboShared );
	pLog( consumption.tbo );
	pLog( consumption.ubo );
	pLog( consumption.ssbo );
}

void deoglDebugMemoryConsumption::pLog( const deoglMemoryConsumptionDeferredRendering &consumption ){
	pLog( consumption.target );
	pLog( consumption.texture );
	pLog( consumption.renderBuffer );
}

void deoglDebugMemoryConsumption::pLog( const deoglMemoryConsumptionShadow &consumption ){
	pLog( consumption.all );
	pLog( consumption.allStatic );
	pLog( consumption.allDynamic );
	
	pLog( consumption.skyStatic );
	pLog( consumption.skyDynamic );
	
	pLog( consumption.solidStaticMap );
	pLog( consumption.solidStaticCube );
	pLog( consumption.solidStaticArray );
	pLog( consumption.solidDynamicMap );
	pLog( consumption.solidDynamicCube );
	pLog( consumption.solidDynamicArray );
	
	pLog( consumption.transparentStaticMapDepth );
	pLog( consumption.transparentStaticMapColor );
	pLog( consumption.transparentStaticCubeDepth );
	pLog( consumption.transparentStaticCubeColor );
	pLog( consumption.transparentDynamicMapDepth );
	pLog( consumption.transparentDynamicMapColor );
	pLog( consumption.transparentDynamicCubeDepth );
	pLog( consumption.transparentDynamicCubeColor );
	
	pLog( consumption.ambientStaticMap );
	pLog( consumption.ambientStaticCube );
	pLog( consumption.ambientDynamicMap );
	pLog( consumption.ambientDynamicCube );
}
