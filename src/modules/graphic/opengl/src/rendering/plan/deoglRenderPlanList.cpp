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

#include "deoglRenderPlan.h"
#include "deoglRenderPlanList.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderPlanList
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanList::deoglRenderPlanList(){
	pPlans = NULL;
	pPlanCount = 0;
	pPlanSize = 0;
}

deoglRenderPlanList::~deoglRenderPlanList(){
	if( pPlans ){
		delete [] pPlans;
	}
}



// Management
///////////////

deoglRenderPlan *deoglRenderPlanList::GetAt( int index ) const{
	if( index < 0 || index >= pPlanCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pPlans[ index ];
}

int deoglRenderPlanList::IndexOf( deoglRenderPlan *plan ) const{
	if( ! plan ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pPlanCount; i++ ){
		if( plan == pPlans[ i ] ){
			return i;
		}
	}
	
	return -1;
}

bool deoglRenderPlanList::Has( deoglRenderPlan *plan ) const{
	if( ! plan ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pPlanCount; i++ ){
		if( plan == pPlans[ i ] ){
			return true;
		}
	}
	
	return false;
}

void deoglRenderPlanList::Add( deoglRenderPlan *plan ){
	if( Has( plan ) ){
		DETHROW( deeInvalidParam );
	}
	
	pAdd( plan );
}

bool deoglRenderPlanList::AddIfMissing( deoglRenderPlan *plan ){
	if( Has( plan ) ){
		return false;
	}
	
	pAdd( plan );
	
	return true;
}

void deoglRenderPlanList::Remove( deoglRenderPlan *plan ){
	const int index = IndexOf( plan );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < pPlanCount - 1 ){
		pPlans[ index ] = pPlans[ pPlanCount - 1 ];
	}
	pPlanCount--;
}

bool deoglRenderPlanList::RemoveIfExisting( deoglRenderPlan *plan ){
	const int index = IndexOf( plan );
	
	if( index == -1 ){
		return false;
	}
	
	if( index < pPlanCount - 1 ){
		pPlans[ index ] = pPlans[ pPlanCount - 1 ];
	}
	pPlanCount--;
	
	return true;
}

void deoglRenderPlanList::RemoveFrom( int index ){
	if( index < 0 || index >= pPlanCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < pPlanCount - 1 ){
		pPlans[ index ] = pPlans[ pPlanCount - 1 ];
	}
	pPlanCount--;
}

void deoglRenderPlanList::RemoveAll(){
	pPlanCount = 0;
}



// Private Functions
//////////////////////

void deoglRenderPlanList::pAdd( deoglRenderPlan *plan ){
	if( pPlanCount == pPlanSize ){
		const int newSize = pPlanCount + 10; // * 3 / 2 + 1;
		deoglRenderPlan ** const newArray = new deoglRenderPlan*[ newSize ];
		
		if( pPlans ){
			memcpy( newArray, pPlans, sizeof( deoglRenderPlan* ) * pPlanSize );
			delete [] pPlans;
		}
		
		pPlans = newArray;
		pPlanSize = newSize;
	}
	
	pPlans[ pPlanCount++ ] = plan;
}
