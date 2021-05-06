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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglTexUnitConfig.h"
#include "deoglTexUnitsConfig.h"
#include "deoglTexUnitsConfigList.h"

#include <dragengine/common/exceptions.h>



// Class deoglTexUnitsConfigList
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTexUnitsConfigList::deoglTexUnitsConfigList( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pRootTUC( NULL ),
pTailTUC( NULL ),
pTUCCount( 0 ),
pTUCEmpty( NULL )
{
	pTUCEmpty = GetWith( NULL, 0, NULL );
	pTUCEmpty->EnsureRTSTexture();
}

deoglTexUnitsConfigList::~deoglTexUnitsConfigList(){
	while( pRootTUC ){
		pTailTUC = pRootTUC;
		pRootTUC = pRootTUC->GetLLNext();
		delete pTailTUC;
	}
	pTailTUC = NULL;
}



// Management
///////////////

deoglTexUnitsConfig *deoglTexUnitsConfigList::GetWith( const deoglTexUnitConfig *units,
int unitCount, deoglSPBlockUBO *paramBlock ){
	deoglTexUnitsConfig *tuc = pRootTUC;
	
	// if there exists already a texture units configuration with these properties add a usage and return it
	while( tuc ){
		if( tuc->Equals( units, unitCount, paramBlock ) ){
			tuc->AddUsage();
			return tuc;
		}
		tuc = tuc->GetLLNext();
	}
	
	// otherwise create a new texture units configuration with these parameters and return it
	tuc = new deoglTexUnitsConfig( pRenderThread );
	
	try{
		tuc->SetUnits( units, unitCount );
		tuc->SetParameterBlock( paramBlock );
		tuc->CalcUnitsHashCode();
		
		if( pTailTUC ){
			pTailTUC->SetLLNext( tuc );
			tuc->SetLLPrev( pTailTUC );
			pTailTUC = tuc;
			
		}else{
			pRootTUC = tuc;
			pTailTUC = tuc;
		}
		pTUCCount++;
		
	}catch( const deException & ){
		delete tuc;
		throw;
	}
	
	return tuc;
}

deoglTexUnitsConfig *deoglTexUnitsConfigList::GetEmpty(){
	pTUCEmpty->AddUsage();
	return pTUCEmpty;
}

deoglTexUnitsConfig *deoglTexUnitsConfigList::GetEmptyNoUsage() const{
	return pTUCEmpty;
}



void deoglTexUnitsConfigList::Remove( deoglTexUnitsConfig *config ){
	if( ! config ){
		DETHROW( deeInvalidParam );
	}
	
	if( config->GetLLPrev() ){
		config->GetLLPrev()->SetLLNext( config->GetLLNext() );
	}
	if( config->GetLLNext() ){
		config->GetLLNext()->SetLLPrev( config->GetLLPrev() );
	}
	if( pRootTUC == config ){
		pRootTUC = config->GetLLNext();
	}
	if( pTailTUC == config ){
		pTailTUC = config->GetLLPrev();
	}
	pTUCCount--;
	
	delete config;
}
