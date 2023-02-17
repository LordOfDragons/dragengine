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

#include "deoglRSky.h"
#include "deoglRSkyControllerTarget.h"
#include "deoglRSkyInstance.h"
#include "deoglRSkyLink.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSkyControllerTarget.h>



// Class deoglRSkyControllerTarget
////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglRSkyControllerTarget::deoglRSkyControllerTarget( const deSkyControllerTarget &target ) :
pLinks( NULL ),
pLinkCount( 0 )
{
	const int linkCount = target.GetLinkCount();
	if( linkCount == 0 ){
		return;
	}
	
	pLinks = new int[ linkCount ];
	for( pLinkCount=0; pLinkCount<linkCount; pLinkCount++ ){
		pLinks[ pLinkCount ] = target.GetLinkAt( pLinkCount );
	}
}

deoglRSkyControllerTarget::~deoglRSkyControllerTarget(){
	if( pLinks ){
		delete [] pLinks;
	}
}



// Management
///////////////

int deoglRSkyControllerTarget::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ){
		DETHROW( deeInvalidParam );
	}
	return pLinks[ index ];
}

float deoglRSkyControllerTarget::GetValue( const deoglRSkyInstance &instance, float defaultValue ) const{
	if( ! instance.GetRSky() ){
		return defaultValue;
	}
	
	const deoglRSky &sky = *instance.GetRSky();
	float value = defaultValue;
	bool firstValue = true;
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		if( pLinks[ i ] == -1 ){
			continue;
		}
		
		const deoglRSkyLink &link = sky.GetLinkAt( pLinks[ i ] );
		if( link.IsDisabled() ){
			continue;
		}
		
		if( firstValue ){
			value = link.GetValue( instance );
			firstValue = false;
			
		}else{
			value *= link.GetValue( instance );
		}
	}
	
	return value;
}
