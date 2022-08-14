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
#include <stdlib.h>
#include <string.h>

#include "deoglRenderTaskParticles.h"
#include "deoglRenderTaskParticlesStep.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderUnitSourceCode.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskParticles
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskParticles::deoglRenderTaskParticles(){
	pRenderParamBlock = nullptr;
	pRenderVSStereo = false;
	
	pSteps = NULL;
	pStepCount = 0;
	pStepSize = 0;
}

deoglRenderTaskParticles::~deoglRenderTaskParticles(){
	if( pSteps ){
		while( pStepSize > 0 ){
			pStepSize--;
			if( pSteps[ pStepSize ] ) delete pSteps[ pStepSize ];
		}
		
		delete [] pSteps;
	}
}



// Management
///////////////

void deoglRenderTaskParticles::Clear(){
	RemoveAllSteps();
	pRenderParamBlock = nullptr;
	pRenderVSStereo = false;
}

void deoglRenderTaskParticles::SetRenderParamBlock( deoglSPBlockUBO *paramBlock ){
	pRenderParamBlock = paramBlock;
}

void deoglRenderTaskParticles::SetRenderVSStereo( bool renderVSStereo ){
	pRenderVSStereo = renderVSStereo;
}



deoglRenderTaskParticlesStep *deoglRenderTaskParticles::GetStepAt( int index ) const{
	if( index < 0 || index >= pStepCount ) DETHROW( deeInvalidParam );
	
	return pSteps[ index ];
}

deoglRenderTaskParticlesStep *deoglRenderTaskParticles::AddStep(){
	if( pStepCount == pStepSize ){
		int newSize = pStepSize + 10;
		deoglRenderTaskParticlesStep **newArray = new deoglRenderTaskParticlesStep*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pSteps ){
			memcpy( newArray, pSteps, sizeof( deoglRenderTaskParticlesStep* ) * pStepSize );
			delete [] pSteps;
		}
		memset( newArray + pStepSize, '\0', sizeof( deoglRenderTaskParticlesStep* ) * ( newSize - pStepSize ) );
		pSteps = newArray;
		pStepSize = newSize;
	}
	
	if( ! pSteps[ pStepCount ] ){
		pSteps[ pStepCount ] = new deoglRenderTaskParticlesStep;
		if( ! pSteps[ pStepCount ] ) DETHROW( deeOutOfMemory );
		
	}else{
		pSteps[ pStepCount ]->Reset();
	}
	
	pStepCount++;
	return pSteps[ pStepCount - 1 ];
}

void deoglRenderTaskParticles::RemoveAllSteps(){
	pStepCount = 0;
}

void deoglRenderTaskParticles::DebugPrint( deoglRTLogger &rtlogger ){
	decString text;
	int i, j;
	
	rtlogger.LogInfoFormat( "RenderTaskParticle %p: particles=%d", this, pStepCount );
	
	const deoglShaderProgram *shader = NULL;
	
	for( i=0; i<pStepCount; i++ ){
		const deoglRenderTaskParticlesStep &step = *pSteps[ i ];
		if( step.GetShader() == shader ){
			continue;
		}
		
		shader = step.GetShader();
		rtlogger.LogInfo( "  - configuration:" );
		rtlogger.LogInfoFormat( "    - vertex %s", shader->GetVertexSourceCode()
			? shader->GetVertexSourceCode()->GetFilePath() : "-" );
		rtlogger.LogInfoFormat( "    - geometry %s", shader->GetGeometrySourceCode()
			? shader->GetGeometrySourceCode()->GetFilePath() : "-" );
		rtlogger.LogInfoFormat( "    - fragment %s", shader->GetFragmentSourceCode()
			? shader->GetFragmentSourceCode()->GetFilePath() : "-" );
		
		text = "    - defines: ";
		const deoglShaderDefines &defines = shader->GetDefines();
		const int defineCount = defines.GetDefineCount();
		for( j=0; j<defineCount; j++ ){
			const char *defineName = defines.GetDefineNameAt( j );
			const char *defineValue = defines.GetDefineValueAt( j );
			
			if( strlen( defineValue ) > 10 ){
				text.AppendFormat( "%s %s=%.10s...", j == 0 ? "" : ",", defineName, defineValue );
				
			}else{
				text.AppendFormat( "%s %s=%s", j == 0 ? "" : ",", defineName, defineValue );
			}
		}
		rtlogger.LogInfo( text.GetString() );
		
		const int unitCount = step.GetTUC()->GetUnitCount();
		text.Format( "    units(" );
		for( j=0; j<unitCount; j++ ){
			const deoglTexUnitConfig &unit = step.GetTUC()->GetUnitAt( j );
			if( unit.GetTexture() ){
				text.AppendFormat( " T%i", unit.GetTexture()->GetTexture() );
				
			}else if( unit.GetCubeMap() ){
				text.AppendFormat( " C%i", unit.GetCubeMap()->GetTexture() );
				
			}else if( unit.GetSpecial() != deoglTexUnitConfig::estNone ){
				text.AppendFormat( " S%i", unit.GetSpecial() );
				
			}else{
				text.AppendFormat( " -" );
			}
		}
		text.AppendFormat( " )" );
		rtlogger.LogInfo( text.GetString() );
	}
}
