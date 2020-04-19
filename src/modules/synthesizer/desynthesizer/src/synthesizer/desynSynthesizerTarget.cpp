/* 
 * Drag[en]gine Synthesizer Module
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

#include "desynSynthesizerTarget.h"
#include "desynSynthesizer.h"
#include "desynSynthesizerInstance.h"
#include "desynSynthesizerLink.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/deSynthesizerControllerTarget.h>



// Class desynSynthesizerTarget
/////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerTarget::desynSynthesizerTarget( const desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerControllerTarget &target ) :
pLinks( NULL ),
pLinkCount( 0 )
{
	const int linkCount = target.GetLinkCount();
	
	if( linkCount == 0 ){
		return;
	}
	
	pLinks = new const desynSynthesizerLink*[ linkCount ];
	
	for( pLinkCount=0; pLinkCount<linkCount; pLinkCount++ ){
		pLinks[ pLinkCount ] = &synthesizer.GetLinkAt( firstLink + target.GetLinkAt( pLinkCount ) );
	}
}

desynSynthesizerTarget::~desynSynthesizerTarget(){
	if( pLinks ){
		delete [] pLinks;
	}
}



// Management
///////////////

const desynSynthesizerLink &desynSynthesizerTarget::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ){
		DETHROW( deeInvalidParam );
	}
	return *pLinks[ index ];
}



float desynSynthesizerTarget::GetValue( const desynSynthesizerInstance &instance, int sample, float defaultValue ) const{
	if( pLinkCount == 0 ){
		return defaultValue;
	}
	
	float value = defaultValue;
	bool firstValue = true;
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		const desynSynthesizerLink &link = *pLinks[ i ];
		if( ! link.HasController() ){
			continue;
		}
		
		if( firstValue ){
			value = link.GetValue( instance, sample, 1.0f );
			firstValue = false;
			
		}else{
			value *= link.GetValue( instance, sample, 1.0f );
		}
	}
	
	return decMath::clamp( value, 0.0f, 1.0f );
}
