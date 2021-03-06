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

#include "deoglTexUnitConfig.h"
#include "deoglTexUnitsConfig.h"
#include "deoglTexUnitsConfigList.h"
#include "../deoglTextureStageManager.h"
#include "../cubemap/deoglCubeMap.h"
#include "../texture2d/deoglTexture.h"
#include "../../gi/deoglGI.h"
#include "../../gi/deoglGIMaterials.h"
#include "../../rendering/defren/deoglDeferredRendering.h"
#include "../../rendering/plan/deoglRenderPlan.h"
#include "../../rendering/deoglRenderReflection.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../../rendering/task/shared/deoglRenderTaskSharedTexture.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
#include "../../renderthread/deoglRTUniqueKey.h"

#include <dragengine/common/exceptions.h>



// Class deoglTexUnitsConfig
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTexUnitsConfig::deoglTexUnitsConfig( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pUnits( NULL ),
pUnitCount( 0 ),
pParamBlock( NULL ),
pMaterialIndex( -1 ),
pMaterialUsageCount( 0 ),
pUsageCount( 1 ),
pUnitsHashCode( 0 ),
pUniqueKey( renderThread.GetUniqueKey().Get() ),
pRTSTexture( NULL ),
pLLPrev( NULL ),
pLLNext( NULL ){
}

deoglTexUnitsConfig::~deoglTexUnitsConfig(){
	if( pRTSTexture ){
		pRenderThread.GetRenderTaskSharedPool().ReturnTexture( pRTSTexture );
	}
	
	if( pUnits ){
		delete [] pUnits;
	}
	
	pRenderThread.GetUniqueKey().Return( pUniqueKey );
	
}



// Management
///////////////

void deoglTexUnitsConfig::SetUnitCount( int count ){
	if( pUnits ){
		delete [] pUnits;
		pUnits = NULL;
		pUnitCount = 0;
	}
	
	if( count > 0 ){
		pUnits = new deoglTexUnitConfig[ count ];
		pUnitCount = count;
	}
}

deoglTexUnitConfig &deoglTexUnitsConfig::GetUnitAt( int index ) const{
	if( index < 0 || index >= pUnitCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pUnits[ index ];
}

void deoglTexUnitsConfig::SetUnits( const deoglTexUnitConfig *units, int unitCount ){
	if( unitCount < 0 || ( unitCount > 0 && ! units ) ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	SetUnitCount( unitCount );
	for( i=0; i<unitCount; i++ ){
		pUnits[ i ].SetFrom( units[ i ] );
	}
}

void deoglTexUnitsConfig::SetParameterBlock( deoglShaderParameterBlock *paramBlock ){
	pParamBlock = paramBlock;
}



void deoglTexUnitsConfig::Apply() const{
	//deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	//tsmgr.DisableStagesAbove( pUnitCount - 1 );
	
	int i;
	for( i=0; i<pUnitCount; i++ ){
		pUnits[ i ].Apply( pRenderThread, i );
	}
}



void deoglTexUnitsConfig::SetMaterialIndex( int index ){
	pMaterialIndex = index;
}

void deoglTexUnitsConfig::AddMaterialUsage(){
	pMaterialUsageCount++;
}

void deoglTexUnitsConfig::RemoveMaterialUsage(){
	if( pMaterialUsageCount == 0 ){
		DETHROW( deeInvalidParam );
	}
	pMaterialUsageCount--;
}



bool deoglTexUnitsConfig::Equals( const deoglTexUnitsConfig &tuc ) const{
	if( pUnitsHashCode != tuc.pUnitsHashCode || pUnitCount != tuc.pUnitCount ){
		return false;
	}
	
	int i;
	
	for( i=0; i<pUnitCount; i++ ){
		if( ! pUnits[ i ].Equals( tuc.pUnits[ i ] ) ){
			return false;
		}
	}
	
	return true;
}

bool deoglTexUnitsConfig::Equals( const deoglTexUnitConfig *units, int unitCount,
deoglShaderParameterBlock *paramBlock ) const{
	if( unitCount < 0 || ( unitCount > 0 && ! units ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pUnitCount != unitCount || pParamBlock != paramBlock ){
		return false;
	}
	
	int i;
	for( i=0; i<pUnitCount; i++ ){
		if( ! pUnits[ i ].Equals( units[ i ] ) ){
			return false;
		}
	}
	
	return true;
}



void deoglTexUnitsConfig::AddUsage(){
	pUsageCount++;
}

void deoglTexUnitsConfig::RemoveUsage(){
	pUsageCount--;
	
	if( pUsageCount > 0 ){
		return;
	}
	
	if( pRTSTexture ){
		pRenderThread.GetRenderTaskSharedPool().ReturnTexture( pRTSTexture );
		pRTSTexture = NULL;
	}
	
	pRenderThread.GetShader().GetTexUnitsConfigList().Remove( this );
}

void deoglTexUnitsConfig::CalcUnitsHashCode(){
	pUnitsHashCode = CalcUnitsHashCodeForUnits( pUnits, pUnitCount );
}

unsigned int deoglTexUnitsConfig::CalcUnitsHashCodeForUnits( const deoglTexUnitConfig *units, int unitCount ){
	if( unitCount < 0 || ( unitCount > 0 && ! units ) ){
		DETHROW( deeInvalidParam );
	}
	
	// for the hash function the opengl texture names of each unit are simply added.
	// this works since opengl names are unique for each texture no matter what type
	// the texture is of. the filtering and wrapping parameters are not very useful
	// for the hash value as is the unit number. textures tend to be used for the
	// same units in different configurations hence the order of the units is not
	// important. this gives a fast hash value that does the trick good enough
	unsigned int hashCode = 0;
	int i;
	
	for( i=0; i<unitCount; i++ ){
		if( units[ i ].GetTexture() ){
			hashCode += ( unsigned int )deoglTexUnitConfig::EST_BASE_GLNAME
				+ ( unsigned int )units[ i ].GetTexture()->GetTexture();
			
		}else if( units[ i ].GetCubeMap() ){
			hashCode += ( unsigned int )deoglTexUnitConfig::EST_BASE_GLNAME
				+ ( unsigned int )units[ i ].GetCubeMap()->GetTexture();
			
		}else if( units[ i ].GetTBO() ){
			hashCode += ( unsigned int )deoglTexUnitConfig::EST_BASE_GLNAME
				+ ( unsigned int )units[ i ].GetTBO();
			
		}else{
			hashCode += ( unsigned int )units[ i ].GetSpecial();
		}
	}
	
	return hashCode;
	
	/*
	unsigned int temp;
	int i;
	
	for( i=0; i<unitCount; i++ ){
		temp = hashCode >> 24;
		hashCode = ( hashCode << 8 ) + temp;
		
		if( units[ i ].GetTexture() ){
			hashCode ^= ( unsigned int )units[ i ].GetTexture()->GetTexture();
			
		}else if( units[ i ].GetCubeMap() ){
			hashCode ^= ( unsigned int )units[ i ].GetCubeMap()->GetTexture();
			
		}else{
			hashCode = ~hashCode;
		}
	}
	*/
}



void deoglTexUnitsConfig::EnsureRTSTexture(){
	if( pRTSTexture ){
		return;
	}
	
	pRTSTexture = pRenderThread.GetRenderTaskSharedPool().GetTexture();
	pRTSTexture->SetTUC( this );
}



void deoglTexUnitsConfig::SetLLPrev( deoglTexUnitsConfig *entry ){
	pLLPrev = entry;
}

void deoglTexUnitsConfig::SetLLNext( deoglTexUnitsConfig *entry ){
	pLLNext = entry;
}
