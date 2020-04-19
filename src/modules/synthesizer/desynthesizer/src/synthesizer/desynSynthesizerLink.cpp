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

#include "desynSynthesizerLink.h"
#include "desynSynthesizerInstance.h"
#include "desynSynthesizerController.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>



// Class desynSynthesizerLink
///////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerLink::desynSynthesizerLink( const deSynthesizerLink &link ) :
pController( link.GetController() ),
pRepeat( link.GetRepeat() ),
pCurve( link.GetCurve() ){
}

desynSynthesizerLink::desynSynthesizerLink( const deSynthesizerLink &link, const decIntList &controllerMapping ) :
pController( link.GetController() ),
pRepeat( link.GetRepeat() ),
pCurve( link.GetCurve() )
{
	if( pController == -1 ){
		return;
	}
	
	if( pController < controllerMapping.GetCount() ){
		pController = controllerMapping.GetAt( pController );
		
	}else{
		pController = -1;
	}
}

desynSynthesizerLink::~desynSynthesizerLink(){
}



// Management
///////////////

float desynSynthesizerLink::GetValue( const desynSynthesizerInstance &instance, int sample, float defaultValue ) const{
	if( pController == -1 ){
		return defaultValue;
	}
	
	float value = instance.GetControllerAt( pController ).GetValue( sample );
	
	if( pRepeat > 1 ){
		value *= ( float )pRepeat;
		value -= floorf( value );
	}
	
	return pCurve.Evaluate( value );
}
