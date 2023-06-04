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

#include "deoglLightShader.h"
#include "deoglLightShaderConfig.h"
#include "deoglLightShaderManager.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderUnitSourceCode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Unit Source Code Path
////////777///////////////

static const char *vUnitSourceCodePath[ deoglLightShaderManager::EUSCP_COUNT ] = {
	// euscpVertexLight
	"vertex/defren/light/light.glsl",
	// euscpVertexParticle
	"vertex/defren/light/particle.glsl",
	// euscpGeometryParticle
	"geometry/defren/light/particle.glsl",
	// euscpGeometryStereo
	"geometry/defren/light/stereo.glsl",
	// euscpFragmentLight
	"fragment/defren/light/light.glsl"
};



// Class deoglLightShaderManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightShaderManager::deoglLightShaderManager( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pMaintananceInterval( 0 ){
}

deoglLightShaderManager::~deoglLightShaderManager(){
	const int shaderCount = pShaderList.GetCount();
	decString text;
	int i;
	
	for( i=0; i<shaderCount; i++ ){
		const deoglLightShader &shader = *( ( deoglLightShader* )pShaderList.GetAt( i ) );
		
		if( shader.GetRefCount() != 1 ){
			shader.GetConfig().DebugGetConfigString( text );
			pRenderThread.GetLogger().LogWarnFormat( "LightShaderManager CleanUp: Shader with refcount %i. Config=%s",
				shader.GetRefCount(), text.GetString() );
		}
	}
	
	RemoveAllShaders();
}



// Management
///////////////

const char *deoglLightShaderManager::GetUnitSourceCodePath( int unitSourceCodePath ) const{
	if( unitSourceCodePath < 0 || unitSourceCodePath >= EUSCP_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return vUnitSourceCodePath[ unitSourceCodePath ];
}



int deoglLightShaderManager::GetShaderCount() const{
	return pShaderList.GetCount();
}

deoglLightShader *deoglLightShaderManager::GetShaderAt( int index ) const{
	return ( deoglLightShader* )pShaderList.GetAt( index );
}

void deoglLightShaderManager::AddShader( deoglLightShader *shader ){
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
	pShaderList.Add( shader );
}

void deoglLightShaderManager::RemoveShader( deoglLightShader *shader ){
	pShaderList.Remove( shader );
}

void deoglLightShaderManager::RemoveAllShaders(){
	pShaderList.RemoveAll();
}



bool deoglLightShaderManager::HasShaderWith( deoglLightShaderConfig &configuration ) const{
	const int count = pShaderList.GetCount();
	int i;
	
	configuration.UpdateKey();
	
	for( i=0; i<count; i++ ){
		const deoglLightShader &shader = *( ( deoglLightShader* )pShaderList.GetAt( i ) );
		
		if( shader.GetConfig() == configuration ){
			return true;
		}
	}
	
	return false;
}

deoglLightShader *deoglLightShaderManager::GetShaderWith( deoglLightShaderConfig &configuration ){
	const int count = pShaderList.GetCount();
	deoglLightShader *shader = NULL;
	int i;
	
	configuration.UpdateKey();
	
	for( i=0; i<count; i++ ){
		shader = ( deoglLightShader* )pShaderList.GetAt( i );
		
		if( shader->GetConfig() == configuration ){
			shader->AddReference();
			return shader;
		}
	}
	
	shader = NULL;
	try{
		shader = new deoglLightShader( pRenderThread, configuration );
		pShaderList.Add( shader );
		
	}catch( const deException & ){
		if( shader ){
			shader->FreeReference();
		}
		throw;
	}
	
	return shader;
}



void deoglLightShaderManager::Maintanance(){
	// currently no maintanance done... all greedy
	
	if( pMaintananceInterval == 0 ){
		deoglLightShader *shader = NULL;
		int i;
		
		for( i=pShaderList.GetCount()-1; i>=0; i-- ){
			shader = ( deoglLightShader* )pShaderList.GetAt( i );
			
			if( shader->GetRefCount() == 1 ){
				pShaderList.Remove( shader );
			}
		}
		
		pMaintananceInterval = 10; // maintenance every 10 frames
	}
}
