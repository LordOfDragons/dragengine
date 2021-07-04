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

#include "deoglSkinShader.h"
#include "deoglSkinShaderConfig.h"
#include "deoglSkinShaderManager.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderUnitSourceCode.h"

#include <dragengine/common/exceptions.h>



// Unit Source Code Path
////////777///////////////

static const char *vUnitSourceCodePath[ deoglSkinShaderManager::EUSCP_COUNT ] = {
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
			pRenderThread.GetLogger().LogWarnFormat( "ShaderSkinManager CleanUp: Shader with refcount %i. Config=%s",
				shader.GetRefCount(), text.GetString() );
		}
	}
	
	RemoveAllShaders();
}



// Management
///////////////

const char *deoglSkinShaderManager::GetUnitSourceCodePath( eUnitSourceCodePath unitSourceCodePath ) const{
	if( unitSourceCodePath < 0 || unitSourceCodePath >= EUSCP_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return vUnitSourceCodePath[ unitSourceCodePath ];
}



int deoglSkinShaderManager::GetShaderCount() const{
	return pShaderList.GetCount();
}

deoglSkinShader *deoglSkinShaderManager::GetShaderAt( int index ) const{
	return ( deoglSkinShader* )pShaderList.GetAt( index );
}

void deoglSkinShaderManager::AddShader( deoglSkinShader *shader ){
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
	pShaderList.Add( shader );
}

void deoglSkinShaderManager::RemoveShader( deoglSkinShader *shader ){
	pShaderList.Remove( shader );
}

void deoglSkinShaderManager::RemoveAllShaders(){
	pShaderList.RemoveAll();
}



bool deoglSkinShaderManager::HasShaderWith( deoglSkinShaderConfig &configuration ) const{
	const int count = pShaderList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglSkinShader &shader = *( ( deoglSkinShader* )pShaderList.GetAt( i ) );
		
		if( shader.GetConfig() == configuration ){
			return true;
		}
	}
	
	return false;
}

deoglSkinShader *deoglSkinShaderManager::GetShaderWith( deoglSkinShaderConfig &configuration ){
	const int count = pShaderList.GetCount();
	deoglSkinShader *shader = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		shader = ( deoglSkinShader* )pShaderList.GetAt( i );
		
		if( shader->GetConfig() == configuration ){
			shader->AddReference();
			return shader;
		}
	}
	
	shader = NULL;
	try{
		shader = new deoglSkinShader( pRenderThread, configuration );
		pShaderList.Add( shader );
		
	}catch( const deException & ){
		if( shader ){
			shader->FreeReference();
		}
		throw;
	}
	
	return shader;
}



void deoglSkinShaderManager::Maintanance(){
	// currently no maintanance done... all greedy
	
	if( pMaintananceInterval == 0 ){
		deoglSkinShader *shader = NULL;
		int i;
		
		for( i=pShaderList.GetCount()-1; i>=0; i-- ){
			shader = ( deoglSkinShader* )pShaderList.GetAt( i );
			
			if( shader->GetRefCount() == 1 ){
				pShaderList.Remove( shader );
			}
		}
		
		pMaintananceInterval = 10; // maintenance every 10 frames
	}
}
