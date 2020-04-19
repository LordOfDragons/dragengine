/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalRayTraceResult.h"
#include "deoalRayTraceHitElement.h"
#include "../../audiothread/deoalAudioThread.h"
#include "../../audiothread/deoalATLogger.h"
#include "../../component/deoalAComponent.h"
#include "../../model/deoalAModel.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>



// Class deoalRayTraceResult
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRayTraceResult::deoalRayTraceResult() :
pElements( NULL ),
pElementCount( 0 ),
pElementSize( 0 )
{
	const int limit = 20;
	pElements = new deoalRayTraceHitElement[ limit ];
	pElementSize = limit;
}

deoalRayTraceResult::~deoalRayTraceResult(){
	if( pElements ){
		delete [] pElements;
	}
}



// Manegement
///////////////

const deoalRayTraceHitElement &deoalRayTraceResult::GetElementAt( int index ) const{
	if( index < 0 || index >= pElementCount ){
		DETHROW( deeInvalidParam );
	}
	return pElements[ index ];
}

void deoalRayTraceResult::AddElement( float distance, const decDVector &point,
const decDVector &normal, deoalAComponent *component, int face, bool frontFacing ){
	deoalRayTraceHitElement * const element = pInsert( distance );
	if( element ){
		element->SetComponentFace( distance, point, normal, component, face, frontFacing );
	}
}

void deoalRayTraceResult::AddElement( const deoalRayTraceHitElement &element ){
	deoalRayTraceHitElement * const insertedElement = pInsert( element.GetDistance() );
	if( insertedElement ){
		insertedElement->SetComponentFace( element.GetDistance(), element.GetPoint(),
			element.GetNormal(), element.GetComponent(), element.GetComponentFace(),
			element.GetForwardFacing() );
	}
}

void deoalRayTraceResult::RemoveAllElements(){
	pElementCount = 0;
}

void deoalRayTraceResult::Clear(){
	pElementCount = 0;
}

void deoalRayTraceResult::DebugPrint( deoalAudioThread &audioThread, const char *prefix ){
	deoalATLogger &logger = audioThread.GetLogger();
	
	logger.LogInfoFormat( "%s) Ray-Trace Result (%d elements):", prefix, pElementCount );
	int i;
	for( i=0; i<pElementCount; i++ ){
		const decDVector &point = pElements[ i ].GetPoint();
		const float distance = pElements[ i ].GetDistance();
		
		if( pElements[ i ].GetComponent() ){
			const deoalAComponent &component = *pElements[ i ].GetComponent();
			const decDVector &compPos = component.GetPosition();
			const char * const model = component.GetModel() ? component.GetModel()->GetFilename().GetString() : "-";
			const int face = pElements[ i ].GetComponentFace();
			const bool frontFacing = pElements[ i ].GetForwardFacing();
			
			logger.LogInfoFormat( "%s) - distance=%.3f point=(%.3f,%.3f,%.3f) "
				"component=(%.3f,%.3f,%.3f) model=%s face=%d frontFacing=%d",
				prefix, distance, point.x, point.y, point.z, compPos.x, compPos.y, compPos.z,
				model, face, frontFacing );
			
		}else{
			logger.LogInfoFormat( "%s) - distance=%.3f point=(%.3f,%.3f,%.3f) ??",
				prefix, distance, point.x, point.y, point.z );
		}
	}
}



// Protected functions
////////////////////////

deoalRayTraceHitElement *deoalRayTraceResult::pInsert( float distance ){
	if( pElementCount == 0 ){
		return pElements + pElementCount++;
	}
	
	int end = pElementCount - 1;
	int begin = 0;
	
	while( begin < end ){
		const int center = ( begin + end ) / 2;
		
		if( distance < pElements[ center ].GetDistance() ){
			end = center - 1;
			
		}else{
			begin = center + 1;
		}
	}
	
	if( distance >= pElements[ begin ].GetDistance() ){
		begin++;
	}
	
	if( pElementCount < pElementSize ){
		end = pElementCount++;
		
	}else{
		if( begin == pElementCount ){
			return NULL; // element to insert drops out
		}
		end = pElementCount - 1;
	}
	
	for( ; end>begin; end-- ){
		pElements[ end ] = pElements[ end - 1 ];
	}
	
	return pElements + begin;
}
