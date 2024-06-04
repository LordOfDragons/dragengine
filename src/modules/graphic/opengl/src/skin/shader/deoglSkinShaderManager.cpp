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

#include "deoglSkinShaderConfig.h"
#include "deoglSkinShaderManager.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderUnitSourceCode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>



// Unit Source Code Path
//////////////////////////

static const char *vUnitSourceCodePath[ deoglSkinShaderManager::UnitSourceCodePathCount ] = {
	"vertex/defren/skin/geometry.glsl", // euscpVertexGeometry
	"vertex/defren/skin/depth.glsl", // euscpVertexDepth
	"vertex/defren/skin/particle.glsl", // euscpVertexParticle
	"vertex/defren/skin/passthrough.glsl", // euscpVertexPassThrough
	"vertex/defren/gi/materialMap.glsl", // euscpGIMaterialMap
	
	"geometry/defren/skin/geometry.glsl", // euscpGeometryGeometry
	"geometry/defren/skin/depth.glsl", // euscpGeometryDepth
	"geometry/defren/skin/particle.glsl", // euscpGeometryParticle
	"geometry/defren/skin/particle_ribbon.glsl", // euscpGeometryParticleRibbon
	
	"tessctrl/defren/skin/geometry.glsl", // euscpTessControlGeometry
	"tessctrl/defren/skin/depth.glsl", // euscpTessControlDepth
	
	"tesseval/defren/skin/geometry.glsl", // euscpTessEvalGeometry
	"tesseval/defren/skin/depth.glsl", // euscpTessEvalDepth
	
	"fragment/defren/skin/geometry.glsl", // euscpFragmentGeometry
	"fragment/defren/skin/depth.glsl", // euscpFragmentDepth
	"fragment/defren/gi/materialMap.glsl" // euscpGIMaterialMap
};



// Class deoglSkinShaderManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinShaderManager::deoglSkinShaderManager( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pMaintananceInterval( 0 ){
}

deoglSkinShaderManager::~deoglSkinShaderManager(){
}



// Management
///////////////

const char *deoglSkinShaderManager::GetUnitSourceCodePath( eUnitSourceCodePath unitSourceCodePath ) const{
	return vUnitSourceCodePath[ unitSourceCodePath ];
}



int deoglSkinShaderManager::GetShaderCount(){
	const deMutexGuard guard( pMutex );
	return pShaderList.GetCount();
}

const deoglSkinShader &deoglSkinShaderManager::GetShaderAt( int index ){
	const deMutexGuard guard( pMutex );
	return *( const deoglSkinShader * )pShaderList.GetAt( index );
}

deoglSkinShader *deoglSkinShaderManager::GetShaderWith( deoglSkinShaderConfig &configuration ){
	const deMutexGuard guard( pMutex );
	const int count = pShaderList.GetCount();
	int i;
	
	configuration.UpdateKey();
	
	for( i=0; i<count; i++ ){
		deoglSkinShader * const shader = ( deoglSkinShader* )pShaderList.GetAt( i );
		if( shader->GetConfig() == configuration ){
			return shader;
		}
	}
	
	const deoglSkinShader::Ref shader( deoglSkinShader::Ref::New(
		new deoglSkinShader( pRenderThread, configuration ) ) );
	
	// make GetShader() to be present. this is a potentially lengthy call if the
	// shader has to be compiled instead of loaded from cache
	shader->PrepareShader();
	
	pShaderList.Add( shader );
	return shader;
}
