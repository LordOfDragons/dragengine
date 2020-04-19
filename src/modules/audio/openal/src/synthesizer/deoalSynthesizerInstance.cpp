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

#include "deoalSynthesizerInstance.h"
#include "deoalASynthesizerInstance.h"
#include "../deoalBasics.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATDebug.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class deoalSynthesizerInstance
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalSynthesizerInstance::deoalSynthesizerInstance( deAudioOpenAL &oal, deSynthesizerInstance &instance ) :
pOal( oal ),
pInstance( instance ),
pAInstance( NULL ),
pUpdateTracker( 1 ),
pDirtyParameters( false )
{
	pAInstance = new deoalASynthesizerInstance( oal.GetAudioThread(), instance );
}

deoalSynthesizerInstance::~deoalSynthesizerInstance(){
	if( pAInstance ){
		pAInstance->DropInstance();
		pAInstance->FreeReference();
	}
}



// Management
///////////////

void deoalSynthesizerInstance::Synchronize(){
	// WARNING Called during synchronization time from main thread.
	
	bool advanceUpdateTracker = false;
	
	if( pDirtyParameters ){
		pAInstance->UpdateParameters();
		pDirtyParameters = false;
		advanceUpdateTracker = true;
	}
	
	if( advanceUpdateTracker ){
		pUpdateTracker++;
		if( pUpdateTracker == 0 ){
			pUpdateTracker = 1;
		}
	}
}



// Notifications
//////////////////

void deoalSynthesizerInstance::SynthesizerChanged(){
	pDirtyParameters = true;
}

void deoalSynthesizerInstance::PlayTimeChanged(){
	// required to force play range update. without this changing the play time does not
	// change the play range causing play to stop too early.
	pDirtyParameters = true;
}
