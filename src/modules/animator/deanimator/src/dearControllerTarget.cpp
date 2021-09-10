/* 
 * Drag[en]gine Animator Module
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
#include <unistd.h>

#include "dearAnimatorInstance.h"
#include "dearControllerTarget.h"
#include "dearControllerStates.h"
#include "dearLink.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>



// Class dearControllerTarget
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearControllerTarget::dearControllerTarget( const deAnimatorControllerTarget &target, int firstLink ) :
pLinks( NULL ),
pLinkCount( 0 ){
	const int linkCount = target.GetLinkCount();
	
	if( linkCount > 0 ){
		pLinks = new int[ linkCount ];
		
		for( pLinkCount=0; pLinkCount<linkCount; pLinkCount++ ){
			pLinks[ pLinkCount ] = firstLink + target.GetLinkAt( pLinkCount );
		}
	}
}

dearControllerTarget::~dearControllerTarget(){
	if( pLinks ){
		delete [] pLinks;
	}
}



// Management
///////////////

int dearControllerTarget::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ){
		DETHROW( deeInvalidParam );
	}
	return pLinks[ index ];
}



float dearControllerTarget::GetValue( const dearAnimatorInstance &instance, float defaultValue ) const{
	float value = defaultValue;
	bool firstValue = true;
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		if( pLinks[ i ] == -1 ){
			continue;
		}
		
		const dearLink &link = *instance.GetLinkAt( pLinks[ i ] );
		if( ! link.HasController() && ! link.HasBone() ){
			continue;
		}
		
		if( firstValue ){
			value = link.GetValue( 1.0f );
			firstValue = false;
			
		}else{
			value *= link.GetValue( 1.0f );
		}
	}
	
	return value;
}

void dearControllerTarget::GetVector( const dearAnimatorInstance &instance, decVector &vector ) const{
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		if( pLinks[ i ] == -1 ){
			continue;
		}
		
		const dearLink &link = *instance.GetLinkAt( pLinks[ i ] );
		if( ! link.HasController() ){
			continue;
		}
		
		vector = instance.GetControllerStates().GetVectorAt( link.GetController() );
		break;
	}
}

void dearControllerTarget::GetQuaternion( const dearAnimatorInstance &instance, decQuaternion &quaternion ) const{
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		if( pLinks[ i ] == -1 ){
			continue;
		}
		
		const dearLink &link = *instance.GetLinkAt( pLinks[ i ] );
		if( ! link.HasController() ){
			continue;
		}
		
		quaternion.SetFromEuler( instance.GetControllerStates().GetVectorAt( link.GetController() ) );
		break;
	}
}
