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

#include "ceActorPose.h"
#include "../controller/ceActorController.h"
#include "../gesture/ceActorGesture.h"

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
#include <dragengine/resources/animator/controller/deAnimatorController.h>



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class ceActorPose
//////////////////////

// Constructor, destructor
////////////////////////////

ceActorPose::ceActorPose( igdeEnvironment &environment, const char *name ) :
pEnvironment( environment ),
pEngAnimator( NULL ),
pName( name ){
}

ceActorPose::ceActorPose( const ceActorPose &pose ) :
pEnvironment( pose.pEnvironment ),
pEngAnimator( NULL ),
pName( pose.pName ),
pPathAnimator( pose.pPathAnimator ),
pControllerNames( pose.pControllerNames )
{
	// clone gestures
	const int gestureCount = pose.pGestures.GetCount();
	int i;
	
	for( i=0; i<gestureCount; i++ ){
		ceActorGesture *gesture = NULL;
		
		try{
			gesture = new ceActorGesture( *pose.pGestures.GetAt( i ) );
			pGestures.Add( gesture );
			gesture->FreeReference();
			
		}catch( const deException & ){
			if( gesture ){
				gesture->FreeReference();
			}
			throw;
		}
	}
	
	// take over animator
	pEngAnimator = pose.pEngAnimator;
	if( pEngAnimator ){
		pEngAnimator->AddReference();
	}
	
	// clone controllers
	const int controllerCount = pose.pControllers.GetCount();
	for( i=0; i<controllerCount; i++ ){
		ceActorController *controller = NULL;
		
		try{
			controller = new ceActorController( *pose.pControllers.GetAt( i ) );
			pControllers.Add( controller );
			controller->FreeReference();
			
		}catch( const deException & ){
			if( controller ){
				controller->FreeReference();
			}
			throw;
		}
	}
}

ceActorPose::~ceActorPose(){
	if( pEngAnimator ){
		pEngAnimator->SetRig( NULL );
		pEngAnimator->FreeReference();
	}
}



// Management
///////////////

void ceActorPose::SetName( const char *name ){
	pName = name;
}

void ceActorPose::SetPathAnimator( const char *path ){
	if( pPathAnimator.Equals( path ) ){
		return;
	}
	
	pPathAnimator = path;
	pLoadAnimator();
}



// Private Functions
//////////////////////

void ceActorPose::pLoadAnimator(){
	pControllerNames.RemoveAll();
	
	if( pPathAnimator.IsEmpty() ){
		return;
	}
	
	// load animator
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
		
		// ignore missing or broken animators. this can easily happen during development
		return;
	}
	
	// update controller name list
	const int count = animator->GetControllerCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pControllerNames.Add( animator->GetControllerAt( i )->GetName() );
	}
}
