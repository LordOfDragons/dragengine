/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceActorGesture.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/loadsave/igdeLoadAnimator.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class ceActorGesture
/////////////////////////

// Constructor, destructor
////////////////////////////

ceActorGesture::ceActorGesture( igdeEnvironment &environment, const char *name ) :
pEnvironment( environment ),
pEngAnimator( NULL ),
pName( name ){
}

ceActorGesture::ceActorGesture( const ceActorGesture &gesture ) :
pEnvironment( gesture.pEnvironment ),
pEngAnimator( NULL ),
pName( gesture.pName ),
pPathAnimator( gesture.pPathAnimator )
{
	pEngAnimator = gesture.pEngAnimator;
	if( pEngAnimator ){
		pEngAnimator->AddReference();
	}
}

ceActorGesture::~ceActorGesture(){
	if( pEngAnimator ){
		pEngAnimator->SetRig( NULL );
		pEngAnimator->FreeReference();
	}
}



// Management
///////////////

void ceActorGesture::SetName( const char *name ){
	pName = name;
}

void ceActorGesture::SetPathAnimator( const char *path ){
	if( pPathAnimator.Equals( path ) ){
		return;
	}
	
	pPathAnimator = path;
	pLoadAnimator();
}



// Private Functions
//////////////////////

void ceActorGesture::pLoadAnimator(){
	if( pPathAnimator.IsEmpty() ){
		return;
	}
	
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	decBaseFileReaderReference reader;
	deAnimator *animator = NULL;
	
	try{
		reader.TakeOver( engine.GetVirtualFileSystem()->OpenFileForReading(
			decPath::CreatePathUnix( pPathAnimator ) ) );
		animator = engine.GetAnimatorManager()->CreateAnimator();
		
		igdeLoadAnimator( pEnvironment, pEnvironment.GetLogger(), LOGSOURCE ).
			Load( pPathAnimator, *animator, reader );
		
		if( pEngAnimator ){
			pEngAnimator->FreeReference();
		}
		pEngAnimator = animator;
		
	}catch( const deException &e ){
		if( animator ){
			animator->FreeReference();
		}
		pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
		throw;
	}
}
