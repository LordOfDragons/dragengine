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
	"v130/vertex/defren/skin/geometry.glsl", // euscpVertexGeometry
	"v130/vertex/defren/skin/depth.glsl", // euscpVertexDepth
	"v130/vertex/defren/skin/particle.glsl", // euscpVertexParticle
	"v130/vertex/defren/skin/passthrough.glsl", // euscpVertexPassThrough
	"v130/vertex/defren/gi/materialMap.glsl", // euscpGIMaterialMap
	
	"v130/geometry/defren/skin/geometry.glsl", // euscpGeometryGeometry
	"v130/geometry/defren/skin/depth.glsl", // euscpGeometryDepth
	"v130/geometry/defren/skin/particle.glsl", // euscpGeometryParticle
	"v130/geometry/defren/skin/particle_ribbon.glsl", // euscpGeometryParticleRibbon
	
	"v130/tessctrl/defren/skin/geometry.glsl", // euscpTessControlGeometry
	"v130/tessctrl/defren/skin/depth.glsl", // euscpTessControlDepth
	
	"v130/tesseval/defren/skin/geometry.glsl", // euscpTessEvalGeometry
	"v130/tesseval/defren/skin/depth.glsl", // euscpTessEvalDepth
	
	"v130/fragment/defren/skin/geometry.glsl", // euscpFragmentGeometry
	"v130/fragment/defren/skin/depth.glsl", // euscpFragmentDepth
	"v130/fragment/defren/gi/materialMap.glsl" // euscpGIMaterialMap
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
