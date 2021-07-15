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
#include <unistd.h>
#include <string.h>

#include "deoalCaches.h"
#include "deAudioOpenAL.h"
#include "audiothread/deoalAudioThread.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class deoalCaches
//////////////////////

// Constructor, Destructor
////////////////////////////

deoalCaches::deoalCaches( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pSound( NULL )
{
	(void)pAudioThread; // silence compiler warning
	
	try{
		pSound = new deCacheHelper( &audioThread.GetOal().GetVFS(),
			decPath::CreatePathUnix( "/cache/local/sound" ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoalCaches::~deoalCaches(){
	pCleanUp();
}



// Management
///////////////

void deoalCaches::Lock(){
	pMutex.Lock();
}

void deoalCaches::Unlock(){
	pMutex.Unlock();
}



// Private Functions
//////////////////////

void deoalCaches::pCleanUp(){
	if( pSound ){
		delete pSound;
	}
}
