/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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
	const int shaderCount = pShaderList.GetCount();
	decString text;
	int i;
	
	for( i=0; i<shaderCount; i++ ){
		const deoglSkinShader &shader = *( ( deoglSkinShader* )pShaderList.GetAt( i ) );
		
		if( shader.GetRefCount() != 1 ){
			shader.GetConfig().DebugGetConfigString( text );
			pRenderThread.GetLogger().LogWarnFormat(
				"ShaderSkinManager CleanUp: Shader with refcount %i. Config=%s",
				shader.GetRefCount(), text.GetString() );
		}
	}
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

deoglSkinShader::Ref deoglSkinShaderManager::GetShaderWith( deoglSkinShaderConfig &configuration ){
	const deMutexGuard guard( pMutex );
	const int count = pShaderList.GetCount();
	int i;
	
	configuration.UpdateKey();
	
	for( i=0; i<count; i++ ){
		deoglSkinShader * const shader = ( deoglSkinShader* )pShaderList.GetAt( i );
		if( shader->GetConfig() == configuration ){
			return deoglSkinShader::Ref( shader );
		}
	}
	
	const deoglSkinShader::Ref shader( deoglSkinShader::Ref::New(
		new deoglSkinShader( pRenderThread, configuration ) ) );
	pShaderList.Add( shader );
	return shader;
}
