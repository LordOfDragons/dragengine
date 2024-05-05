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
int unitCount, deoglShaderParameterBlock *paramBlock ){
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
