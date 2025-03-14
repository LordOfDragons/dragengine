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
#include <stdlib.h>
#include <string.h>

#include "deoglRenderTaskParticles.h"
#include "deoglRenderTaskParticlesStep.h"
#include "../../pipeline/deoglPipeline.h"
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
	
	const deoglPipeline *pipeline = nullptr;
	
	for( i=0; i<pStepCount; i++ ){
		const deoglRenderTaskParticlesStep &step = *pSteps[ i ];
		if( step.GetPipeline() == pipeline ){
			continue;
		}
		
		pipeline = step.GetPipeline();
		rtlogger.LogInfo( "  - configuration:" );
		const deoglShaderProgram &shader = pipeline->GetConfiguration().GetShaderRef();
		rtlogger.LogInfoFormat( "    - vertex %s", shader.GetVertexSourceCode()
			? shader.GetVertexSourceCode()->GetFilePath().GetString() : "-" );
		rtlogger.LogInfoFormat( "    - geometry %s", shader.GetGeometrySourceCode()
			? shader.GetGeometrySourceCode()->GetFilePath().GetString() : "-" );
		rtlogger.LogInfoFormat( "    - fragment %s", shader.GetFragmentSourceCode()
			? shader.GetFragmentSourceCode()->GetFilePath().GetString() : "-" );
		
		text = "    - defines: ";
		const deoglShaderDefines &defines = shader.GetDefines();
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
